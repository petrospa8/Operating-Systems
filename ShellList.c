#include "ShellList.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void createList(node *l) {
    l = NULL;
}

node *insertList(node *l, void *data) {
    node *aux;
    node *element = malloc(sizeof(node));
    aux = l;
    if (element == NULL)
        return NULL;
    else {
      if (aux == NULL){
        element->data = data;
        element->nxt = NULL;
        l = element;
        return element;

      }
      else{
        while (aux->nxt != NULL){
          aux = aux->nxt;
        }
          element->data = data;
          element->nxt = NULL;
          aux->nxt = element;
          return l;
      }
   }
}

node *searchListMall(node *l, char *size) {
    long size2;
    char *end;
    struct data *item;
    item = l->data;
    size2 = strtol(size, &end, 10);
    while (l != NULL) {
        if ( item->size == size2)
            return l;
        l = l->nxt;
    }
    return NULL;
}

void removeNode(node **l, node *pos){ /* Pointer to pointer (&)*/
    node *aux;
    if (*l == NULL)
      return;
    else if (pos == *l){
      *l = pos->nxt;
    }
    else{
      aux = *l;
      while(aux->nxt != pos){
        aux = aux->nxt;
      }
      aux->nxt = pos->nxt;
    }
    free(pos->data);
    free(pos);
    pos = NULL;
}

void printListMall(node *l) {
    node *aux;
    struct data *item;
    aux = l;
    while (aux != NULL) {
        item = aux->data;
        printf(" %p: size: %ld.", item->MemAddr, item->size );
        printf("malloc %s", item->timeinfo);
        aux = aux->nxt;
    }
}

/***************************** LIST MMAP ********************************************/

node *searchListMmap(node *l, char *name) {
  struct data *item;
  item = l->data;
    while (l != NULL) {
        if ( strcmp(item->name, name) == 0)
            return l;
        l = l->nxt;
    }
    return NULL;
}

void printListMmap(node*l) {
  node *aux;
  struct data *item;
  aux = l;
    while (aux != NULL) {
        item = aux->data;
        printf(" %p: size: %ld. mmap %s (fd:%d) ", item->MemAddr, item->size, item->name ,item->fd);
        printf("%s", item->timeinfo);
        aux = aux->nxt;
    }
}

/******************************* LIST SHARED **************************************/
node *searchNodeShared(node *l, int key) {
  node *aux;
  struct data *item;
  aux = l;
  item = aux->data;
    while (aux != NULL) {
        if (item->key == key)
            return aux;
        aux = aux->nxt;
    }
    return NULL;
}

void printNodeShared(node *node){
  struct data *item;
  item = node->data;
  printf("Allocated shared memory (key %d) at %p\n", item->key, item->MemAddr);
}

void printListShared(node*l) {
    node *aux;
    struct data *item;
    aux = l;
    while (aux != NULL) {
        item = aux->data;
        printf("%p: size: %ld. shared memory (key %d) ", item->MemAddr,item->size,item->key);
        printf("%s", item->timeinfo);
        aux = aux->nxt;
    }
}

node *searchListAddr(node *l, char *MemAddr){
  int k;
  k = atoi(MemAddr);
  node *aux;
  struct data *item;
  aux = l;
  item = aux->data;
    while (aux != NULL) {
        if (item->MemAddr == &k)
            return aux;
        aux = aux->nxt;
    }
    return NULL;
}
