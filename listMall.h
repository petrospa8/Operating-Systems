/******************************* LIST MALL **************************************/

struct tnode {
    long size;
    void *MemAddr;
    char *timeinfo;
    struct tnode *nxt;
};

typedef struct tnode node;

void createListMall(node *l);
node *insertListMall(node *l, long size, void *MemAddr, char *timeinfo);
node *searchListMall(node *l, char* size);
void emptyListMall(node *l);
void removeNodeMall(node **l, node *pos);
void printListMall(node *l);

/******************************* LIST MMAP **************************************/

struct tnodeMmap {
    long size;
    void *MemAddr;
    char *timeinfo;
    int fd;
    char *name;
    struct tnodeMmap *nxt;
};

typedef struct tnodeMmap nodeMmap;

void createListMmap(nodeMmap *l);
nodeMmap *insertListMmap(nodeMmap *l, long size, void *MemAddr, char *timeinfo, int fd, char *name);
nodeMmap *searchListMmap(nodeMmap *l, char *name);
void emptyListMmap(nodeMmap *l);
void removeNodeMmap(nodeMmap **l, nodeMmap *pos);
void printListMmap(nodeMmap *l);

/******************************* LIST SHARED **************************************/

struct tnodeShared {
    long size;
    char *MemAddr;
    char *timeinfo;
    int key;
    struct tnodeShared *nxt;
};

typedef struct tnodeShared nodeShared;

void createListShared(nodeShared *l);
nodeShared *insertListShared(nodeShared *l, long size, void *MemAddr, char *timeinfo, int key);
//void updateList(node *n, char *size, char *MemAddr, char *timeinfo, int fd);
nodeShared *searchNodeShared(nodeShared *l, int key);
void emptyListShared(nodeShared *l);
void removeNodeShared(nodeShared **l, nodeShared *pos);
void printListShared(nodeShared *l);
void printNodeShared(nodeShared *node);
