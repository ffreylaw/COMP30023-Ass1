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

/* Struct represents a disk to store a list of processes */
typedef struct {
    list_t process_list;
} disk_t;

/* Struct represents a main memory */
typedef struct {
    int memsize;
} memory_t;

/* Singleton struct for the CPU */
typedef struct {
    disk_t *disk;
    memory_t *memory;
} cpu_t;

/* Simulate the memory management task */
void simulate(char*, char*, int, int);

/* Get the instance from the singleton structure */
cpu_t* get_instance();

/* Load processes from standard input */
disk_t *load_processes();

/* Print a process */
void print_process(FILE*, void*);
