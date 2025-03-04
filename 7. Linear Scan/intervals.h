#ifndef INTERVALS_H
#define INTERVALS_H

#include <string.h>
typedef struct {
    char name[10]; 
    int start;
    int end;
    int reg;
    int spill;
} Interval;

int compare_intervals(
    const void *a, 
    const void *b
);

#endif 
