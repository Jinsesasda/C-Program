/* Author: Jinseok Back
 * Section #: 0208
 * File description:
 * This source file contains the necessary functions for the program to
 * initialize a data structure (singly-linked list) that would store command
 * line arguments. This data structure is implemented in the header file
 * "simple-sserver-datastructure.h" that is linked by "simple-sserver.h".
 * Also, there are functions to extract these command line arguments from
 * the data structure and actually execute the commands using child
 * processes. The static functions are helper-functions that support these
 * functions to do their jobs. Please read detailed descriptions of each
 * function below. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sysexits.h>
#include <sys/stat.h>
#include <unistd.h>
#include <err.h>
#include <fcntl.h>
#include "simple-sserver.h"
#include "split.h"
#include "safe-fork.h"

/* Function prototypes:
 * (Helper-functions are described in detail at near the end of the file.) */
/* Supports the read_commands() function. */
static void init_que(Commands *const commands);

/* Supports the read_commands() function. */
static void enqueue_compile_cmd(Commands *const commands,
				const char new_cmd[]);

/* Supports the read_commands() function. */
static void enqueue_test_cmd(Commands *const commands,
			     const char new_cmd[]);

/* Supports the compile_program() and test_program() functions. */
static char **io_redir(char *args[], int *last_elem, int *second_from_last,
		       int *in, int *out);

/* This function is called to read two lists of commands, "compile_cmds" for
 * compiling code, and "test_cmds" for testing and compiling the code. It will
 * first initialize the data structure (singly-linked list). After successfully
 * opening the files and reading the command lists, it will call the
 * "enqueue_compile_cmd" and "enqueue_test_cmd" helper-functions to insert
 * data (command) into the initialized data structure. Each node and its data
 * will use dynamically allocated memory. If anything fails during any of these
 * processes, it will exit with status 1; otherwise, it will return the
 * "commands". */
Commands read_commands(const char compile_cmds[], const char test_cmds[]) {
  FILE *fp1, *fp2;
  char new_cmd[LINE_MAX];
  Commands commands;

  if (compile_cmds == NULL || test_cmds == NULL)
    exit(1);

  init_que(&commands);

  fp1 = fopen(compile_cmds, "r");
  if (fp1 != NULL) {
    while (fgets(new_cmd, LINE_MAX, fp1) != NULL) {
      /* Chop off the trailing newline character. */
      new_cmd[strlen(new_cmd) - 1] = '\0';
      enqueue_compile_cmd(&commands, new_cmd);
    }
  } else exit(1);
  fclose(fp1);

  fp2 = fopen(test_cmds, "r");
  if (fp2 != NULL) {
    while (fgets(new_cmd, LINE_MAX, fp2) != NULL) {
      new_cmd[strlen(new_cmd) - 1] = '\0';
      enqueue_test_cmd(&commands, new_cmd);
    }
  } else exit(1);
  fclose(fp2);

  return commands;
}

/* This function deallocates all dynamically allocated memory used by
 * the "commands" data structure. */
void clear_commands(Commands *const commands) {
  Node *temp;

  if (commands != NULL) {
    /* "commands->head_cc" is pointing to the first node of the list
     * that stores compile_cmds. It it's not NULL, there is at least
     * one compile_cmd that uses dynamically allocated memory. */
    while (commands->head_cc != NULL) {
      temp = commands->head_cc;
      commands->head_cc = commands->head_cc->next;
      free(temp->compile_cmd);
      free(temp);
    }
    /* Same here, except for that it checks "head_tc", instead.*/
    while (commands->head_tc != NULL) {
      temp = commands->head_tc;
      commands->head_tc = commands->head_tc->next;
      free(temp->test_cmd);
      free(temp);
    }
  }
}

/* This function will be called to execute all compile cmds that are stored in
 * the "commands" data structure. It will first call the "split" helper-
 * function to split one command line into command line arguments separated by
 * white spaces, and store them in the array "args". Then it will check if
 * there needs to be I/O redirection by calling the "io_redir" helper function.
 * If it requires, it will create a new command line argument array that has
 * removed the "<", ">", "infile", and "outfile". Otherwise, it will create
 * a child process and execute compile cmds. If anything fails, it will return
 * the "FAILED_COMPILATION" value. Otherwise, it will return the "SUCCESSFUL_
 * COMPILATION" value. */
