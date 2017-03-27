#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>

#define NUM_ENTRIES 4

struct options {
    char *filename;
    char *algorithm_name;
    int memsize;
    int quantum;
};

struct process {
    int time_created;
    int process_id;
    int memory_size;
    int job_time;
};

typedef struct options options_t;
typedef struct process process_t;

static void usage_exit(char *exe);
static options_t load_options(int argc, char *argv[]);
process_t *load_processes(unsigned int *n);

int main(int argc, char *argv[]) {
    options_t opts = load_options(argc, argv);

    unsigned int num_processes = 0;
    process_t *list = load_processes(&num_processes);
    return 0;
}

static void usage_exit(char *exe) {
    fprintf(stderr, "Usage: %s (-f filename | -a algorithm_name | -m memsize | -q quantum) \n", exe);
    fprintf(stderr, "-f     the name of the process size file\n");
    fprintf(stderr, "-a     the placement algorithm, one of {first,best,worst}\n");
    fprintf(stderr, "-m     the size of main memory\n");
    fprintf(stderr, "-q     the length of the quantum\n");
    exit(EXIT_FAILURE);
}

static options_t load_options(int argc, char *argv[]) {
    extern char *optarg;
    extern int optind;
    int c;
    options_t opts = {
        .filename       = NULL,
        .algorithm_name = NULL,
        .memsize        = -1,
        .quantum        = -1,
    };

    while ((c = getopt(argc, argv, "f:a:m:q:")) != -1) {
        switch (c) {
        case 'f': opts.filename = optarg; break;
        case 'a': opts.algorithm_name = optarg; break;
        case 'm': opts.memsize = atoi(optarg); break;
        case 'q': opts.quantum = atoi(optarg); break;
        default: usage_exit(argv[0]);
        }
    }

    if (opts.filename == NULL || opts.algorithm_name == NULL
            || opts.memsize == -1 || opts.quantum == -1) {
        fprintf(stderr, "All of -f, -a, -m or -q must be specified\n");
        usage_exit(argv[0]);
    }

    return opts;
}

process_t *load_processes(unsigned int *n) {
    process_t *list = (process_t*)malloc(sizeof(process_t));
    int time_created, process_id, memory_size, job_time;

    while (scanf("%d %d %d %d\n", &time_created, &process_id,
            &memory_size, &job_time) == NUM_ENTRIES) {
        (*n)++;
        list = (process_t*)realloc(list, (*n)*sizeof(process_t));
        list[(*n)-1].time_created = time_created;
        list[(*n)-1].process_id = process_id;
        list[(*n)-1].memory_size = memory_size;
        list[(*n)-1].job_time = job_time;
    }

    return list;
}
