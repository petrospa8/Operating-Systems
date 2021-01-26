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
nodeShared *searchListShared(nodeShared *l, char *key);
void emptyListShared(nodeShared *l);
void printListShared(nodeShared *l);
void printNodeShared(nodeShared *node);
