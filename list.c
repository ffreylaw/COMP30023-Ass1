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
list_t *create_list() {
    list_t *new_list = (list_t*)malloc(sizeof(list_t));
	new_list->head = NULL;
	new_list->tail = NULL;
    return new_list;
}

/* Create a new node */
node_t *create_node(void *data) {
    node_t *new_node = (node_t*)malloc(sizeof(node_t));
	new_node->data = data;
	new_node->prev = NULL;
	new_node->next = NULL;
    return new_node;
}

/* Return the length of the list */
int len(list_t *list) {
    int length = 0;
    node_t *node;
    for (node = list->head; node != NULL; node = node->next){ length++; }
    return length;
}

/* Inserts data into the head of list */
void insert_at_head(void *data, list_t **list) {
    node_t *new_node = create_node(data);
    if (*list == NULL) {
    	*list = create_list();
        (*list)->head = new_node;
        (*list)->tail = new_node;
        return;
    }
    if ((*list)->head == NULL) {
        (*list)->head = new_node;
        (*list)->tail = new_node;
        return;
    }
	(*list)->head->prev = new_node;
	new_node->next = (*list)->head;
	(*list)->head = new_node;
}

/* Inserts data into the tail of list */
void insert_at_tail(void *data, list_t **list) {
    node_t *new_node = create_node(data);
    if (*list == NULL) {
    	*list = create_list();
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
void *pop(list_t **list) {
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

/* Inserts data before aim */
bool insert_before(void *aim, void *data, list_t **list) {
    node_t *node = (*list)->head;
    while (node != NULL) {
        if (node->data == aim) {
            node_t *new_node = create_node(data);
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

void *del(void *aim, list_t **list) {
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

bool replace(void *aim, void *data, list_t **list) {
    node_t *node = (*list)->head;
    while (node != NULL) {
        if (node->data == aim) {
            node_t *new_node = create_node(data);
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

/* Free the memory allocated to each list node */
void free_list(node_t *root) {
    if (root) {
        free_list(root->next);
        free(root);
    }
}

/* Print list to file by applying print to each node that is not NULL */
void print_list(void (*print)(FILE *f, void *data), FILE *f, node_t *root) {
    if (root) {
        print(f, root->data);
        print_list(print, f, root->next);
    }
}
