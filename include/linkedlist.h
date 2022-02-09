#ifndef LINKEDLIST_H

#define LINKEDLIST_H

struct LinkedListNode
{
    void *data;
    struct LinkedListNode *next;
};

struct LinkedListNode *linked_list_create(void *data);

void linked_list_push(struct LinkedListNode *list, struct LinkedListNode *node);

#endif