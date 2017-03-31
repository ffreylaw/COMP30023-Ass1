/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * computer.h
 *
 */
#ifndef COMPUTER_H
#define COMPUTER_H

#include "list.h"

/* Struct to store the entries of a process */
typedef struct process {
    int time_created;
    int process_id;
    int memory_size;
    int job_time;

    int time_placed_on_disk;
    int time_placed_on_memory;
} process_t;

/* Struct for a hole */
typedef struct hole {
    int memory_size;
} hole_t;

/* Struct for a segment */
typedef struct segment {
    hole_t *hole;
    process_t *process;
} segment_t;

/* Struct represents a CPU */
typedef struct cpu {
    void (*swap)();
    void (*schedule)(int);
    int quantum;
    list_t *process_queue;
    process_t *running_process;
    int running_time;
    int num_completed_process;
} cpu_t;

/* Struct represents a disk to store a list of created processes */
typedef struct disk {
    list_t *process_list;
} disk_t;

/* Struct represents a main memory */
typedef struct memory {
    int memsize;
    int memusage;
    int num_processes;
    int num_holes;
    list_t *segment_list;
} memory_t;

/* Singleton struct for the computer */
typedef struct computer {
    cpu_t *cpu;
    disk_t *disk;
    memory_t *memory;
} computer_t;

/* Get instance from the singleton */
computer_t* computer_get_instance();

/* Initialize the computer */
void computer_init(char*, int, int);

/* Initialize the CPU */
cpu_t *cpu_init(char*, int);

/* Initialize a disk */
disk_t *disk_init();

/* Initialize a memory with given memsize */
memory_t *memory_init(int);

/* Calculate memory usage */
void memory_calculate_memusage();

/* Get a process from disk */
process_t *disk_get_process();

/* Get a process from memory segments */
process_t *memory_get_process();

/* Delete a process from disk */
process_t *disk_del_process(process_t*);

/* Delete a process from memory segments */
process_t *memory_del_process(process_t*);

/* Print a process */
void process_print(FILE*, void*);

/* Print a hole */
void hole_print(FILE *f, void *data);

/* Print a segment */
void segment_print(FILE *f, void *data);

#endif
