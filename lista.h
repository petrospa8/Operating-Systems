struct tnode {
    char *var;
    char *value;
    struct tnode *nxt;
};

typedef struct tnode node;

node *createList(); // Inicializa a NULL
node *insertList(node *l, char *var, char *value);
void updateList(node *n, char *value);
node *buscarLista(node *l, char *var); // Se non atopa o elemento, devolve NULL
void vaciarLista(node *l); // Libera a memoria, despues de este metodo
// es necesario igualar el parametro l a null, puesto que no se puede modificar la direccion a la que apunta
void imprimirLista(node *l);
