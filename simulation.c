#include <stdlib.h>
#include <stdio.h>

#include "simulation.h"
#include "list.h"

/* Simulate the memory management task */
void simulate(char *filename, char *algorithm_name, int memsize, int quantum) {
    cpu_t *cpu = get_instance();
    disk_t *disk = cpu->disk;
    memory_t *memory = cpu->memory;
    memory->memsize = memsize;
    //print_list(print_process, stdout, disk->process_list);
}

/* Get the instance from the singleton structure */
cpu_t* get_instance() {
    static cpu_t *instance = NULL;

    if (instance == NULL) {
        instance = (cpu_t*)malloc(sizeof(cpu_t));
        instance->disk = load_processes();
        instance->memory = (memory_t*)malloc(sizeof(memory_t));
        instance->memory->memsize = 0;
    }

    return instance;
}

/* Load processes from standard input */
disk_t *load_processes() {
    list_t list = NULL;
    int time_created, process_id, memory_size, job_time;

    while (scanf("%d %d %d %d\n", &time_created, &process_id, &memory_size, &job_time) == NUM_ENTRIES) {
        process_t *data = (process_t*)malloc(sizeof(process_t));
        data->time_created = time_created;
        data->process_id = process_id;
        data->memory_size = memory_size;
        data->job_time = job_time;
        insert(data, &list);
    }

    disk_t *disk = (disk_t*)malloc(sizeof(disk_t));
    disk->process_list = list;
    return disk;
}

/* Print a process */
void print_process(FILE *f, void *data) {
    process_t *process = (process_t*) data;
    fprintf(f, "{time created: %d, process id: %d, memory size: %d, job time: %d}\n",
                process->time_created,
                process->process_id,
                process->memory_size,
                process->job_time);
}
