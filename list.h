/*
 * Clement Poh (cpoh@unimelb.edu.au)
 *
 * This module contains useful functions for list manipulation and creation.
 *
 * XXX: This module may be used as a black box. It is not necessary to
 * XXX: understand the implementation details of the functions to use them.
 * XXX: The intrepid student is welcome to explore and learn the dark arts.
 *
*/
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stdio.h>

/* list_t is a pointer to a list_t struct */
typedef struct list_t* list_t;

struct list_t {
    void *data;
    list_t next;
};

/* Pushes data as the new head of list. May be used to create a new list:
 * new_list = push(NULL, data) */
extern list_t push(list_t list, void *data);

/* Pop the head off the list */
extern void *pop(list_t *list);

/* Return the length of the list */
extern int len(list_t list);

/* Returns a reversed copy of list */
list_t reverse(list_t list);

/* Prepend data to list and update list */
extern list_t prepend(list_t *list, void *data);

/* Append l1 to the end of l2 */
void append(list_t l1, list_t *l2);

/* Inserts data into the tail of list */
void insert(void *data, list_t *list);

/* Inserts data into the tail of list or position equal to the next element */
void insert_by(bool (*eq)(void *data, void *node), void *data, list_t *list);

/* Inserts data into the tail of list. Returns true if sucessful,
 * false if it finds an element already equal to data */
bool insert_if(bool (*eq)(void *data, void *node), void *data, list_t *list);

/* Returns the node equal to aim in list, returns NULL if not found */
extern list_t find(bool (*eq)(void *aim, void *node), void *aim, list_t list);

/* Removes and returns the element equal to aim in list,
 * returns NULL if not found */
extern void *del(bool (*eq)(void *aim, void *node), void *aim, list_t *list);

/* Returns a new list that passes the predicate p */
list_t filter(bool (*p)(void *data), list_t list);

/* Print list to f by applying print to each node that is not NULL */
extern void print_list(void (*print)(FILE *f, void *data), FILE *f, list_t node);

/* Free the memory allocated to each list node */
extern void free_list(list_t node);

#endif
