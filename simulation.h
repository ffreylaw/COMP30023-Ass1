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
#include "list.h"

/* Constants */
#define NUM_ENTRIES 4

/* Struct to store the entries of a process */
typedef struct {
    int time_created;
    int process_id;
    int memory_size;
    int job_time;
} process_t;

/* Struct for a hole */
typedef struct {
    int address;
    int memory_size;
} hole_t;

/* Struct for a segment */
typedef struct {
    hole_t *hole;
    process_t *process;
} segment_t;

/* Struct represents a CPU */
typedef struct {
    void (*swap)();
    int quantum;
    int num_completed_process;
} cpu_t;

/* Struct represents a disk to store a list of processes */
typedef struct {
    list_t process_list;
} disk_t;

/* Struct represents a main memory */
typedef struct {
    int memsize;
    list_t segment_list;
    list_t free_list;
} memory_t;

/* Singleton struct for the computer */
typedef struct {
    cpu_t *cpu;
    disk_t *disk;
    memory_t *memory;
} computer_t;

/* Clock time singleton */
int *time();

/* Simulate the memory management task */
void simulate(char*, char*, int, int);

/* Add created process to disk */
void step_create_process(list_t*);

/* Load processes from standard input */
list_t load_processes(char*);

/* Get the instance from the singleton structure */
computer_t* get_instance();

/* Initialize the computer */
void initialize_computer(char*, int, int);

/* Initialize a disk */
disk_t *initialize_disk();

/* Initialize a memory with given memsize */
memory_t *initialize_memory(int);

/* First fit algorithm implementation */
void first_fit();

/* Best fit algorithm implementation */
void best_fit();

/* Worst fit algorithm implementation */
void worst_fit();

/* Schedule function */
void schedule();

/* Print a process */
void print_process(FILE*, void*);

/* Print a hole */
void print_hole(FILE *f, void *data);

/* Print a segment */
void print_segment(FILE *f, void *data);

void test_driver();
