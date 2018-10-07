typedef struct DLL {
} DLL;

typedef struct Iterator {
} Iterator;

DLL *initDLL();
void addFirst(DLL *, void *);
void addLast(DLL *, void *);
void *first(DLL *);
void *removeFirst(DLL *);
void *removeLast(DLL *);
int sizeOfList(DLL *);
int isEmpty(DLL *);
void printDLL(DLL *, void (*)(void *));
Iterator *iterator(DLL *);
int hasNext(Iterator *);
void *next(Iterator *);
