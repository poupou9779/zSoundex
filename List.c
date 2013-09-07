//#ifdef LINKED_LIST
#include "List.h"
#include <stdlib.h>
#include <string.h>

List *List_init(void) {
    List *ret = malloc(sizeof(List));
    if(ret != NULL) {
        ret->n = 0;
        ret->Beg = ret->End = NULL;
    }
    return ret;
}

List *List_add(char *ptr, List *l) {
    Cell *el = malloc(sizeof(Cell));
    el->next = NULL;
    el->str = ptr;
    if(l->n == 0)
        l->Beg = l->End = el;
    else {
        l->End->next = el;
        l->End = el;
        if(l->n == 1)
            l->Beg->next = l->End;
    }
    ++l->n;
    return l;
}

List *List_free(List *l) {
    Cell *el = l->Beg,
         *tmp;
    while(el != NULL) {
        free(el->str);
        tmp = el;
        el = el->next;
        free(tmp); tmp = NULL;
        --l->n;
    }
    if(l->n == 0) {
        free(l); l = NULL;
    }
    return l;
}

bool List_equal(List *l1, List *l2) {
    if(l1 == NULL || l2 == NULL || l1 == l2) return false;
    if(l1->n != l2->n) return false;
    Cell *el1 = l1->Beg,
         *el2 = l2->Beg;
    for(unsigned i = 0; i < l1->n; ++i) {
        if(strcmp(el1->str, el2->str) != 0)
            return false;
        el1 = el1->next;
        el2 = el2->next;
    }
    return true;
}
//#endif
