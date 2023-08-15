typedef struct node {
  char *data;
  unsigned int priority;
  struct node *next;
} Node;

typedef struct Prio_que {
  Node *head;
  unsigned int size;
} Prio_que;
