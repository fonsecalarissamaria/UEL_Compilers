#include <stdio.h>
#include <stdlib.h>
#include <math.h> 
#include <limits.h>     
#include <errno.h>     
#include <pthread.h> 
#include "gerenciador_pilha.h"

// Declarações do gerenciador de pilha
PILHA* allocate_stack_memory();
void initialize_stack(PILHA* pilha_aux);
PILHA* iniciar_pilha();
ELEMENTO* allocate_new_element(DADO dado, ELEMENTO* previous_node);
void add_element_to_stack(PILHA* pilha_aux, ELEMENTO* no_novo);
void adicionar_na_pilha(PILHA* pilha_aux, DADO dado);
int is_stack_empty(PILHA* pilha_aux);
ELEMENTO* pop_top_element(PILHA* pilha_aux);
void free_node(ELEMENTO* node);
void clear_all_from_stack(PILHA* pilha_aux);
void free_stack_memory(PILHA* pilha_aux);
void destruir_pilha(PILHA* pilha_aux);
DADO process_removed_node(ELEMENTO* node);
void process_and_free_node(ELEMENTO* node);
ELEMENTO* get_next_node(ELEMENTO* node);
void remove_all_nodes_from_stack(PILHA* pilha_aux);
DADO free_and_return_data(ELEMENTO* no_removido);
int is_stack_non_empty(PILHA* pilha_aux);
void adjust_stack_top(PILHA* pilha_aux);
DADO remover_da_pilha(PILHA* pilha_aux);



// Função que aloca memória para a pilha
PILHA* allocate_stack_memory() {
    return malloc(sizeof(PILHA));
}

// Função que inicializa a pilha com valores padrão
void initialize_stack(PILHA* pilha_aux) {
    pilha_aux->quantidade = 0;
    pilha_aux->topo = NULL;
}

// Função principal para iniciar a pilha
PILHA* iniciar_pilha() {
    PILHA* pilha_aux = allocate_stack_memory();  // Aloca memória para a pilha
    initialize_stack(pilha_aux);  // Inicializa a pilha com valores padrão
    return pilha_aux;
}


// Função que aloca memória para um novo elemento da pilha
ELEMENTO* allocate_new_element(DADO dado, ELEMENTO* previous_node) {
    ELEMENTO* no_novo = malloc(sizeof(ELEMENTO));
    no_novo->dado = dado;
    no_novo->anterior = previous_node;
    return no_novo;
}

// Função que adiciona um novo elemento na pilha
void add_element_to_stack(PILHA* pilha_aux, ELEMENTO* no_novo) {
    pilha_aux->topo = no_novo;
    pilha_aux->quantidade++;
}

// Função principal para adicionar um dado na pilha
void adicionar_na_pilha(PILHA* pilha_aux, DADO dado) {
    ELEMENTO* no_novo = allocate_new_element(dado, pilha_aux->topo);  // Aloca e inicializa o novo elemento
    add_element_to_stack(pilha_aux, no_novo);  // Adiciona o novo elemento à pilha
}


// Função que verifica se a pilha está vazia
int is_stack_empty(PILHA* pilha_aux) {
    return pilha_aux->topo == NULL;
}

// Função que remove o topo da pilha e retorna o dado
ELEMENTO* pop_top_element(PILHA* pilha_aux) {
    ELEMENTO* no_removido = pilha_aux->topo;
    pilha_aux->topo = pilha_aux->topo->anterior;
    return no_removido;
}

// Função que trata a remoção de um nó da pilha
DADO process_removed_node(ELEMENTO* node) {
    DADO removed_data = node->dado;
    free(node);  // Libera a memória do nó
    return removed_data;
}

// Função para liberar o nó removido e retornar o dado
DADO free_and_return_data(ELEMENTO* no_removido) {
    DADO dado_removido = no_removido->dado;
    free(no_removido);  // Libera a memória do nó removido
    return dado_removido;
}

// Função para verificar se a pilha não está vazia
int is_stack_non_empty(PILHA* pilha_aux) {
    return pilha_aux->topo != NULL;
}

// Função para ajustar o topo da pilha após remoção
void adjust_stack_top(PILHA* pilha_aux) {
    if (pilha_aux->topo != NULL) {
        pilha_aux->topo = pilha_aux->topo->anterior;  // Move o topo para o próximo nó
    }
}

// Função principal para remover o dado da pilha
DADO remover_da_pilha(PILHA* pilha_aux) {
    if (!is_stack_non_empty(pilha_aux)) {
        return NULL;  // Pilha vazia
    }

    ELEMENTO* no_removido = pilha_aux->topo;  // Guarda o nó do topo
    adjust_stack_top(pilha_aux);  // Atualiza o topo da pilha

    pilha_aux->quantidade--;  // Decrementa a quantidade de elementos na pilha

    return free_and_return_data(no_removido);  // Libera o nó e retorna o dado
}


// Função que libera a memória de um nó
void free_node(ELEMENTO* node) {
    free(node);
}

// Função que processa a remoção de um nó e libera a memória
void process_and_free_node(ELEMENTO* node) {
    free_node(node);  // Libera a memória do nó
}

// Função que obtém o próximo nó a ser removido
ELEMENTO* get_next_node(ELEMENTO* node) {
    return node->anterior;
}

// Função principal para remover todos os nós da pilha
void remove_all_nodes_from_stack(PILHA* pilha_aux) {
    ELEMENTO* current_node = pilha_aux->topo;

    // Processa a remoção de todos os nós da pilha
    while (current_node != NULL) {
        ELEMENTO* next_node = get_next_node(current_node);  // Obtém o próximo nó
        process_and_free_node(current_node);  // Processa e libera o nó
        current_node = next_node;  // Move para o próximo nó
    }

    pilha_aux->topo = NULL;  // Garantir que o topo da pilha é NULL após remoção
    pilha_aux->quantidade = 0;  // Zera a quantidade de elementos na pilha
}


// Função principal para remover todos os elementos da pilha
void clear_all_from_stack(PILHA* pilha_aux) {
    remove_all_nodes_from_stack(pilha_aux);  // Remove todos os nós
    pilha_aux->topo = NULL;  // Define o topo como NULL
    pilha_aux->quantidade = 0;  // Reseta a quantidade de elementos
}


// Função que libera a memória da pilha
void free_stack_memory(PILHA* pilha_aux) {
    free(pilha_aux);  // Libera a memória da pilha
}

// Função principal para destruir a pilha
void destruir_pilha(PILHA* pilha_aux) {
    clear_all_from_stack(pilha_aux);  // Remove todos os elementos da pilha
    free_stack_memory(pilha_aux);  // Libera a memória da pilha
}
