/**
 * @file dlist.c
 * @brief Implementation of Doubly-Linked List ADT interface.
 * @author Kyle Loudon
 * @date 2009
 */

#include "dlist.h"

void dlist_init(DList *list, void (*destroy)(void *data))
{
        list->size = 0;
        list->destroy = destroy;
        list->head = NULL;
        list->tail = NULL;
}

void dlist_destroy(DList *list)
{
        void *data;

        while (dlist_size(list) > 0)
                if (dlist_remove(list, dlist_tail(list), &data) == 0 &&
                                list->destroy != NULL)
                        list->destroy(data);

        memset(list, 0, sizeof(DList));
}

int dlist_ins_next(DList *list, DListElmt *element, const void *data)
{
        if (element == NULL && dlist_size(list) != 0)
                return -1;

        DListElmt *new_element;
        new_element = malloc(sizeof(DListElmt));
        if (new_element == NULL)
                return -1;

        new_element->data = (void *) data;

        if (dlist_size(list) == 0) {
                list->head = new_element;
                list->head->prev = NULL;
                list->head->next = NULL;
                list->tail = new_element;
        } else {
                new_element->next = element->next;
                new_element->prev = element;

                if (element->next == NULL)
                        list->tail = new_element;
                else
                        element->next->prev = new_element;
                element->next = new_element;
        }
        list->size++;

        return 0;
}

int dlist_ins_prev(DList *list, DListElmt *element, const void *data)
{
        if (element == NULL && dlist_size(list) != 0)
                return -1;

        DListElmt *new_element;
        new_element = malloc(sizeof(DListElmt));
        if (new_element == NULL)
                return -1;

        new_element->data = (void *) data;

        if (dlist_size(list) == 0) {
                list->head = new_element;
                list->head->prev = NULL;
                list->head->next = NULL;
                list->tail = new_element;
        } else {
                new_element->prev = element->prev;
                new_element->next = element;

                if (element->prev == NULL)
                        list->head = new_element;
                else
                        element->prev->next = new_element;
                element->prev = new_element;
        }
        list->size++;

        return 0;
}

int dlist_remove(DList *list, DListElmt *element, void **data)
{
        if (dlist_size(list) == 0 || element == NULL)
                return -1;

        *data = element->data;

        if (element == list->head) {
                list->head = element->next;
                if (list->head == NULL)
                        list->tail = NULL;
                else
                        element->next->prev = NULL;
        } else {
                element->prev->next = element->next;
                if (element->next == NULL)
                        list->tail = element->prev;
                else
                        element->next->prev = element->prev;
        }
        free(element);
        list->size--;

        return 0;
}
