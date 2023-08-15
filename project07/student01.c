#include <stdio.h>
#include <assert.h>
#include "prio-q.h"
#include "memory-checking.h"
#include "compare-name-lists.h"

int main(void) {
  Prio_que prio_q;
  char *elements[] = {"Rocky Racoon", "Wally Walrus", "Aaron Aardvark",
		      "Ginny Giraffe", "Manny Manatee", "Donny Donkey",
		      "Courtney Koala", "Bunny Rabbit"};
  int priorities[] = {90, 40, 60, 20, 30, 50, 80, 70};
  char *expected_elements[] = {"Rocky Racoon", "Courtney Koala", "Bunny Rabbit",
			       "Aaron Aardvark", "Donny Donkey", "Wally Walrus",
			       "Manny Manatee", "Ginny Giraffe", NULL};
  char **result;
  int i;

#if !defined(ENABLE_VALGRIND)
  setup_memory_checking();
#endif

  init(&prio_q);
  for(i = 0; i < ARRSIZE(elements); i++)
    enqueue(&prio_q, elements[i], priorities[i]);
  result = get_all_elements(prio_q);
  assert(compare_name_lists(result, expected_elements) == 1);
  free_name_list(result);
  clear(&prio_q);

#if !defined(ENABLE_VALGRIND)
  check_heap();
#endif

  printf("Yet another test worked right!\n");
  return 0;
}
