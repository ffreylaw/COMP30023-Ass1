/*
 * COMP30024 Computer Systems
 * Semester 1 2017
 * Assignment 1
 *
 * Geoffrey Law (glaw@student.unimelb.edu.au)
 * 759218
 *
 * algorithm.h
 *
 */

 #include "computer.h"

 /* Swap function - First fit algorithm */
 void first_fit();

 /* Swap function - Best fit algorithm */
 void best_fit();

 /* Swap function - Worst fit algorithm */
 void worst_fit();

 /* Schedule function - Round Robin */
 void round_robin(int);

 /* Swap a process into memory */
 void swap_in(process_t*, segment_t*);

 /* Swap a process out of memory into disk */
 void swap_out();

 /* Merge holes in memory segments */
 void merge_holes(segment_t*);
