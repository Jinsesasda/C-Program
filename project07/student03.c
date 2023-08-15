#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "prio-q.h"

int main(void) {
  Prio_que prio_q;
  char *elements[] = {"Rocky Racoon", "Earl Squirrel", "Sally Salamander",
		      "Shelly Sheep", "Robin Robin", "Gisselle Gazelle"};
  int priorities[] = {90, 40, 60, 20, 30, 50};
  char *expected_elements[] = {"Rocky Racoon", "Sally Salamander", "Gisselle Gazelle",
			       "Earl Squirrel", "Robin Robin", "Shelly Sheep"};
  /*int expected_priorities[] = {90, 60, 50, 40, 30, 20};*/
  int i;
  init(&prio_q);
  for (i = 0; i < ARRSIZE(elements); i++)
    enqueue(&prio_q, elements[i], priorities[i]);
  assert(change_priority(&prio_q, "Earl Squirrel", 45) == 1);
  for (i = 0; i < ARRSIZE(elements); i++)
    printf("%d\n", get_priority(prio_q, expected_elements[i]));
  return 0;
}
