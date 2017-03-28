/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * computer.c
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "computer.h"
#include "simulation.h"
#include "list.h"

/* Get instance from the singleton */
computer_t* get_instance() {
    static computer_t *instance = NULL;
    if (instance == NULL) {
        instance = (computer_t*)malloc(sizeof(computer_t));
        instance->cpu = NULL;
        instance->disk = NULL;
        instance->memory = NULL;
    }
    return instance;
}

/* Initialize the computer */
void initialize_computer(char *algorithm_name, int memsize, int quantum) {
    computer_t *computer = get_instance();
    computer->cpu = (cpu_t*)malloc(sizeof(cpu_t));
    computer->disk = initialize_disk();
    computer->memory = initialize_memory(memsize);
    if (!strcmp(algorithm_name, "first")) {
        computer->cpu->swap = first_fit;
    } else if (!strcmp(algorithm_name, "best")) {
        computer->cpu->swap = best_fit;
    } else if (!strcmp(algorithm_name, "worst")) {
        computer->cpu->swap = worst_fit;
    }
    computer->cpu->schedule = round_robin;
    computer->cpu->quantum = quantum;
    computer->cpu->process_queue = NULL;
    computer->cpu->running_process = NULL;
    computer->cpu->running_time = 0;
    computer->cpu->num_completed_process = 0;
}

/* Initialize a disk */
disk_t *initialize_disk() {
    disk_t *disk = (disk_t*)malloc(sizeof(disk_t));
    disk->process_list = NULL;
    return disk;
}

/* Initialize a memory with given memsize */
memory_t *initialize_memory(int memsize) {
    memory_t *memory = (memory_t*)malloc(sizeof(memory_t));
    memory->memsize = memsize;
    memory->memusage = 0;
    memory->num_processes = 0;
    memory->num_holes = 1;
    memory->segment_list = NULL;
    memory->free_list = NULL;

    segment_t *segment = (segment_t*)malloc(sizeof(segment_t));
    segment->process = NULL;
    hole_t *hole = (hole_t*)malloc(sizeof(hole_t));
    hole->memory_size = memsize;
    segment->hole = hole;

    insert_at_tail(segment, &(memory->segment_list));
    insert_at_tail(hole, &(memory->free_list));

    return memory;
}

/* Calculate memory usage */
void calculate_memusage() {
    computer_t *computer = get_instance();
    memory_t *memory = computer->memory;
    node_t *segment_node = memory->segment_list->head;

    int total_process_memsize = 0;

    while (segment_node != NULL) {
        segment_t *segment = (segment_t*) segment_node->data;
        if (segment->process != NULL) {
            total_process_memsize += segment->process->memory_size;
        }
        segment_node = segment_node->next;
    }

    memory->memusage = (int)ceil(100.0*total_process_memsize/memory->memsize);
}

/* Print a process */
void print_process(FILE *f, void *data) {
    process_t *process = (process_t*) data;
    fprintf(f, "process %p: {time created: %d, process id: %d, memory size: %d, job time: %d, time placed on disk: %d, time placed on memory: %d}\n",
                process,
                process->time_created,
                process->process_id,
                process->memory_size,
                process->job_time,
                process->time_placed_on_disk,
                process->time_placed_on_memory);
}

/* Print a hole */
void print_hole(FILE *f, void *data) {
    hole_t *hole = (hole_t*) data;
    fprintf(f, "hole %p: {memory size: %d}\n",
                hole,
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
