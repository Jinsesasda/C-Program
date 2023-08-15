#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "randomdelay.h"

/* Author: Jinseok Back
 * Section: 0208
 * File Description:
 * This is a modified version of the original C file "longlines2.c". It is
 * modified to allow the program to concurrently read files and check if
 * there are any lines that are longer than 80 characters in each file using
 * multiple threads. The main function will create threads and call the
 * function "read_lines" to open and read files. Each thread will join
 * after doing these and return the total number of lines that exceed 80
 * characters. The main function will add the numbers returned by each
 * thread and then print it.
 */

/* Used to set fids array size. */
#define MAX_THREADS 100

static void *read_lines(void *arg);

/* This is a static function that will be only used by the main function
 * in this file. After the main function creates threads, this function
 * will be called to open files and read to check if there are any lines
 * that exceed 80 characters; the parameter "arg" holds the name of the
 * file to be opened. If it fails to read any files, it will just return
 * 0. If successful, it will return the total count of the lines that
 * exceed 80 characters in the file, which could also be 0 if there are
 * none. It is a void function that returns a void pointer. */
static void *read_lines(void *arg) {
  /* Since this function is run by a new thread, it should allocate
   * memory for the return value. */
  long *one_file_count = malloc(sizeof(*one_file_count)), line_length;
  char ch;
  FILE *fp = NULL;

  fp = fopen(arg, "r");

  (*one_file_count) = 0;

  if (fp != NULL) {
    line_length = 0;

    while ((ch = fgetc(fp)) != EOF) { /* Read character by character */
      if (ch != '\n')
	line_length++;
      else {
	if (line_length > 80)
	  (*one_file_count)++;

	line_length = 0; /* Reset for the next line to be read. */
      }
    }

    /* A special case where the last line does not end with a newline. */
    if (line_length > 80)
      (*one_file_count)++;

    fclose(fp);
  }

  /* Uncomment this to check if threads are running
     randomdelay(arg); */

  /* Return a void pointer that points to one_file_count. */
  return (void *) one_file_count;
}

/* This is the main function that will create threads as many as there
 * are files to be read. argc holds the number of arguments and argv
 * holds the file names after its first element. After threads terminate,
 * it will join with them, adding up the total number of lines that
 * exceed 80 characters. Finally, it will print the total. */
int main(int argc, char *argv[]) {
  long total = 0;
  int i;
  pthread_t tids[MAX_THREADS];
  void *temp = NULL;

  /* Check if there's at least one file to be read. */
  if (argc > 1 && argv != NULL) {
    /* Create threads, each calling the "read_lines" function; argv[i]
     * contains the name of the file. */
    for (i = 1; i < argc; i++)
      pthread_create(&tids[i - 1], NULL, read_lines, argv[i]);

    /* Join with terminated threads. Each return value will be stored in
     * temp and then will be added to total. */
    for (i = 1; i < argc; i++) {
      pthread_join(tids[i - 1], &temp);
      total += *((long *) temp);
      free(temp);
    }

    temp = NULL;
  }

  printf("%ld\n", total);

  return 0;
}
