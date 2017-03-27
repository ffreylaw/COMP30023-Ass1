#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "simulation.h"
#include "list.h"

/* Simulate the memory management task */
void simulate(char *filename, char *algorithm_name, int memsize, int quantum) {
    initialize_computer(filename, algorithm_name, memsize, quantum);
    computer_t *computer = get_instance();
    cpu_t *cpu = computer->cpu;
    disk_t *disk = computer->disk;
    memory_t *memory = computer->memory;

    // print_list(print_process, stdout, disk->process_list);
    // print_list(print_segment, stdout, memory->segment_list);
    // print_list(print_hole, stdout, memory->free_list);

}

/* Clock time singleton */
int *time() {
    static int *time = NULL;
    if (time == NULL) {
        time = (int*)malloc(sizeof(int));
        *time = 0;
    }
    return time;
}

/* Initialize the computer */
void initialize_computer(char *filename, char *algorithm_name, int memsize, int quantum) {
    computer_t *computer = get_instance();
    computer->disk = load_processes(filename);
    computer->memory = initialize_memory(memsize);
    if (!strcmp(algorithm_name, "first")) {
        computer->cpu->swap = first_fit;
    } else if (!strcmp(algorithm_name, "best")) {
        computer->cpu->swap = best_fit;
    } else if (!strcmp(algorithm_name, "worst")) {
        computer->cpu->swap = worst_fit;
    }
    computer->cpu->quantum = quantum;
}

/* Get the instance from the singleton structure */
computer_t* get_instance() {
    static computer_t *instance = NULL;
    if (instance == NULL) {
        instance = (computer_t*)malloc(sizeof(computer_t));
        instance->cpu = (cpu_t*)malloc(sizeof(cpu_t));
        instance->disk = NULL;
        instance->memory = NULL;
    }
    return instance;
}

/* Load processes from standard input */
disk_t *load_processes(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Fail to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    list_t process_list = NULL;
    int time_created, process_id, memory_size, job_time;

    while (fscanf(f, "%d %d %d %d\n", &time_created, &process_id, &memory_size, &job_time) == NUM_ENTRIES) {
        process_t *process = (process_t*)malloc(sizeof(process_t));
        process->time_created = time_created;
        process->process_id = process_id;
        process->memory_size = memory_size;
        process->job_time = job_time;
        insert(process, &process_list);
    }

    disk_t *disk = (disk_t*)malloc(sizeof(disk_t));
    disk->process_list = process_list;
    return disk;
}

memory_t *initialize_memory(int memsize) {
    memory_t *memory = (memory_t*)malloc(sizeof(memory_t));
    memory->memsize = memsize;
    memory->segment_list = NULL;
    memory->free_list = NULL;

    segment_t *segment = (segment_t*)malloc(sizeof(segment_t));
    segment->process = NULL;
    hole_t *hole = (hole_t*)malloc(sizeof(hole_t));
    hole->address = 0;
    hole->memory_size = memsize;
    segment->hole = hole;

    insert(segment, &(memory->segment_list));
    insert(hole, &(memory->free_list));

    return memory;
}

/* First fit algorithm implementation */
void first_fit() {
    computer_t *computer = get_instance();
    cpu_t *cpu = computer->cpu;
    disk_t *disk = computer->disk;
    memory_t *memory = computer->memory;
}

/* Best fit algorithm implementation */
void best_fit() {
    computer_t *computer = get_instance();
    cpu_t *cpu = computer->cpu;
    disk_t *disk = computer->disk;
    memory_t *memory = computer->memory;
}

/* Worst fit algorithm implementation */
void worst_fit() {
    computer_t *computer = get_instance();
    cpu_t *cpu = computer->cpu;
    disk_t *disk = computer->disk;
    memory_t *memory = computer->memory;
}

/* Schedule function */
void schedule() {
    computer_t *computer = get_instance();
    cpu_t *cpu = computer->cpu;
    disk_t *disk = computer->disk;
    memory_t *memory = computer->memory;
}

/* Print a process */
void print_process(FILE *f, void *data) {
    process_t *process = (process_t*) data;
    fprintf(f, "process %p: {time created: %d, process id: %d, memory size: %d, job time: %d}\n",
                process,
                process->time_created,
                process->process_id,
                process->memory_size,
                process->job_time);
}

/* Print a hole */
void print_hole(FILE *f, void *data) {
    hole_t *hole = (hole_t*) data;
    fprintf(f, "hole %p: {address: %d, memory size: %d}\n",
                hole,
                hole->address,
                hole->memory_size);
}

/* Print a segment */
void print_segment(FILE *f, void *data) {
    segment_t *segment = (segment_t*) data;
    if (segment->process != NULL) {
        print_process(f, segment->process);
    } else {
        print_hole(f, segment->hole);
    }
}
