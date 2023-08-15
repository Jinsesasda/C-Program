#include <stdio.h>
#include <pthread.h>
#include "randomdelay.h"

/* (c) Larry Herman, 2021.  You are allowed to use this code yourself, but
 * not to provide it to anyone else.
 * This program processes a list of zero or more files whose names are given
 * on its command line, counting the number of lines that each file contains
 * that are longer than 80 characters.  It aggregates the number of lines in
 * all the files that are longer than 80 characters and prints the total as
 * its only output.  Files are read one character at a time, by character, *
 * because we don't have any way to know the maximum length of lines.  More
 * realistically, and unlike Project #2, we do not require that the last
 * line of a file end with a newline.
 */

#define MAX_THREADS 100

static void *read_lines(void *arg);

static void *read_lines(void *arg) {
  long one_file_count = 0, line_length;
  char ch;
  FILE *fp = NULL;

  fp = fopen(arg, "r");

  if (fp != NULL) {
    line_length = 0;

    while ((ch = fgetc(fp)) != EOF) {
      if (ch != '\n')
	line_length++;
      else {
	if (line_length > 80)
	  one_file_count++;

	line_length = 0;
      }
    }

    if (line_length > 80)
      one_file_count++;
  }

  fclose(fp);

  printf("%ld\n", one_file_count);

  return NULL;
}

int main(int argc, char *argv[]) {
  /*void *temp = NULL;*/
  long total = 0;
  int i, j;
  pthread_t tids[MAX_THREADS];

  if (argc > 1 && argv != NULL) {
    for (i = 1; i < argc && argv[i] != NULL; i++)
      pthread_create(&tids[i - 1], NULL, read_lines, argv[i]);

    for (j = 1; j < argc && argv[j] != NULL; j++) {
      pthread_join(tids[j - 1], NULL);
    }
  }

  printf("%ld\n", total);

  return 0;
}
