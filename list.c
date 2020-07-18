#include "list.h"

linked_list *list_init(void (*f) (void *val)) {
    linked_list *l = malloc(sizeof(linked_list));
    l->free_cb = f;

    return l;
}

bool list_is_empty(linked_list *list) {
    return list->head == NULL;
}

void list_append(linked_list *list, void *val) {
    list_node *node = malloc(sizeof(list_node));
    node->value = val;

    if (list_is_empty(list)) {
        list->head = node;
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
}

void list_free(linked_list *list) {
    if (list->head != NULL) {
        list_node *node = list->head;
        while (node != NULL) {
            list->free_cb(node->value);
            node->value = NULL;
            node = node->next;
        }

        list->head = NULL;
    }

    list->tail = NULL;
}