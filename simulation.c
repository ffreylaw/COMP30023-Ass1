#include <stdlib.h>
#include <stdio.h>

#include "simulation.h"

void simulate(char *filename, char *algorithm_name, int memsize, int quantum) {
    disk_singleton_t *disk_instance = get_disk_instance();
    disk_t *disk = disk_instance->disk;

    memory_singleton_t *memory_instance = get_memory_instance();
    memory_t *memory = memory_instance->memory;
    memory->memsize = memsize;

}

disk_singleton_t* get_disk_instance() {
    static disk_singleton_t *instance = NULL;

    if (instance == NULL) {
        instance = (disk_singleton_t*)malloc(sizeof(disk_singleton_t));
        instance->disk = load_processes();
    }

    return instance;
}

memory_singleton_t* get_memory_instance() {
    static memory_singleton_t *instance = NULL;

    if (instance == NULL) {
        instance = (memory_singleton_t*)malloc(sizeof(memory_singleton_t));
        instance->memory = (memory_t*)malloc(sizeof(memory_t));
        instance->memory->memsize = 0;
    }

    return instance;
}

disk_t *load_processes() {
    process_t *list = (process_t*)malloc(sizeof(process_t));
    int time_created, process_id, memory_size, job_time;
    int n = 0;

    while (scanf("%d %d %d %d\n", &time_created, &process_id, &memory_size, &job_time) == NUM_ENTRIES) {
        n++;
        list = (process_t*)realloc(list, n*sizeof(process_t));
        list[n-1].time_created = time_created;
        list[n-1].process_id = process_id;
        list[n-1].memory_size = memory_size;
        list[n-1].job_time = job_time;
    }

    disk_t *disk = (disk_t*)malloc(sizeof(disk_t));
    disk->list = list;
    disk->num_processes = n;
    return disk;
}
