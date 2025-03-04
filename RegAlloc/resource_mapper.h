#ifndef _RESOURCE_MAPPER_H_
#define _RESOURCE_MAPPER_H_

#include "gerenciador_grafo.h"
#include "gerenciador_pilha.h"

typedef struct resource_assignment RESOURCE_ASSIGNMENT;

RESOURCE_ASSIGNMENT* assign_resources();

void discard_resource_assignment(
    RESOURCE_ASSIGNMENT* resource_assignment
);

void execute_resource_assignment(
    RESOURCE_ASSIGNMENT* resource_assignment
);

NETMAP* generate_map();

struct resource_assignment {
    int map_id;
    int available_registers;
    NETMAP* network_map;
};

PILHA* refine_map(
    NETMAP* map, 
    int k
);

char* select_map(
    NETMAP* map, 
    PILHA* stack, 
    int k
);

#endif