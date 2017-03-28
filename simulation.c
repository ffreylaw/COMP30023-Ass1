/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * simulation.c
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "simulation.h"
#include "computer.h"
#include "list.h"

/* Simulate the memory management task */
void simulate(char *filename, char *algorithm_name, int memsize, int quantum) {
    initialize_computer(algorithm_name, memsize, quantum);

    computer_t *computer = get_instance();

    list_t *all_processes = load_processes(filename);
    int num_processes = len(all_processes);

    // print_list(print_process, stdout, all_processes->head);
    // print_list(print_segment, stdout, memory->segment_list->head);
    // print_list(print_hole, stdout, memory->free_list->head);

    while (computer->cpu->num_completed_process < num_processes) {
        create_process(&all_processes);
        int event = run_cpu();
        if (event) {
            (computer->cpu->swap)();
            (computer->cpu->schedule)(event);
        }
        if (!(all_processes->head)) {
            break;
        }
        (*time())++;
    }

    fprintf(stdout, "time %d, simulation finished.\n", *time());
}

void test_driver() {

}

/* Run the cpu and return an event */
int run_cpu() {
    computer_t *computer = get_instance();

    int event = NONE;

    /* E1 */
    node_t *segment_node = computer->memory->segment_list->head;
    bool empty_flag = true;
    while (segment_node != NULL) {
        segment_t *segment = (segment_t*) segment_node->data;
        if (segment->process != NULL) {
            empty_flag = false;
            break;
        }
        segment_node = segment_node->next;
    }
    if (empty_flag) {
        event = E1;
    }

    /* E2, E3 */
    if (computer->cpu->running_process != NULL) {
        if (computer->cpu->running_time < computer->cpu->quantum
            && computer->cpu->running_process->job_time != 0) {
            computer->cpu->running_time += 1;
            computer->cpu->running_process->job_time -= 1;
        }

        if (computer->cpu->running_process->job_time == 0) {
            event = E3;
        } else if (computer->cpu->running_time == computer->cpu->quantum) {
            event = E2;
        }
    }

    return event;

}

/* Add created process to disk */
void create_process(list_t **all_processes) {
    computer_t *computer = get_instance();
    disk_t *disk = computer->disk;

    if ((*all_processes)->head != NULL) {
        process_t *process = (process_t*) (*all_processes)->head->data;
        if (process->time_created == *time()) {
            process_t *data = pop_head(all_processes);
            insert_at_tail(data, &(disk->process_list));
        }
    }
}

/* Get clock time from the singleton */
int *time() {
    static int *time = NULL;
    if (time == NULL) {
        time = (int*)malloc(sizeof(int));
        *time = 0;
    }
    return time;
}

/* Load processes from given input file */
list_t *load_processes(char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        fprintf(stderr, "Fail to open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    list_t *process_list = NULL;
    int time_created, process_id, memory_size, job_time;

    while (fscanf(f, "%d %d %d %d\n", &time_created, &process_id, &memory_size, &job_time) == NUM_ENTRIES) {
        process_t *process = (process_t*)malloc(sizeof(process_t));
        process->time_created = time_created;
        process->process_id = process_id;
        process->memory_size = memory_size;
        process->job_time = job_time;
        process->time_placed_on_disk = time_created;
        process->time_placed_on_memory = -1;
        insert_at_tail(process, &process_list);
    }

    return process_list;
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
        if (!has_enough_hole) {

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
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        case E2:
            computer->cpu->running_process = NULL;
            process_t *process_e2 = pop_head(&queue);
            insert_at_tail(process_e2, &queue);
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        case E3:
            computer->cpu->running_process = NULL;
            process_t *process_e3 = pop_head(&queue);
            del_memory_process(process_e3);
            if (queue->head != NULL) {
                computer->cpu->running_process = (process_t*) queue->head->data;
            }
            break;
        default: break;
    }

}

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

process_t *del_disk_process(process_t *process) {
    computer_t *computer = get_instance();
    return del(process, &(computer->disk->process_list));
}

process_t *del_memory_process(process_t *process) {
    computer_t *computer = get_instance();


}

void swap_in(process_t *process, segment_t *segment_hole) {
    computer_t *computer = get_instance();

    segment_hole->hole->memory_size -= process->memory_size;

    segment_t *process_segment = (segment_t*)malloc(sizeof(segment_t));
    process_segment->hole = NULL;
    process_segment->process = del_disk_process(process);
    process_segment->process->time_placed_on_disk = -1;
    process_segment->process->time_placed_on_memory = *time();
    insert_before(segment_hole, process_segment, &(computer->memory->segment_list));
    insert_at_tail(process_segment->process, &(computer->cpu->process_queue));

    calculate_memusage();
    computer->memory->num_processes += 1;

    fprintf(stdout, "time %d, %d loaded, numprocesses=%d, numholes=%d, memusage=%d%%\n",
            *time(),
            process_segment->process->process_id,
            computer->memory->num_processes,
            computer->memory->num_holes,
            computer->memory->memusage);
    // printf("segment list: \n");
    // print_list(print_segment, stdout, computer->memory->segment_list->head);
}

void swap_out() {
    computer_t *computer = get_instance();


}
