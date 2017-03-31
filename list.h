/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * list.h
 *
 */
#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdbool.h>

typedef struct node node_t;

struct node {
    void *data;
    node_t *next;
    node_t *prev;
};

typedef struct list {
    node_t *head;
    node_t *tail;
} list_t;

/* Create a new list */
list_t *list_init();

/* Create a new node */
node_t *list_create_node(void*);

/* Return the length of the list */
int list_len(list_t *list);

/* Add data into the tail of list */
void list_add(void*, list_t**);

/* Insert data before aim */
bool list_insert(void*, void*, list_t**);

/* Pop the head off the list */
void *list_pop(list_t**);

/* Delete node equals to aim */
void *list_del(void*, list_t**);

/* Replace node equals to aim */
bool list_rplc(void*, void*, list_t**);

/* Print list to file by applying print to each node that is not NULL */
void list_print(void (*)(FILE*, void*), FILE*, node_t*);

#endif
