#ifndef LINEAR_SCAN_ALLOC_H
#define LINEAR_SCAN_ALLOC_H

#include "intervals.h"

#define MAX_VARIABLES 100

typedef struct {
    Interval intervals[MAX_VARIABLES];
    int num_intervals;
    int registradores_fisicos; 
    int current_K;          
    int spill_aux; 
} LINEAR_SCAN_ALLOCATOR;

LINEAR_SCAN_ALLOCATOR* create_linear_scan_allocation(

);

void linear_scan_register_allocation(
    LINEAR_SCAN_ALLOCATOR *reg_alloc, 
    int **spill_array
);

int linear_scan(
    Interval *intervals, 
    int num_intervals, 
    int K, 
    int *spill_vars, 
    int *spill_iterations, 
    int *spill_count, 
    int **spill_array, 
    int index_novo
);

void deallocate_registers(
    LINEAR_SCAN_ALLOCATOR* reg_alloc
);

#endif 