int compile_program(Commands commands) {
  Node *temp;
  pid_t pid;
  int i;

  /* No arguments in the data structure for compile cmds so it skips the
   * next part and immediately returns the exit code SUCCESSFUL_COMPILATION.*/
  if (commands.head_cc == NULL)
    return SUCCESSFUL_COMPILATION;
  else temp = commands.head_cc;

  /* This for loop will execute commands extracted from the data struct.
   * Each iteration represents one compile command line. */
  for (i = 0; i < commands.size_cc; i++) {
    char file[LINE_MAX],file2[LINE_MAX];
    char **io_args, **args = split(temp->compile_cmd);
    /* If in_redir = 1, input redirection is needed; if out_redir = 1,
     * output redirection is needed. "last_elem" holds the index value that
     * will be used to find the file name to be opend for I/O redirection;
     * "second_from_last" is for the second file name if both I/O redirections
     * are used. */
    int j, in_redir = 0, out_redir = 0, last_elem = 0, second_from_last = 0;

    /* If I/O redirection is required, the "io_redir" helper-function
     * will return the new suitable array, else, it will return NULL. */
    io_args = io_redir(args, &last_elem, &second_from_last, &in_redir,
		       &out_redir);

    if (io_args != NULL) {
      strcpy(file, io_args[last_elem]);
      /* Both input and output redirection is required. So it stores the
       * second file to be used. */
      if (in_redir == 1 && out_redir == 1)
	strcpy(file2, io_args[second_from_last]);
    }

    pid = safe_fork();

    if (pid > 0) {
      int status;
      wait(&status);

      if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	return FAILED_COMPILATION;

    } else if (pid == 0) {
      /* ------- Child process begins ------- */
      /* If in_redir and/or out_redir are 1, it redirects I/O properly,
       * then executes the command. */
      if (in_redir == 1 || out_redir == 1) {

	if (in_redir == 1 && out_redir == 0) {
	  int fpd = open(file, O_RDONLY);
	  dup2(fpd, STDIN_FILENO);
	  close(fpd);
	} else
	  if (in_redir == 0 && out_redir == 1) {
	    int fpd = open(file, FILE_FLAGS, FILE_MODE);
	    dup2(fpd, STDOUT_FILENO);
	    close(fpd);
	  } else
	    if (in_redir == 1 && out_redir == 1) {
	      int fpd1, fpd2;

	      fpd1 = open(file, O_RDONLY);
	      dup2(fpd1, STDIN_FILENO);
	      close(fpd1);

	      fpd2 = open(file2, FILE_FLAGS, FILE_MODE);
	      dup2(fpd2, STDOUT_FILENO);
	      close(fpd2);
	    }

	execvp(io_args[0], io_args);

      } else execvp(args[0], args); /* No I/O redirections used */
      /* ------- Child process ends ------- */
    } else return FAILED_COMPILATION;

    /* Free the arguments that use memory before moving onto the next
     * command. */
    for (j = 0; args[j] != NULL; j++)
      free(args[j]);
    free(args);

    if (in_redir == 1 || out_redir == 1)
      free(io_args);

    temp = temp->next;
  }

  return SUCCESSFUL_COMPILATION;
}

/* This function is very similar to the previous function, "compile_program",
 * except for that it will not stop testing commands even if one of them
 * fails. To avoid unecessary, duplicated comments, some comments are removed.
 * Each time a child process is created, it will increment "result" by
 * one if the command execution was successful. It will then return the number
 * of successful command executions by returning "result". */
int test_program(Commands commands) {
  Node *temp;
  pid_t pid;
  int i, result = 0;

  /* No arguments in the data structure for compile cmds so it skips the
   * next part and immediately returns 0. */
  if (commands.head_tc == NULL)
    return 0;
  else temp = commands.head_tc;

  /* This for loop will execute commands extracted from the data struct.
   * Each iteration represents one command line argument. */
  for (i = 0; i < commands.size_tc; i++) {
    char file[LINE_MAX], file2[LINE_MAX];
    char **io_args, **args = split(temp->test_cmd);
    int j, in_redir = 0, out_redir = 0, last_elem = 0, second_from_last = 0;

    io_args = io_redir(args, &last_elem, &second_from_last, &in_redir,
		       &out_redir);

    if (io_args != NULL) {
      strcpy(file, io_args[last_elem]);
      if (in_redir == 1 && out_redir == 1)
	strcpy(file2, io_args[second_from_last]);
    }

    pid = safe_fork();

    if (pid > 0) {
      int status;
      wait(&status);

      if (WIFEXITED(status) && WEXITSTATUS(status) == 0)
	result++;
    } else
      if (pid == 0) {
	/* ------- Child process begins ------- */
	if (in_redir == 1 || out_redir == 1) {

	  if (in_redir == 1 && out_redir == 0) {
	    int fpd = open(file, O_RDONLY);
	    dup2(fpd, STDIN_FILENO);
	    close(fpd);
	  } else
	    if (in_redir == 0 && out_redir == 1) {
	      int fpd = open(file, FILE_FLAGS, FILE_MODE);
	      dup2(fpd, STDOUT_FILENO);
	      close(fpd);
	    } else
	      if (in_redir == 1 && out_redir == 1) {
		int fpd1, fpd2;

		fpd1 = open(file, O_RDONLY);
		dup2(fpd1, STDIN_FILENO);
		close(fpd1);

		fpd2 = open(file2, FILE_FLAGS, FILE_MODE);
		dup2(fpd2, STDOUT_FILENO);
		close(fpd2);
	      }

	  execvp(io_args[0], io_args);

	} else execvp(args[0], args);
	/* ------- Child process ends ------- */
      } else printf("Failed to create a child process.\n");

    /* Free the arguments that use memory before moving onto the next
     * command. */
    for (j = 0; args[j] != NULL; j++)
      free(args[j]);
    free(args);
    
    if (in_redir == 1 || out_redir == 1)
      free(io_args);

    temp = temp->next;
  }

  return result;
}

