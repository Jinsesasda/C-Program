/* Author: Jinseok Back
 * This program is designed to print a summary of the output produced by
 * the warnlines.c program. It reads each line and checks whether there
 * is an asterisk in the beginning of the line (if there is an asterisk,
 * it means the line contains more than 80 characters). This program will
 * display ONLY the line numbers of the lines that contain more than 80
 * characters. */

#include<stdio.h>

int main(void) {
  char line[999];
  signed int index_input = 0;
  signed int index_output;
  /* If there are more than one line numbers to display, the line numbers
   * are separated by a white space and they are printed in one line. So
   * the variable space is used to determine whether a line should include 
   * a white space in front of the line number or not.
   * (Used at line 27 and 38) */
  char space = 0;

  while (scanf("%c", &line[index_input]) != EOF) {
    if (line[index_input] == '\n') {
      if (line[0] == '*') {
	/* If space is 0, it means it's the very first line number to be
	 * displayed so there isn't any white space printed. If more than
	 * 1, then there is a white space to separate the line numbers. */
	if (space > 0) {
	  printf(" ");
	}
	/* The line number is printed here. */
	for (index_output = 1; index_output < 6; index_output++) {
	  /* It skips over white spaces and finds numbers that will make
	   * up the line number. */
	  if (line[index_output] != ' ') {
	    printf("%c", line[index_output]);
	  }
	}
	if (space == 0) {
	  space++;
	}
      }
      index_input = 0;
    } else {
      index_input++;
    }
  }
  printf("\n");
  return 0;
}
