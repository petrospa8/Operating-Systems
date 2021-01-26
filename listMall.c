#include "listMall.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void createListMall(node *l) {
    l = NULL;
}

node *insertListMall(node *l, long size, void *MemAddr, char *timeinfo) {
    node *aux;
    node *element = malloc(sizeof(node));
    aux = l;
    if (element == NULL)
        return NULL;
    else {
      if (aux == NULL){
        element->size = size;
        element->timeinfo = timeinfo;
        element->MemAddr = MemAddr;
        element->nxt = NULL;
        l = element;
        return element;

      }
      else{
        while (aux->nxt != NULL){
          aux = aux->nxt;
        }
          element->size = size;
          element->timeinfo = timeinfo;
          element->MemAddr = MemAddr;
          element->nxt = NULL;
          aux->nxt = element;
          return l;
      }
   }
}

void updateListMall(node *n, void *MemAddr) {
    if (n != NULL) {
        free(n->MemAddr);
        n->MemAddr = strdup(MemAddr);
    }
}

node *searchListMall(node *l, char *size) {
    long size2;
    char *end;
    size2 = strtol(size, &end, 10);
    while (l != NULL) {
        if ( l->size == size2)
            return l;
        l = l->nxt;
    }
    return NULL;
}

void emptyListMall(node *l) {
    node *next;
    while (l != NULL) {
        if (l->nxt != NULL) {
            next = l->nxt;
            free(l->MemAddr);
            free(l);
            l = next;
        } else {
            free(l->MemAddr);
            free(l);
            l=NULL;
        }
    }
}

void removeNodeMall(node **l, node *pos){ /* Pointer to pointer (&)*/
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
    free(pos->MemAddr);
    free(pos);
    pos = NULL;
}

void printListMall(node *l) {
    node *aux;
    aux = l;
    while (aux != NULL) {
        printf(" %p: size: %ld.", aux->MemAddr, aux->size );
        printf("malloc %s", aux->timeinfo);
        aux = aux->nxt;
    }
}

/***************************** LIST MMAP ********************************************/

void createListMmap(nodeMmap *l) {
    l = NULL;
}

nodeMmap *insertListMmap(nodeMmap*l, long size, void *MemAddr, char *timeinfo, int fd, char *name) {
    nodeMmap*aux;
    nodeMmap*element = malloc(sizeof(nodeMmap));
    aux = l;
    if (element == NULL)
        return NULL;
    else {
      if (aux == NULL){
        element->size = size;
        element->timeinfo = timeinfo;
        element->MemAddr = MemAddr;
        element->fd = fd;
        element->name = strdup(name);
        element->nxt = NULL;
        l = element;
        return l;
      }
      else{
        while (aux->nxt != NULL){
          aux = aux->nxt;
        }
          element->size = size;
          element->timeinfo = timeinfo;
          element->MemAddr = MemAddr;
          element->fd = fd;
          element->name = strdup(name);
          element->nxt = NULL;
          aux->nxt = element;
          return l;
      }
   }
}

nodeMmap *searchListMmap(nodeMmap*l, char *name) {

    while (l != NULL) {
        if ( strcmp(l->name, name) == 0)
            return l;
        l = l->nxt;
    }
    return NULL;
}

void emptyListMmap(nodeMmap*l) {
    nodeMmap*next;
    while (l != NULL) {
        if (l->nxt != NULL) {
            next = l->nxt;
            free(l->MemAddr);
            free(l);
            l = next;
        } else {
            free(l->MemAddr);
            free(l);
            l=NULL;
        }
    }
}

void removeNodeMmap(nodeMmap **l, nodeMmap *pos){ /* Pointer to pointer (&)*/
    nodeMmap *aux;
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
    free(pos);
    pos = NULL;
}

void printListMmap(nodeMmap*l) {
    while (l != NULL) {
        printf(" %p: size: %ld. mmap %s (fd:%d) ", l->MemAddr, l->size, l->name ,l->fd);
        printf("%s", l->timeinfo);
        l = l->nxt;
    }
}

/******************************* LIST SHARED **************************************/

void createListShared(nodeShared *l) {
    l = NULL;
}

nodeShared *insertListShared(nodeShared*l, long size, void *MemAddr, char *timeinfo, int key) {
    nodeShared*aux;
    nodeShared*element = malloc(sizeof(nodeShared));
    aux = l;
    if (element == NULL)
        return NULL;
    else {
      if (aux == NULL){
        element->size = size;
        element->timeinfo = timeinfo;
        element->MemAddr = MemAddr;
        element->key = key;
        element->nxt = NULL;
        l = element;
        return l;
      }
      else{
        while (aux->nxt != NULL){
          aux = aux->nxt;
        }
          element->size = size;
          element->timeinfo = timeinfo;
          element->MemAddr = MemAddr;
          element->key = key;
          element->nxt = NULL;
          aux->nxt = element;
          return l;
      }
   }
}

nodeShared *searchNodeShared(nodeShared *l, int key) {
  nodeShared *aux;
  aux = l;
    while (aux != NULL) {
        if (aux->key == key)
            return aux;
        aux = aux->nxt;
    }
    return NULL;
}
/*
nodeShared *searchAddrShared(nodeShared *l, void *p) {
  nodeShared *aux;
  aux = l;
    while (aux != NULL) {
        if (aux->MemAddr == p)
            return aux;
        aux = aux->nxt;
    }
    return NULL;
}*/

void emptyListShared(nodeShared*l) {
    nodeShared *next;
    while (l != NULL) {
        if (l->nxt != NULL) {
            next = l->nxt;
            free(l->MemAddr);
            free(l);
            l = next;
        } else {
            free(l->MemAddr);
            free(l);
            l=NULL;
        }
    }
}
void printNodeShared(nodeShared *node){
  printf("Allocated shared memory (key %d) at %p\n", node->key, node->MemAddr);
}

void removeNodeShared(nodeShared **l, nodeShared *pos){ /* Pointer to pointer (&)*/
    nodeShared *aux;
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
    free(pos);
    pos = NULL;
}

void printListShared(nodeShared*l) {
    nodeShared *aux;
    aux = l;
    while (aux != NULL) {
        printf("%p: size: %ld. shared memory (key %d) ", aux->MemAddr, aux->size,aux->key);
        printf("%s", aux->timeinfo);
        aux = aux->nxt;
    }
}