/* This helper-function is called by the "read_commands" function.
 * It simply initializes the "commands" data structure. */
static void init_que(Commands *const commands) {
  if (commands != NULL) {
    commands->head_cc = NULL;
    commands->head_tc = NULL;
    commands->size_cc = 0;
    commands->size_tc = 0;
  }
}

/* This helper-function is called by the "read_commands" function.
 * Since this program uses the sigly-linked list data structure,
 * this function inserts a new node that stores compile cmds.
 * The new node and compile cmds use dynamically allocated memory. */
static void enqueue_compile_cmd(Commands *const commands,
				const char new_cmd[]) {
  Node *temp = NULL, *new = NULL;
  unsigned int result = 0;

  new = malloc(sizeof(*new));
  if (new != NULL) {
    new->compile_cmd = malloc(strlen(new_cmd) + 1);
    strcpy(new->compile_cmd, new_cmd);
    new->test_cmd = NULL;
    new->next = NULL;
  }

  if (commands->head_cc == NULL) {
    commands->head_cc = new;
    commands->size_cc += 1;
    result++;
  } else {
    temp = commands->head_cc;

    while (temp->next != NULL)
      temp = temp->next;

    temp->next = new;
    commands->size_cc += 1;
    result++;
  }
}

/* This helper-function is called by the "read_commands" function.
 * It is almost identical to the other helper-function, "enqueue_compile_cmd",
 * except for that it inserts a new node for test cmds, instead. */
static void enqueue_test_cmd(Commands *const commands,
			     const char new_cmd[]) {
  Node *temp = NULL, *new = NULL;
  unsigned int result = 0;

  new = malloc(sizeof(*new));
  if (new != NULL) {
    new->test_cmd = malloc(strlen(new_cmd) + 1);
    strcpy(new->test_cmd, new_cmd);
    new->compile_cmd = NULL;
    new->next = NULL;
  }

  if (commands->head_tc == NULL) {
    commands->head_tc = new;
    commands->size_tc += 1;
    result++;
  } else {
    temp = commands->head_tc;

    while (temp->next != NULL)
      temp = temp->next;

    temp->next = new;
    commands->size_tc += 1;
    result++;
  }
}

/* This helper-function is called by both the "compile_program" and "test_
 * program" functions. First, it checks if "args" contains any "<" and/or
 * ">" characters. If it does, it indicates that I/O redirection will be
 * used, so it creates a new command line argument array that has removed
 * the "<", ">", "infile", and "outfile" arguments. And the last element
 * of "args" is stored into "last_elem" parameter (it is the file name to
 * be opened for I/O redirection). Also, if both I/O redirections are used,
 * the second element from the last element is stored into "second_from_last"
 * (it is the second file name to be opened for I/O redirection). */
static char **io_redir(char *args[], int *last_elem, int *second_from_last,
		       int *in, int *out) {
  char **io_args;
  int size, i = 0, j;

  while (args[i] != NULL) {
    if (strcmp(args[i], "<") == 0) 
      (*in) = 1;
    else if (strcmp(args[i], ">") == 0) 
      (*out) = 1;
    i++;
  }

  if ((*in) == 1 || (*out) == 1) {
    size = i;
    io_args = malloc(sizeof(char **) * size);

    /* Only one I/O redirection is needed. */
    if (((*in) == 1 && (*out) == 0) || ((*in) == 0 && (*out) == 0)) {
      for (j = 0; j < size - 2; j++)
	io_args[j] = args[j];
      io_args[j] = NULL;
      (*last_elem) = i - 1;
    } else if ((*in) == 1 && (*out) == 1) { /* Both are used. */
      for (j = 0; j < size - 4; j++) 
	io_args[j] = args[j];
      io_args[j] = NULL;
      (*second_from_last) = i - 3;
    } else io_args[0] = NULL;

    return io_args;
  }

  return NULL;
}
