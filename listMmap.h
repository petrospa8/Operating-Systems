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
//void updateList(node *n, char *size, char *MemAddr, char *timeinfo, int fd);
//node *searchList(node *l, char *name);
void emptyListMmap(nodeMmap *l);
void printListMmap(nodeMmap *l);
