/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * algorithm.c
 *
 */
#include <stdlib.h>
#include <stdbool.h>

#include "algorithm.h"
#include "simulation.h"
#include "computer.h"
#include "list.h"

/* Swap function - First fit algorithm */
void first_fit() {
    computer_t *computer = computer_get_instance();

    if (computer->disk->process_list->head != NULL) {
        process_t *process = disk_get_process();
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

/* Swap function - Best fit algorithm */
void best_fit() {
    computer_t *computer = computer_get_instance();

    if (computer->disk->process_list->head != NULL) {
        process_t *process = disk_get_process();
        bool has_enough_hole = false;
        node_t *segment_node = computer->memory->segment_list->head;
        segment_t *smallest_hole = NULL;
        int smallest_hole_memsize = -1;
        while (segment_node != NULL) {
            segment_t *segment = (segment_t*) segment_node->data;
            if (segment->hole != NULL) {
                if (process->memory_size <= segment->hole->memory_size) {
                    has_enough_hole = true;
                    if (smallest_hole_memsize == -1) {
                        smallest_hole = segment;
                        smallest_hole_memsize = segment->hole->memory_size;
                    }
                    if (segment->hole->memory_size < smallest_hole_memsize) {
                        smallest_hole = segment;
                        smallest_hole_memsize = segment->hole->memory_size;
                    }
                }
            }
            segment_node = segment_node->next;
        }
        if (has_enough_hole) {
            swap_in(process, smallest_hole);
            return;
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

/* Swap function - Worst fit algorithm */
void worst_fit() {
    computer_t *computer = computer_get_instance();

    if (computer->disk->process_list->head != NULL) {
        process_t *process = disk_get_process();
        bool has_enough_hole = false;
        node_t *segment_node = computer->memory->segment_list->head;
        segment_t *largest_hole = NULL;
        int largest_hole_memsize = -1;
        while (segment_node != NULL) {
            segment_t *segment = (segment_t*) segment_node->data;
            if (segment->hole != NULL) {
                if (process->memory_size <= segment->hole->memory_size) {
                    has_enough_hole = true;
                    if (segment->hole->memory_size > largest_hole_memsize) {
                        largest_hole = segment;
                        largest_hole_memsize = segment->hole->memory_size;
                    }
                }
            }
            segment_node = segment_node->next;
        }
        if (has_enough_hole) {
            swap_in(process, largest_hole);
            return;
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

/* Schedule function - Round Robin */
void round_robin(int event) {
    computer_t *computer = computer_get_instance();

    list_t *queue = computer->cpu->process_queue;

    switch (event) {
        case E1:
            computer->cpu->running_time = 0;
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        case E2:
            if (queue->head != NULL && ((process_t*)queue->head->data) == computer->cpu->running_process) {
                process_t *process = list_pop(&queue);
                list_add(process, &queue);
            }
            computer->cpu->running_time = 0;
            computer->cpu->running_process = NULL;
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        case E3:
            if (queue->head != NULL && ((process_t*)queue->head->data) == computer->cpu->running_process) {
                list_pop(&queue);
            }
            computer->cpu->num_completed_process += 1;
            computer->cpu->running_time = 0;
            computer->cpu->running_process = NULL;
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        default: break;
    }
}

/* Swap a process into memory */
void swap_in(process_t *process, segment_t *segment_hole) {
    computer_t *computer = computer_get_instance();

    segment_hole->hole->memory_size -= process->memory_size;

    segment_t *segment_process = (segment_t*)malloc(sizeof(segment_t));
    segment_process->hole = NULL;
    segment_process->process = disk_del_process(process);
    segment_process->process->time_placed_on_disk = -1;
    segment_process->process->time_placed_on_memory = *time();
    list_insert(segment_hole, segment_process, &(computer->memory->segment_list));
    list_add(segment_process->process, &(computer->cpu->process_queue));
    computer->memory->num_processes += 1;

    merge_holes(segment_hole);
    memory_calculate_memusage();

    fprintf(stdout, "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
            *time(),
            segment_process->process->process_id,
            computer->memory->num_processes,
            computer->memory->num_holes,
            computer->memory->memusage);
}

/* Swap a process out of memory into disk */
void swap_out() {
    computer_t *computer = computer_get_instance();

    process_t *process = memory_get_process();
    memory_del_process(process);
    list_del(process, &(computer->cpu->process_queue));
    process->time_placed_on_disk = *time();
    process->time_placed_on_memory = -1;
    list_add(process, &(computer->disk->process_list));
}

/* Merge holes in memory segments */
void merge_holes(segment_t *segment_hole) {
    computer_t *computer = computer_get_instance();

    node_t *segment_node = computer->memory->segment_list->head;

    hole_t *hole = (hole_t*)malloc(sizeof(hole_t));
    hole->memory_size = segment_hole->hole->memory_size;
    if (hole->memory_size == 0) {
        list_del(segment_hole, &(computer->memory->segment_list));
        computer->memory->num_holes -= 1;
        return;
    }

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
                    list_del(prev_segment, &(computer->memory->segment_list));
                    computer->memory->num_holes -= 1;
                }
            }
            if (segment_node->next != NULL) {
                segment_t *next_segment = (segment_t*)segment_node->next->data;
                if (next_segment->hole != NULL) {
                    hole->memory_size += next_segment->hole->memory_size;
                    list_del(next_segment, &(computer->memory->segment_list));
                    computer->memory->num_holes -= 1;
                }
            }
            if (hole->memory_size != segment_hole->hole->memory_size) {
                list_rplc(segment, new_segment_hole, &(computer->memory->segment_list));
            }
            break;
        }
        segment_node = segment_node->next;
    }
}
