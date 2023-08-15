/* This source file contains the necessary functions for the program to store
 * priority queues. Please see the description of each function below. The
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

  if (prio_q == NULL)
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
      /* This program does not allow elements with the same priority. */
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

  if (is_empty(prio_q))
    return NULL;

  /* It tries to find the last node of the queue because the last
   * node has the highest priority */
  curr = prio_q.head;
  while (curr != NULL && curr -> next != NULL)
    curr = curr -> next;

  return curr -> data;
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

    while (curr != NULL && curr -> next != NULL) {
      prev = curr;
      curr = curr -> next;
    }

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

char **get_all_elements(Prio_que prio_q) {
  Node **head, *curr;
  const int size = (prio_q.size) + 1;
  char **arr;
  int i;

  if (size < 1)
    return NULL;

  head = get_head(&(prio_q.head));

  arr = malloc(sizeof(char **) * size);

  if (head != NULL) {
    curr = *head;

    arr[size - 1] = NULL;

    for (i = size - 2; i >= 0; i--) {
      arr[i] = (curr -> data);
      curr = curr -> next;
    }
  }

  return arr;
}

static Node **get_head(Node **head) {
  return head;
}
