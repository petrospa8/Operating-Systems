#include "listMmap.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void createListMmap(nodeMmap *l) {
    l = NULL;
}
/*
nodeMmapcreateNode(){
  nodeMmap*element = malloc(sizeof(node));
  if (element == NULL)
      return NULL;
  else {
    return element;
  }
}
*/

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

/*void updateList(nodeMmap*n, char *MemAddr) {
    if (n != NULL) {
        free(n->MemAddr);
        n->MemAddr = strdup(MemAddr);
    }
}
nodeMmap*searchList(nodeMmap*l, char *MemAddr) {

    while (l != NULL) {
        if ( strcmp(l->MemAddr, MemAddr) == 0)
            return l;
        l = l->nxt;
    }
    return NULL;
}*/

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

void printListMmap(nodeMmap*l) {
    while (l != NULL) {
        printf(" %p: size: %ld. mmap %s (fd:%d) ", l->MemAddr, l->size, l->name ,l->fd);
        printf("%s", l->timeinfo);
        l = l->nxt;
    }
}
