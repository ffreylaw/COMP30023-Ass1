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
list_t *create_list();

/* Create a new node */
node_t *create_node(void*);

/* Return the length of the list */
int len(list_t *list);

/* Inserts data into the head of list */
void insert_at_head(void*, list_t**);

/* Inserts data into the tail of list */
void insert_at_tail(void*, list_t**);

/* Inserts data before aim */
bool insert_before(void*, void*, list_t**);

/* Pop the head off the list */
void *pop_head(list_t**);

/* Pop the tail off the list */
void *pop_tail(list_t**);

void *del(void*, list_t**);

/* Print list to file by applying print to each node that is not NULL */
void print_list(void (*)(FILE*, void*), FILE*, node_t*);

#endif
