#include <time.h>
struct tnode {
    int size;
    int MemAddr;
    char *date;
    char *op;
    struct tnode *nxt;
};

typedef struct tm * time;
typedef struct tnode node;

node *createList();
node *insertList(node *l, int size, int MemAddr, time date; char *op); 
void updateList(node *n, int size, int MemAddr, time date, char *op);
node *searchList(node *l, int MemAddr);
void emptyList(node *l);
void printList(node *l);
