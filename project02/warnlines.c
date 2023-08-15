/* This program is designed to read input and then determine whether each line
 * of the input contains more than 80 characters. If it contains more than 80
 * characters in a line, then the output will contain an asterisk in the
 * beginning of that line; if not, it will just print a blank space instead.
 * Each output line will indicate its line number that is no more than 99999.
 * The input line cannot contain more than 1000 characters. */

#include<stdio.h>

int main(void) {
  char line[999];
  signed int index_input = 0;
  signed int index_output;
  signed int length = 0;
  signed int size = 0;
  signed int tabs_spaces = 0;
  signed int line_number = 1;
  signed int tab_stop = 0;
  signed char space_index;

  while (scanf("%c", &line[index_input]) != EOF) {
    if (line[index_input] == '\n') {
      /* It will first determine whether to print a space or an asterisk
       * depending on the length of the line. */
      if (length > 80) {
	printf("*%5d: ", line_number);
      } else {
	printf(" %5d: ", line_number);
      }
      /* Prints all the characters of the line. */
      for (index_output = 0; index_output < size; index_output++) {
	printf("%c", line[index_output]);
      }
      printf("\n");
      if (length > 80) {
	/* 88 (8 for line description + 80 characters) blank spaces are printed
	 * before printing any carets. */
	for (space_index = 0; space_index < 88; space_index++) {
	  printf(" ");
	}
	/* It will then print as many carets as there are characters over 80. */
	for (index_output = 80; index_output < length; index_output++) {
	  printf("^");
        }
	printf("\n");
      }
      line_number++;

      /* Line ends here, so everything is set back to 0. */
      index_input = 0;
      size = 0;
      length = 0;
      tabs_spaces = 0;
    } else {
      ++size;
      ++length;
      if (line[index_input] == '\t') {
	/* next tab_stop is determined here. */
	tab_stop = (length + 9) / 8 * 8;
	/* tabs_spaces is used to increase length by the appropriate amount
	 * depending on where the next tab stop is. */
	tabs_spaces = tab_stop - length;
	length += tabs_spaces;
      }
      /* index_input is incremented by one until it reaches the end of the line */
      index_input++;
    }
  }
  return 0;
}
