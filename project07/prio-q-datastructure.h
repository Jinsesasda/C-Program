/* Author: Jinseok Back
 * UID: 115775008
 * Section #: 0208

 * This is a header file to be included by "prio-q.c".
 * It has the definitions of the data structure that is a singly-linked list.
 * The data structure is Prio_que in this program, and it contains the
 * nested-struct Node. The head node points to the first element of the
 * linked list and the tail node points to the last element of the linked
 * list. Size refers to the number of elements in the list.
 */
typedef struct node {
  char *data;
  unsigned int priority;
  struct node *next;
} Node;

typedef struct Prio_que {
  Node *head, *tail;
  unsigned int size;
} Prio_que;
