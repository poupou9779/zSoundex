#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

typedef struct Cell {
    struct Cell *next;
    char *str;
} Cell;

typedef struct {
    unsigned n;
    Cell *Beg,
         *End;
} List;

List *List_init(void);
List *List_add(char *, List *);
List *List_free(List *);
bool  List_equal(List *, List *);

#endif // LIST_H
