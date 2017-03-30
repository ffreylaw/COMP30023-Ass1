/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * simulation.h
 *
 */
#ifndef SIMULATION_H
#define SIMULATION_H

#include <stdbool.h>

#include "list.h"

/* Constants */
#define NUM_ENTRIES 4

/* Event enumeration */
typedef enum {
    NONE    = 0,
    E1      = 1,  // A process has been created and memory is currently empty.
    E2      = 2,  // The quantum has expired for the process running on the CPU.
    E3      = 3,  // A process that was running on the CPU has called exit and terminated.
} event;

/* Struct represents a generator to store all read processes from input */
typedef struct generator {
    list_t *all_processes;
    int num_processes;
} generator_t;

/* Simulate the memory management task */
void simulate(char*, char*, int, int);

/* Get clock time from the singleton */
int *time();

/* Observe and return an event */
int observe_event();

/* Step in CPU */
void cpu_step();

/* Generate created process in disk */
void generator_step(generator_t*);

/* Load processes from given input file */
list_t *load_processes(char*);

/* Initialize a generator */
generator_t *initialize_generator(char*);

#endif
