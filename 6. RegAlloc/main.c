#include <stdio.h>
#include <stdlib.h>
#include "gerenciador_pilha.h"
#include "gerenciador_grafo.h"
#include "resource_mapper.h"

void read_graph_data(RESOURCE_ASSIGNMENT* allocation);
void start_resource_allocation(RESOURCE_ASSIGNMENT* allocation);
void clean_resources(RESOURCE_ASSIGNMENT* allocation);
void process_input_data(RESOURCE_ASSIGNMENT* allocation);
void process_register_allocation(RESOURCE_ASSIGNMENT* allocation);
void clean_up(RESOURCE_ASSIGNMENT* allocation);

void process_input_data( 
    RESOURCE_ASSIGNMENT* allocation
);

void process_register_allocation( 
    RESOURCE_ASSIGNMENT* allocation 
);

void clean_up(
    RESOURCE_ASSIGNMENT* allocation
);

int main() {
    RESOURCE_ASSIGNMENT* reg_assigner = assign_resources();
    
    // Processando os dados de entrada
    process_input_data(reg_assigner);
    
    // Realizando o processo de alocação
    process_register_allocation(reg_assigner);
    
    // Limpando a alocação
    clean_up(reg_assigner);
    
    return 0;
}

// Função para ler os dados do grafo
void read_graph_data(RESOURCE_ASSIGNMENT* allocation) {
    fscanf(stdin, "Grafo %d:\nK=%d\n", &allocation->map_id, &allocation->available_registers);
}

// Função que inicia a alocação de recursos
void start_resource_allocation(RESOURCE_ASSIGNMENT* allocation) {
    execute_resource_assignment(allocation);
}

// Função para limpar os recursos alocados
void clean_resources(RESOURCE_ASSIGNMENT* allocation) {
    discard_resource_assignment(allocation);
}

// Função principal para processar os dados de entrada
void process_input_data(RESOURCE_ASSIGNMENT* allocation) {
    read_graph_data(allocation);  // Lê os dados do grafo
}

// Função principal para alocar recursos
void process_register_allocation(RESOURCE_ASSIGNMENT* allocation) {
    start_resource_allocation(allocation);  // Inicia a alocação de recursos
}

// Função principal para fazer o cleanup
void clean_up(RESOURCE_ASSIGNMENT* allocation) {
    clean_resources(allocation);  // Limpa os recursos alocados
}

