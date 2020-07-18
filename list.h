#include <stdbool.h>
#include <stdlib.h>

typedef struct l_node {
    void *value;
    struct l_node *next;
} list_node;

typedef void (*free_cb) (void *val);

typedef struct {
    list_node *head;
    list_node *tail;
    free_cb free_cb;
} linked_list;

linked_list *list_init(void (*f) (void *val));

bool list_is_empty(linked_list *list);

void list_append(linked_list *list, void *val);

void list_free(linked_list *list);