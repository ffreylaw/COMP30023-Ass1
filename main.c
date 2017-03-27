/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * main.c
 *
 */
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>

#include "simulation.h"

/* Struct to store the configuration options from the command line */
typedef struct {
    char *filename;
    char *algorithm_name;
    int memsize;
    int quantum;
} options_t;

/* Function declarations */
static options_t load_options(int argc, char *argv[]);
static void usage_exit(char *exe);
char *string_lower(char *str);
int string_digit(const char *str);

/* The main function */
int main(int argc, char *argv[]) {
    options_t opts = load_options(argc, argv);
    simulate(opts.filename, opts.algorithm_name, opts.memsize, opts.quantum);
    return 0;
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
        case 'a':
            string_lower(optarg);
            if (!strcmp(optarg, "first")) {
                opts.algorithm_name = optarg;
            } else if (!strcmp(optarg, "best")) {
                opts.algorithm_name = optarg;
            } else if (!strcmp(optarg, "worst")) {
                opts.algorithm_name = optarg;
            } else {
                fprintf(stderr, "Unrecognised algorithm name\n");
                usage_exit(argv[0]);
            }
            break;
        case 'm':
            if (string_digit(optarg)) {
                opts.memsize = atoi(optarg);
            } else {
                fprintf(stderr, "Invalid memsize. Please enter integer only\n");
                usage_exit(argv[0]);
            }
            break;
        case 'q':
            if (string_digit(optarg)) {
                opts.quantum = atoi(optarg);
            } else {
                fprintf(stderr, "Invalid quantum. Please enter integer only\n");
                usage_exit(argv[0]);
            }
            break;
        default: usage_exit(argv[0]);
        }
    }

    if (opts.filename == NULL || opts.algorithm_name == NULL || opts.memsize == -1 || opts.quantum == -1) {
        fprintf(stderr, "All of -f, -a, -m or -q must be specified\n");
        usage_exit(argv[0]);
    }

    return opts;
}

static void usage_exit(char *exe) {
    fprintf(stderr, "Usage: %s (-f filename | -a algorithm_name | -m memsize | -q quantum) \n", exe);
    fprintf(stderr, "-f     the name of the process size file\n");
    fprintf(stderr, "-a     the placement algorithm one of {first,best,worst}\n");
    fprintf(stderr, "-m     the size of main memory\n");
    fprintf(stderr, "-q     the length of the quantum\n");
    exit(EXIT_FAILURE);
}

char *string_lower(char *str) {
    unsigned char *p = (unsigned char *)str;
    while (*p) {
        *p = tolower((unsigned char)*p);
        p++;
    }
    return str;
}

int string_digit(const char *str) {
    while (*str) {
        if (!isdigit(*str++)) return 0;
    }
    return 1;
}
