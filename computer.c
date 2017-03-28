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

    node_t *segment_node = computer->memory->segment_list->head;

    int total_process_memsize = 0;

    while (segment_node != NULL) {
        segment_t *segment = (segment_t*) segment_node->data;
        if (segment->process != NULL) {
            total_process_memsize += segment->process->memory_size;
        }
        segment_node = segment_node->next;
    }

    computer->memory->memusage = (int)ceil(100.0*total_process_memsize/computer->memory->memsize);
}

/* First fit algorithm implementation */
void first_fit() {
    computer_t *computer = get_instance();

    if (computer->disk->process_list->head != NULL) {
        process_t *process = get_disk_process();
        node_t *segment_node = computer->memory->segment_list->head;
        bool has_enough_hole = false;
        while (segment_node != NULL) {
            segment_t *segment = (segment_t*) segment_node->data;
            if (segment->hole != NULL) {
                if (process->memory_size <= segment->hole->memory_size) {
                    has_enough_hole = true;
                    swap_in(process, segment);
                    break;
                }
            }
            segment_node = segment_node->next;
        }
        while (!has_enough_hole) {
            swap_out();
            segment_node = computer->memory->segment_list->head;
            while (segment_node != NULL) {
                segment_t *segment = (segment_t*) segment_node->data;
                if (segment->hole != NULL) {
                    if (process->memory_size <= segment->hole->memory_size) {
                        has_enough_hole = true;
                        swap_in(process, segment);
                        break;
                    }
                }
                segment_node = segment_node->next;
            }
        }
    } else {
        return;
    }
}

/* Best fit algorithm implementation */
void best_fit() {
    computer_t *computer = get_instance();


}

/* Worst fit algorithm implementation */
void worst_fit() {
    computer_t *computer = get_instance();


}

/* Schedule function */
void round_robin(int event) {
    computer_t *computer = get_instance();

    list_t *queue = computer->cpu->process_queue;

    switch (event) {
        case E1:
            computer->cpu->running_time = 0;
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        case E2:
            computer->cpu->running_time = 0;
            computer->cpu->running_process = NULL;
            process_t *process = pop_head(&queue);
            insert_at_tail(process, &queue);
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        case E3:
            computer->cpu->running_time = 0;
            computer->cpu->num_completed_process += 1;
            computer->cpu->running_process = NULL;
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        default: break;
    }
}

/* Get a process from disk */
process_t *get_disk_process() {
    computer_t *computer = get_instance();

    node_t *process_node = computer->disk->process_list->head;

    process_t *process = (process_t*) process_node->data;
    int longest_time = *time() - process->time_placed_on_disk;
    int highest_priority = process->process_id;
    process_node = process_node->next;

    while (process_node != NULL) {
        process_t *data = (process_t*) process_node->data;
        if ((*time() - data->time_placed_on_disk) > longest_time) {
            process = data;
            longest_time = *time() - data->time_placed_on_disk;
            highest_priority = data->process_id;
        } else if ((*time() - data->time_placed_on_disk) == longest_time) {
            if (data->process_id < highest_priority) {
                process = data;
                longest_time = *time() - data->time_placed_on_disk;
                highest_priority = data->process_id;
            }
        }
        process_node = process_node->next;
    }

    return process;
}

/* Get a process from memory segments */
process_t *get_memory_process() {
    computer_t *computer = get_instance();

    node_t *segment_node = computer->memory->segment_list->head;

    process_t *process = NULL;
    int longest_time = -1;

    while (segment_node != NULL) {
        if (((segment_t*)segment_node->data)->process != NULL) {
            process_t *data = ((segment_t*)segment_node->data)->process;
            if ((*time() - data->time_placed_on_memory) > longest_time) {
                process = data;
                longest_time = *time() - data->time_placed_on_memory;
            }
        }
        segment_node = segment_node->next;
    }

    return process;
}

