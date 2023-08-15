/* Author: Jinseok Back
 * Section #: 0209
 * File description:
 * This is a header file that is linked by another header file that's
 * named "simple-sserver.h". It provides definitions for the data
 * structure to be used by "simple-sserver.c". It is a singly-linked
 * list data structure, so it uses nodes and each node contains data
 * for either compile_cmds or test_cmds (so one of them would be set
 * to NULL). "Commands" struct has two head nodes which would be pointing
 * at two different linked lists (compile_cmds and test_cmds). */

/* As previously explained in the file description, each node stores data
 * for either compile_cmd or test_cmd; one of them would be set to NULL.*/
typedef struct node {
  char *compile_cmd, *test_cmd;
  struct node *next;
} Node;

/* The head_cc pointer will be pointing to the first node of the compile
 * cmd list. The head_tc pointer will be pointing to the first node of
 * the test cmd list. "size_cc" holds the size of the compile_cmd list,
 * "size_tc" holds the size of the test_cmd list. */
typedef struct Commands {
  Node *head_cc, *head_tc;
  unsigned int size_cc, size_tc;
} Commands;
