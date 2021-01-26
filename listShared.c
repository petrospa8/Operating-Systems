#include "listShared.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void createListShared(nodeShared *l) {
    l = NULL;
}
/*
nodeSharedcreateNode(){
  nodeShared*element = malloc(sizeof(node));
  if (element == NULL)
      return NULL;
  else {
    return element;
  }
}
*/

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

/*void updateList(nodeShared*n, char *MemAddr) {
    if (n != NULL) {
        free(n->MemAddr);
        n->MemAddr = strdup(MemAddr);
    }
}*/

nodeShared *searchListShared(nodeShared *l, char *key) {
  nodeShared *aux;
  aux = l;
  int k = atol(key);
    while (aux != NULL) {
        if (aux->key == k)
            return aux;
        aux = aux->nxt;
    }
    return NULL;
}


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
void printListShared(nodeShared*l) {
    nodeShared *aux;
    aux = l;
    while (aux != NULL) {
        printf("%p: size: %ld. shared memory (key %d) ", aux->MemAddr, aux->size,aux->key);
        printf("%s", aux->timeinfo);
        aux = aux->nxt;
    }
}
