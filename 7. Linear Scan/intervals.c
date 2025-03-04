#include <stdio.h>
#include <stdlib.h>
#include "intervals.h"

Interval* create_interval(char* name, int start, int end) {
    Interval* new_interval = malloc(sizeof(Interval));
    strcpy(new_interval->name, name);
    new_interval->start = start;
    new_interval->end = end;
    new_interval->reg = -1;
    return new_interval;
}
 
void print_intervals(Interval *intervals, int num_intervals) {
    printf("Variável\tIntervalo\n");
    for (int i = 0; i < num_intervals; i++) {
        printf("%s\t[%d, %d]\n", intervals[i].name, intervals[i].start, intervals[i].end);
    }
}
int compare_intervals(const void *a, const void *b) {
    Interval *intervalA = (Interval *)a;
    Interval *intervalB = (Interval *)b;
    return intervalA->start - intervalB->start;
}