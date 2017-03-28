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
    list_t *free_list;
} memory_t;

/* Singleton struct for the computer */
typedef struct computer {
    cpu_t *cpu;
    disk_t *disk;
    memory_t *memory;
} computer_t;

/* Get instance from the singleton */
computer_t* get_instance();

/* Initialize the computer */
void initialize_computer(char*, int, int);

/* Initialize a disk */
disk_t *initialize_disk();

/* Initialize a memory with given memsize */
memory_t *initialize_memory(int);

/* Calculate memory usage */
void calculate_memusage();

/* Print a process */
void print_process(FILE*, void*);

/* Print a hole */
void print_hole(FILE *f, void *data);

/* Print a segment */
void print_segment(FILE *f, void *data);

#endif
