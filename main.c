#include <stdlib.h>
#include <stdio.h>

#define NUM_ENTRIES 4

struct process {
    int time_created;
    int process_id;
    int memory_size;
    int job_time;
};

typedef struct process process_t;

void get_input(process_t **list, unsigned int *n);

int main(int argc, char const *argv[]) {
    process_t *list = NULL;
    unsigned int num_processes = 0;
    get_input(&list, &num_processes);
    return 0;
}

void get_input(process_t **list, unsigned int *n) {
    *list = (process_t*)malloc(sizeof(process_t));
    int time_created, process_id, memory_size, job_time;
    while (scanf("%d %d %d %d\n", &time_created, &process_id,
            &memory_size, &job_time) == NUM_ENTRIES) {
        (*n)++;
        *list = (process_t*)realloc(*list, (*n)*sizeof(process_t));
        (*list)[(*n)-1].time_created = time_created;
        (*list)[(*n)-1].process_id = process_id;
        (*list)[(*n)-1].memory_size = memory_size;
        (*list)[(*n)-1].job_time = job_time;
    }
}
