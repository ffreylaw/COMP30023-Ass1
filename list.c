/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * list.c
 *
 */
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

/* Create a new list */
list_t *list_init() {
    list_t *new_list = (list_t*)malloc(sizeof(list_t));
	new_list->head = NULL;
	new_list->tail = NULL;
    return new_list;
}

/* Create a new node */
node_t *list_create_node(void *data) {
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
    return new_node;
}

/* Return the length of the list */
int list_len(list_t *list) {
    int length = 0;
    node_t *node;
    for (node = list->head; node != NULL; node = node->next){ length++; }
    return length;
}

/* Add data into the tail of list */
void list_add(void *data, list_t **list) {
    node_t *new_node = list_create_node(data);
    if (*list == NULL) {
    	*list = list_init();
        (*list)->head = new_node;
        (*list)->tail = new_node;
        return;
    }
    if ((*list)->tail == NULL) {
        (*list)->head = new_node;
        (*list)->tail = new_node;
        return;
    }
	(*list)->tail->next = new_node;
	new_node->prev = (*list)->tail;
	(*list)->tail = new_node;
}

/* Pop the head off the list */
void *list_pop(list_t **list) {
    if (!((*list)->head)) {
        return NULL;
    } else {
        void *data = (*list)->head->data;
        // node_t *ptr = (*list)->head;
        /* Update list to point at the next element */
        (*list)->head = (*list)->head->next;
        if ((*list)->head != NULL) {
            (*list)->head->prev = NULL;
        } else {
            (*list)->tail = NULL;
        }
        /* Free the memory allocated to the list node */
        // free(ptr);
        return data;
    }
}

/* Insert data before aim */
bool list_insert(void *aim, void *data, list_t **list) {
    node_t *node = (*list)->head;
    while (node != NULL) {
        if (node->data == aim) {
            node_t *new_node = list_create_node(data);
            if (node->prev != NULL) {
                node->prev->next = new_node;
            } else {
                (*list)->head = new_node;
            }
            new_node->prev = node->prev;
            node->prev = new_node;
            new_node->next = node;
            return true;
            break;
        }
        node = node->next;
    }
    return false;
}

/* Delete node equals to aim */
void *list_del(void *aim, list_t **list) {
    node_t *node = (*list)->head;
    while (node != NULL) {
        if (node->data == aim) {
            void *data = node->data;
            if (node->prev != NULL) {
                node->prev->next = node->next;
            } else {
                (*list)->head = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            } else {
                (*list)->tail = node->prev;
            }
            return data;
            break;
        }
        node = node->next;
    }
    return NULL;
}

/* Replace node equals to aim */
bool list_rplc(void *aim, void *data, list_t **list) {
    node_t *node = (*list)->head;
    while (node != NULL) {
        if (node->data == aim) {
            node_t *new_node = list_create_node(data);
            if (node->prev != NULL) {
                node->prev->next = new_node;
            } else {
                (*list)->head = new_node;
            }
            if (node->next != NULL) {
                node->next->prev = new_node;
            } else {
                (*list)->tail = new_node;
            }
            new_node->next = node->next;
            new_node->prev = node->prev;
            return true;
            break;
        }
        node = node->next;
    }
    return false;
}

/* Print list to file by applying print to each node that is not NULL */
void list_print(void (*print)(FILE *f, void *data), FILE *f, node_t *root) {
    if (root) {
        print(f, root->data);
        list_print(print, f, root->next);
    }
}
