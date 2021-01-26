#include "list2.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void createList(node *l) {
    l = NULL;
}
node *insertList(node *l, char *size, char *MemAddr, ttime *timeinfo) {
    node *element = malloc(sizeof(node));
    if (element == NULL)
        return NULL;
    else {
      if (l != NULL){
        element->size = strdup(size);
        element->MemAddr = strdup(MemAddr);
        element->nxt = NULL;
        element->timeinfo = timeinfo;
        l->nxt = element;
        return l;
      }
      else {
        element->size = strdup(size);
        element->timeinfo = timeinfo;
        element->MemAddr = strdup(MemAddr);
        element->nxt = NULL;
        return element;
      }
    }
}

void updateList(node *n, char *MemAddr) {
    if (n != NULL) {
        free(n->MemAddr);
        n->MemAddr = strdup(MemAddr);
    }
}

node *searchList(node *l, char *size) { // Se non atopa o element, devolve NULL
    while (l != NULL) {
        if ( strcmp(l->size, size) == 0)
            return l;
        l = l->nxt;
    }
    return NULL;
}

void emptyList(node *l) {
    node *next;
    while (l != NULL) {
        if (l->nxt != NULL) {
            next = l->nxt;
            free(l->size);
            free(l->MemAddr);
            free(l);
            l = next;
        } else {
            free(l->size);
            free(l->MemAddr);
            free(l);
            l=NULL;
        }
    }
}

void printList(node *l) {
    while (l != NULL) {
        printf("-> %s, %s ", l->size, l->MemAddr);
        printf("%s ", l->timeinfo->tm_mon);
        printf("%d ", l->timeinfo->tm_mday);
        printf("%d:", l->timeinfo->tm_hour);
        printf("%d ", l->timeinfo->tm_min);
        printf("\n");
        l = l->nxt;
    }
}
