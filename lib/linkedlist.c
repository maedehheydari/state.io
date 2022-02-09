
#include <stdio.h>

#include <stdlib.h>

#include <stdbool.h>

#include <time.h>

#include "../include/linkedlist.h"

struct LinkedListNode *linked_list_create(void *data)
{
    struct LinkedListNode *node = malloc(sizeof(struct LinkedListNode));
    node->next = NULL;
    node->data = data;
    return node;
}

void linked_list_push(struct LinkedListNode *list, struct LinkedListNode *node)
{
    struct LinkedListNode *tmp = list->next;
    node->next = tmp;
    list->next = node;
}