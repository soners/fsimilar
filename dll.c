#include <stdio.h>
#include <stdlib.h>

typedef struct DLL {
  struct Node *header;
  struct Node *trailer;
  int size;
} DLL;

typedef struct Node {
  struct Node *prev;
  struct Node *next;
  void *data;
} Node;

typedef struct Iterator { Node *current; } Iterator;

DLL *initDLL() {
  DLL *dll = (DLL *)malloc(sizeof(DLL));

  Node *header = (Node *)malloc(sizeof(Node));
  Node *trailer = (Node *)malloc(sizeof(Node));

  header->next = trailer;
  trailer->prev = header;

  dll->header = header;
  dll->trailer = trailer;

  return dll;
}

int isEmpty(DLL *dll) { return dll->size == 0; }

int sizeOfList(DLL *dll) { return dll->size; }

void addBetween(Node *prev, Node *next, void *data) {
  Node *new = (Node *)malloc(sizeof(Node));
  new->data = data;

  prev->next = new;
  next->prev = new;

  new->next = next;
  new->prev = prev;
}

void *first(DLL *dll) {
  if (isEmpty(dll)) {
    return NULL;
  }
  return dll->header->next->data;
}

void addFirst(DLL *dll, void *data) {
  addBetween(dll->header, dll->header->next, data);
  dll->size++;
}

void addLast(DLL *dll, void *data) {
  addBetween(dll->trailer->prev, dll->trailer, data);
  dll->size++;
}

void *removeFirst(DLL *dll) {
  if (isEmpty(dll)) {
    return NULL;
  }

  Node *header = dll->header;
  Node *first = header->next;
  void *data = first->data;

  header->next = first->next;
  first->next->prev = header;
  dll->size--;
}

void *removeLast(DLL *dll) {
  if (isEmpty(dll)) {
    return NULL;
  }

  Node *trailer = dll->trailer;
  Node *last = trailer->prev;
  void *data = last->data;

  trailer->prev = last->prev;
  last->prev->next = trailer;
  dll->size--;
}

void printDLL(DLL *dll, void (*printData)(void *)) {
  Node *walk = dll->header->next;
  while (walk != dll->trailer) {
    printData(walk->data);
    walk = walk->next;
  }
}

Iterator *iterator(DLL *dll) {
  Iterator *iter = malloc(sizeof(Iterator));
  iter->current = dll->header;
  return iter;
}

int hasNext(Iterator *iter) { return iter->current->next->data != NULL; }

void *next(Iterator *iter) {
  Node *temp = iter->current;
  void *data = temp->next->data;
  iter->current = temp->next;
  return data;
}
