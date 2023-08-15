/* Author: Jinseok Back
 * Section #: 0208
 * This source file contains the necessary functions for the program to store
 * priority queues. Please see the descriptions of each function below. The
 * header file "prio-q.h" includes another header file "prio-q-datastructure.h"
 * which has the declared type struct called Prio_que. This struct Prio_que
 * represents the priority queue this program intends to build. Inside
 * the struct, there is another type struct, Node. Each node will contain
 * data, priority, and a pointer to the next node. So this program builds a
 * priority queue data structure with a singly-linked list using nodes.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "prio-q.h"

/* This is a helper function. It takes the head node from the struct Prio_que,
 * and then returns a double pointer to it. */
static Node **get_head(Node **head);

/* This function takes the pointer to the struct prio_q. It initializes
 * everything inside the struct Prio_que. So the head node and the size
 * (size of the queue) are initialized here. Nothing happens if the pointer
 * prio_q is NULL. */
void init(Prio_que *const prio_q) {
  if (prio_q != NULL) {
    prio_q -> head = NULL;
    prio_q -> size = 0;
  }
}

/* This function takes the pointer to the struct prio_q; it takes
 * characters from the new_element char array to create a new node; it takes
 * the argument priority that indicates the order for the new node to be
 * created inside the priority queue. It will return 1 if successful or
 * 0 if failed. */
unsigned int enqueue(Prio_que *const prio_q, const char new_element[],
		     unsigned int priority) {
  Node **head, *curr, *prev = NULL, *new = NULL;
  unsigned int result = 0;

  if (prio_q == NULL || new_element == NULL)
    return result;

  /* A double pointer is acquired for the head node */
  head = get_head(&(prio_q -> head));

  /* Creating the new node */
  new = malloc(sizeof(*new));
  if (new != NULL) {
    new -> data = malloc(strlen(new_element) + 1);
    strcpy(new -> data, new_element);
    new -> priority = priority;
    new -> next = NULL;
  }

  if (head != NULL) {
    curr = *head;

    /* It traverses until it finds the appropriate position considering
     * its priority value. It will eventually stop when it sees the end of
     * the list. If the curr node's next is NULL, it indicates the end of
     * the list. */
    while (curr != NULL && curr -> priority <= priority) {
      /* This program does not allow elements to be added with the same
       * priority. */
      if (curr -> priority == priority)
	return result;
      prev = curr;
      curr = curr -> next;
    }

    if (new != NULL) {
      new -> next = curr;

      /* If prev is NULL, it means the queue is empty. So the head node
       * is set with the new node. */
      if (prev == NULL)
	*head = new;
      else prev -> next = new;
    }
    prio_q -> size += 1;
    result = 1;
  }

  return result;
}

/* This function takes the initialized struct prio_q. It checks
 * whether the priority queue is empty. If the queue's head node
 * is NULL, it means the queue is empty. It returns 1 if empty,
 * or 0 if not empty. */
unsigned int is_empty(const Prio_que prio_q) {
  return prio_q.head == NULL;
}

/* This function takes the initialized struct prio_q. It returns
 * the size of the priority queue. */
unsigned int size(const Prio_que prio_q) {
  return prio_q.size;
}

/* This function takes the initialized struct prio_q. It will traverse
 * the priority queue and return the element that has the highest
 * priority. */
char *peek(Prio_que prio_q) {
  Node *curr;
  char *element = "";

  if (is_empty(prio_q))
    return NULL;

  /* It tries to find the last node of the queue because the last
   * node has the highest priority */
  curr = prio_q.head;
  while (curr != NULL && curr -> next != NULL)
    curr = curr -> next;

  element = malloc(strlen(curr -> data) + 1);
  strcpy(element, curr -> data);

  return element;
}

/* This functions takes the pointer to the struct prio_q. It will also
 * traverse the priority queue and find the element with the highest
 * priority. Then it will free (remove) the node that has the element.
 * Then it returns the freed node. */
char *dequeue(Prio_que *const prio_q) {
  Node **head, *curr, *prev = NULL;
  char *temp;

  if (prio_q == NULL || prio_q -> head == NULL)
    return NULL;

  head = get_head(&(prio_q -> head));

  if (head != NULL) {
    curr = *head;

    /* It finds the element with the highest priority. */
    while (curr != NULL && curr -> next != NULL) {
      prev = curr;
      curr = curr -> next;
    }

    /* Changes the linking before freeing the node. */
    if (curr != NULL) {
      if (prev == NULL)
	*head = curr -> next;
      else prev -> next = curr -> next;

      temp = curr -> data;
      free(curr);
    }

    prio_q -> size -= 1;
  }

  return temp;
}

/* This function takes the struct prio_q. It will dynamically allocate
 * memory for an array that consists of pointers to copies of the
 * elements in the priority queue. It will have n + 1 elements, where
 * n is the actual number of elements copied, because the last element
 * of this array must be NULL. This function then returns the array. */
char **get_all_elements(Prio_que prio_q) {
  Node **head, *curr;
  const int size = (prio_q.size) + 1;
  char **arr;
  int i;

  head = get_head(&(prio_q.head));

  arr = malloc(sizeof(char *) * size);
  arr[size - 1] = NULL;

  /* If size < 1, the queue is empty. So it returns the array with only
   * the null element. */
  if (size < 1)
    return arr;

  if (head != NULL) {
    curr = *head;

    /* Initializing the array with pointers to copies of the elements. */
    for (i = size - 2; i >= 0; i--) {
      arr[i] = malloc(strlen(curr->data) + 1);
      strcpy(arr[i], curr->data);
      curr = curr -> next;
    }
  }

  return arr;
}

