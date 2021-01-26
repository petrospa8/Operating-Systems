typedef struct tm ttime;
struct tnode {
    char *size;
    char *MemAddr;
    ttime *timeinfo;
    struct tnode *nxt;
};

typedef struct tnode node;

void createList(node *l); // Initialize to NULL
node *insertList(node *l, char *size, char *MemAddr, ttime *timeinfo);
void updateList(node *n, char *MemAddr);
node *searchList(node *l, char *size); // Se non atopa o elemento, devolve NULL
void emptyList(node *l); // Libera a memoria, despues de este metodo
// es necesario igualar el parametro l a null, puesto que no se puede modificar la direccion a la que apunta
void printList(node *l);
