/******************************* LIST MALL **************************************/
struct data {
    void *MemAddr;
    long size;
    char *timeinfo;
    int fd;
    char *name;
    int key;
};

struct tnode {
    void *data;
    struct tnode *nxt;
};

typedef struct tnode node;

void createList(node *l);
node *insertList(node *l, void *data);
node *searchListMall(node *l, char* size);
void removeNode(node **l, node *pos);
void printListMall(node *l);

/******************************* LIST MMAP **************************************/
node *searchListMmap(node *l, char *name);
void printListMmap(node *l);

/******************************* LIST SHARED **************************************/

node *searchNodeShared(node *l, int key);
void printListShared(node *l);
void printNodeShared(node *node);
node *searchListAddr(node *l, void *MemAddr);