/* Delete a process from disk */
process_t *del_disk_process(process_t *process) {
    computer_t *computer = get_instance();
    return del(process, &(computer->disk->process_list));
}

/* Delete a process from memory segments */
process_t *del_memory_process(process_t *process) {
    computer_t *computer = get_instance();

    hole_t *hole = (hole_t*)malloc(sizeof(hole_t));
    hole->memory_size = process->memory_size;
    segment_t *segment_hole = (segment_t*)malloc(sizeof(segment_t));
    segment_hole->process = NULL;
    segment_hole->hole = hole;

    segment_t *segment_process = NULL;
    node_t *segment_node = computer->memory->segment_list->head;
    while (segment_node != NULL) {
        if (((segment_t*)segment_node->data)->process != NULL) {
            if (((segment_t*)segment_node->data)->process->process_id == process->process_id) {
                segment_process = (segment_t*)segment_node->data;
                break;
            }
        }
        segment_node = segment_node->next;
    }

    computer->memory->num_processes -= 1;
    computer->memory->num_holes += 1;
    replace(segment_process, segment_hole, &(computer->memory->segment_list));
    merge_holes(segment_hole);
    return process;
}

/* Merge holes in memory segments */
void merge_holes(segment_t *segment_hole) {
    computer_t *computer = get_instance();

    node_t *segment_node = computer->memory->segment_list->head;

    hole_t *hole = (hole_t*)malloc(sizeof(hole_t));
    hole->memory_size = segment_hole->hole->memory_size;
    segment_t *new_segment_hole = (segment_t*)malloc(sizeof(segment_t));
    new_segment_hole->process = NULL;
    new_segment_hole->hole = hole;

    while (segment_node != NULL) {
        segment_t *segment = (segment_t*)segment_node->data;
        if (segment == segment_hole) {
            if (segment_node->prev != NULL) {
                segment_t *prev_segment = (segment_t*)segment_node->prev->data;
                if (prev_segment->hole != NULL) {
                    hole->memory_size += prev_segment->hole->memory_size;
                    del(prev_segment, &(computer->memory->segment_list));
                    computer->memory->num_holes -= 1;
                }
            }
            if (segment_node->next != NULL) {
                segment_t *next_segment = (segment_t*)segment_node->next->data;
                if (next_segment->hole != NULL) {
                    hole->memory_size += next_segment->hole->memory_size;
                    del(next_segment, &(computer->memory->segment_list));
                    computer->memory->num_holes -= 1;
                }
            }
            if (hole->memory_size != segment_hole->hole->memory_size) {
                replace(segment, new_segment_hole, &(computer->memory->segment_list));
            }
            break;
        }
        segment_node = segment_node->next;
    }
}

/* Swap a process into memory */
void swap_in(process_t *process, segment_t *segment_hole) {
    computer_t *computer = get_instance();

    segment_hole->hole->memory_size -= process->memory_size;

    segment_t *segment_process = (segment_t*)malloc(sizeof(segment_t));
    segment_process->hole = NULL;
    segment_process->process = del_disk_process(process);
    segment_process->process->time_placed_on_disk = -1;
    segment_process->process->time_placed_on_memory = *time();
    insert_before(segment_hole, segment_process, &(computer->memory->segment_list));
    insert_at_tail(segment_process->process, &(computer->cpu->process_queue));

    calculate_memusage();
    computer->memory->num_processes += 1;

    fprintf(stdout, "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
            *time(),
            segment_process->process->process_id,
            computer->memory->num_processes,
            computer->memory->num_holes,
            computer->memory->memusage);
}

/* Swap a process out of memory into disk */
void swap_out() {
    computer_t *computer = get_instance();

    process_t *process = get_memory_process();
    del_memory_process(process);
    del(process, &(computer->cpu->process_queue));
    process->time_placed_on_disk = *time();
    process->time_placed_on_memory = -1;
    insert_at_tail(process, &(computer->disk->process_list));
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