/* This function takes the pointer to the list of names to be freed. It
 * will free all the elements inside the list as well as the list itself.
 * This helper function can be used on any type of dynamically allocated
 * array of dynamically allocated strings. */
void free_name_list(char *name_list[]) {
  int i = 0;
  if (name_list != NULL) {
    while (name_list[i] != NULL) {
      free(name_list[i]);
      i++;
    }
    free(name_list);
  }
}

/* This function takes the initialized struct prio_q. Its goal is to free
 * all the elements in the queue including their data memory. If the queue
 * is empty, it will have no effect. */
void clear(Prio_que *const prio_q) {
  Node *temp;
  if (prio_q != NULL) {
    while (prio_q->head != NULL) {
      temp = prio_q->head;
      prio_q->head = prio_q->head->next;
      free(temp->data);
      free(temp);
    }
  }
}

/* This function takes the initialized prio_q. It should traverse through
 * the list and find the matching element. If found, it will return its
 * priority value, if not, it will have no effect and just return -1.
 * If there are more than one elements that are the same, but with different
 * priorities, it will then compare their priority values and return the
 * highest one possible. */
int get_priority(Prio_que prio_q, char element[]) {
  Node *curr = prio_q.head;
  int prio = 0;

  while (curr != NULL) {
    /* Sets the prio value to the highest one possible. */
    if (strcmp(curr->data, element) == 0 && curr->priority > prio)
      prio = curr->priority;

    curr = curr -> next;
  }

  /* Matching element not found so return value will be -1. */
  if (prio == 0)
    prio = -1;

  return prio;
}

/* This function takes the pointer to the initialized struct prio_q. It will
 * remove all the elements between low and high values. They are compared
 * with each node's priority value. It should also free the node's data
 * and the node itself as it gets removed from the list. It will return 1 if
 * successful, or 0 if failed. */
unsigned int remove_elements_between(Prio_que *const prio_q, unsigned int low,
				     unsigned int high) {
  Node **head, *temp, *prev;
  unsigned int result = 0;

  if (prio_q != NULL && prio_q->head != NULL) {
    head = get_head(&(prio_q->head));
    temp = *head;

    /* If the head node is within the range, this is a special case where
     * the head node should be set to its next node and then get removed. */
    if (temp != NULL && temp->priority >= low && temp->priority <= high) {
      *head = temp->next;
      free(temp->data);
      free(temp);
      temp = *head;
      result++;
      prio_q->size -= 1;
    }

    /* Otherwise, it will traverse the list and find all the elements that are
     * within the range. Then it will free and remove the elements. */
    while (temp != NULL) {
      while (temp != NULL && (temp->priority < low || temp->priority > high)) {
	prev = temp;
	temp = temp->next;
      }

      if (temp != NULL) {
	prev->next = temp->next;
	free(temp->data);
	free(temp);
	temp = prev->next;
	result++;
	prio_q->size -= 1;
      }
    }
  }

  return result;
}

/* This function takes the initialized struct prio_q. "element" is the
 * element to change its priority to "new_priority". It should then sort
 * the linked list appropriately, if necessary. It will not have any
 * effects if "element" is not present in the list or there exist the same
 * priority values. The functions knows this by checking the result value.
 * If result is greater than one or zero, it will skip all the remaining
 * code and then return 0. If the result value is exactly 1, it will
 * do its job properly. */
unsigned int change_priority(Prio_que *prio_q, char element[],
			     unsigned int new_priority) {
  Node **head, *prev, *curr, *element_to_change, *tail, *temp;
  unsigned int result = 0;

  if (prio_q != NULL && prio_q->head !=NULL) {
    head = get_head(&(prio_q->head));
    temp = *head;

    while (temp != NULL && temp->next != NULL) {
      /* There cannot exit duplicated priority values; sets the result value
       * to just 2 so it won't have any effects afterwards. */
      if (temp->priority == new_priority)
	result = 2;
      if (strcmp(temp->data, element) == 0) {
	element_to_change = temp;
	result++;
      }
      temp = temp->next;
    }
    tail = temp;
  }

  /* If result > 1 there are more than one occurrences of the given element
   * so the return value is set to 0 and it skips the other part. */
  if (result > 1)
    result = 0;
  /* elseif begins here ------------------------------------------*/
  else if (result != 0) {
    element_to_change->priority = new_priority;
    curr = *head;

    /* This while loop operates until the queue is sorted appropriately. */
    while (element_to_change->priority < tail->priority) {
      while (curr != NULL && curr->next != element_to_change) {
	prev = curr;
	curr = curr->next;
      }
      
      /* Pushes the element_to_change forward or backward; else, it assumes
       * everything is sorted and returns the result value which is 1. */
      if (curr != NULL && curr->next == element_to_change &&
	  element_to_change->next->priority < element_to_change->priority) {
	temp = element_to_change->next->next;
	curr->next = element_to_change->next;
	element_to_change->next->next = element_to_change;
	element_to_change->next = temp;
      } else if (curr != NULL && curr->next == element_to_change &&
		 curr->priority > element_to_change->priority) {
	temp = element_to_change->next;
	prev->next = element_to_change;
	element_to_change->next = curr;
	curr->next = temp;
      } else return result;

      curr = *head;
    }

    /* In this case, the element contains the priority value that is
     * greater than the highest priority value in the list. So it should
     * change its last node which holds the old highest priority value. */
    if (element_to_change->priority > tail->priority) {
      while (curr != NULL && curr->next != element_to_change)
	curr = curr->next;

      if (curr != NULL && curr->next == element_to_change) {
	curr->next = element_to_change->next;
	tail->next = element_to_change;
	element_to_change->next = NULL;
      }
    }
  }
  /* elseif ends here --------------------------------------------*/

  return result;
}

static Node **get_head(Node **head) {
  return head;
}
