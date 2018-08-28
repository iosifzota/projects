#include <stddef.h>
#include <stdlib.h>

#include "base.h"

void* emalloc(size_t bytes)
{
    void* ptr = malloc(bytes);
    if (!ptr) exit(1); // TODO

    return ptr;
}

void zerout(void *ptr)
{
    if (ptr) memset(&ptr, 0, 0); /// here
}

struct(Node) {
    Node* nxt;
    char arr[];
};

struct(List) {
    Node* head;
    size_t size;
    size_t cap;   // prop
    size_t node_size;
    void* beg;
    void* end;
};


size_t list_size(List* l)
{
    return (l) ? l->size : 0;
}

size_t list_cap(List *l)
{
    return (l) ? l->cap : 0;
}

void list_free(List *l)
{
    if (l) free(l);
}


void list_new(size_t node_size)
{
    List* l;
    l = (List *) emalloc(sizeof(List));

    l->node_size = node_size;

}
