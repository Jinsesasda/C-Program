#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "prio-q.h"
#include "compare-name-lists.h"

int main(void) {
  Prio_que prio_q;
  char *elements[] = {"Rocky Racoon", "Ellite Elephant", "Wally Walrus",
		      "Aaron Aardvark", "Sally Salamander"};
  int i, priorities[] = {90, 50, 40, 30, 60};
    
  char *expected_elements[] = {"Rocky Racoon", "Sally Salamander", "Aaron Aardvark", NULL};
  
  char **result;

  init(&prio_q);
  for (i = 0; i < ARRSIZE(elements); i++)
    enqueue(&prio_q, elements[i], priorities[i]);
  remove_elements_between(&prio_q, 40, 50);
  
  i = 0;
  result = get_all_elements(prio_q);
  while(result[i] != NULL) {
  printf("%s\n", result[i]);
  i++;
  }
  
assert(compare_name_lists(result, expected_elements) == 1);
  printf("Success!\n");
  
  return 0;
}
