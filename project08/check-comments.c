#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include "safe-fork.h"

int main(int argc, char **argv) {
  int pfd1[2], pfd2[2];
  int result1 = 0, result2 = 0;
  pid_t pid1, pid2;
  char *args1[3] = {"./remove-comment.x", NULL, NULL};
  char *args2[4] = {"wc", "-m", NULL, NULL};

  /* argv[1] contains the name of the file. */
  args1[1] = argv[1];
  args2[2] = argv[1];

  /* Checks if there exists a second argument, which will hold the name
   * of the file. */
  if (*++argv == NULL) {
    printf("Not valid command-line argument(s) for this program.\n");
    exit(2);
  }
  
  /* Creates pipes. */
  if (pipe(pfd1) == -1) {
    printf("Failed to create a pipe.\n");
    exit(1);
  }
  if (pipe(pfd2) == -1) {
    printf("Failed to create a pipe.\n");
    exit(1);
  }

  /* First child process begin --------------------------------- */
  pid1 = safe_fork();

  if (pid1 < 0) {
    printf("Failed to create a child.\n");
    exit(1);
  } else if (pid1 > 0) {
    dup2(pfd1[0], STDIN_FILENO);
    close(pfd1[0]);
    close(pfd1[1]);
    scanf("%d", &result1);
    
    /* Second child process begin ------------------------------ */
    pid2 = safe_fork();

    if (pid2 < 0) {
      printf("Failed to create a child.\n");
      exit(1);
    } else if (pid2 > 0) {
      dup2(pfd2[0], STDIN_FILENO);
      close(pfd2[0]);
      close(pfd2[1]);
      scanf("%d", &result2);
    } else {
      dup2(pfd2[1], STDOUT_FILENO);
      close(pfd2[0]);
      close(pfd2[1]);
      execv("/usr/bin/wc", args2);
      exit(0);
    }
    /* Second child process end -------------------------------- */

  } else {
    dup2(pfd1[1], STDOUT_FILENO);
    close(pfd1[0]);
    close(pfd1[1]);
    execv("./remove-comments.x", args1);
    exit(0);
  }
  /* First child process end ----------------------------------- */

  /* Compares the results produced by the other programs that were run. */
  result1 *= 2;
  if (result2 > result1) {
    printf("Enough comments!\n");
    exit(0);
  } else {
    printf("Too few comments!\n");
    exit(1);
  }

  return 0;
}
