#include "list.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

node *createList() {
    return NULL;
}
node *insertList(node *l, long size, int MemAddr, time date; char *op);
    node *element = malloc(sizeof(node));
    if (element == NULL)
        return NULL;
        time = gmtime(&(mystat.st_mtime));
    element->size = size;
    element->MemAddr = MemAddr;
    /*
    element->time = date;
    element->op = *op;
    */
    element->nxt = l;
    return element;
}

void updateList(node *n, long size, int MemAddr, time date, char *op);
    if (n != NULL) {
        free(n->MemAddr);
        element->size = size;
        element->MemAddr = MemAddr;
        /*
        element->hour = hour;
        element->minute = minute;
        element->second = second;
        element->year = year;
        element->month = month;
        element->day = day;
        element->op = *op;
        */
        element->nxt = l;
    }
}

node *searchList(node *l, int MemAddr) { // Se non atopa o element, devolve NULL
    while (l != NULL) {
        if (l->MemAddr == MemAddr)
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
        printf("%d: size:%d. %s ",l->MemAddr,l->size);
        l = l->nxt;
    }
    printf("\n");
}
