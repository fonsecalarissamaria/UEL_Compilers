#include <stdio.h>
#include <stddef.h>    
#include <string.h>
#include <complex.h>   
#include <setjmp.h>    
#include <locale.h>    
#include <stdlib.h>
#include "gerenciador_grafo.h"

// Declarações do gerenciador_grafo
NETMAP* create_empty_map();
void initialize_map_points(NETMAP* novo_grafo);
void set_total_nodes(NETMAP* novo_grafo, int totalNodes);
NO* create_node();
void setup_node(NO* novo_no, int indice_no);
void configure_node_pointers(NO* novo_no);
void configure_node_attributes(NO* novo_no, int indice_no);
void handle_allocation_failure();
void insert_edge_if_exists(NO* no_temporario, NO* novo_no);
void insert_no_in_empty_graph(NETMAP* novo_grafo, NO* novo_no);
void insert_no_in_non_empty_graph(NETMAP* novo_grafo, NO* novo_no);
int is_connected_to_target(NO* input, int target_id);
CONEXAO* create_new_edge(int target_id);
void insert_new_edge_in_source(NO* input, CONEXAO* new_edge);
void connect_node_to_target(NO* input, int target_id);
NO* find_no_by_id(NETMAP* novo_grafo, int no_id);
void connect_no_to_target(NETMAP* novo_grafo, int no_id);
void connect_nodes_in_map(NETMAP* novo_grafo, int sourceVertexId, int target_id);
void disconnect_edge_from_node(NO* novo_no, int target_id);
void disconnect_all_edges_from_no(NETMAP* novo_grafo, NO* novo_no);
void remove_node_from_map(NETMAP* novo_grafo, NO* novo_no);
void update_graph_entry(NETMAP* graph, NO* node);
void update_graph_exit(NETMAP* graph, NO* node);
void adjust_neighbor_links(NO* node);
void update_graph_links_after_removal(NETMAP* graph, NO* node);
void check_and_insert_reverse_edge(CONEXAO* connection, NO* source_node, NO* target_node);
void insert_reverse_edge_if_exists(NO* temp_node, NO* new_node);
void update_no_connections(NO* no, CONEXAO* target_connection, CONEXAO* previous_connection);
void remove_edge_from_no(NO* no, CONEXAO* target_connection, CONEXAO* previous_connection);
int is_edge_matching(CONEXAO* connection, int target_id);
CONEXAO* search_for_edge(CONEXAO* start_connection, int target_id, CONEXAO** last_connection);
CONEXAO* find_edge_to_disconnect(NO* node, int target_id, CONEXAO** last_connection);

NETMAP* initialize_map() {
    NETMAP* novo_grafo = create_empty_map();
    initialize_map_points(novo_grafo);
    set_total_nodes(novo_grafo, 0);
    return novo_grafo;
}

NETMAP* create_empty_map() {
    return malloc(sizeof(NETMAP));
}

void initialize_map_points(NETMAP* novo_grafo) {
    novo_grafo->entryPoint = NULL;
    novo_grafo->exitPoint = NULL;
}

void set_total_nodes(NETMAP* novo_grafo, int totalNodes) {
    novo_grafo->totalNodes = totalNodes;
}
       

NO* add_new_node(int indice_no) {
    NO* novo_no = create_node();
    setup_node(novo_no, indice_no);
    return novo_no;
}

NO* create_node() {
    NO* novo_no = malloc(sizeof(NO));
    if (novo_no == NULL) {
        handle_allocation_failure();
    }
    return novo_no;
}

void setup_node(NO* novo_no, int indice_no) {
    configure_node_pointers(novo_no);
    configure_node_attributes(novo_no, indice_no);
}

void configure_node_pointers(NO* novo_no) {
    novo_no->inicio = NULL;
    novo_no->fim = NULL;
}

void configure_node_attributes(NO* novo_no, int indice_no) {
    novo_no->identificador = indice_no;
    novo_no->grau = 0;
    novo_no->cor = -1;
}

void handle_allocation_failure() {
    printf("Memory allocation failed!\n");
    exit(1);  // Termina a execução se falhar alocar memória
}


void insert_edge_if_exists(NO* no_temporario, NO* novo_no) {
    CONEXAO* aux_node;
    for (aux_node = novo_no->inicio; aux_node != NULL; aux_node = aux_node->proximo_no) {
        if (no_temporario->identificador == aux_node->indice_no) {
            connect_node_to_target(no_temporario, novo_no->identificador);
        }
    }
}

// Verifica se há conexão entre os nós e, se houver, cria uma conexão reversa
void check_and_insert_reverse_edge(CONEXAO* connection, NO* source_node, NO* target_node) {
    if (connection->indice_no == target_node->identificador) {
        connect_node_to_target(target_node, source_node->identificador);
    }
}

// Percorre todas as conexões do nó temporário e verifica a necessidade de inserção da conexão reversa
void insert_reverse_edge_if_exists(NO* temp_node, NO* new_node) {
    CONEXAO* current_connection;
    for (current_connection = temp_node->inicio; current_connection != NULL; current_connection = current_connection->proximo_no) {
        check_and_insert_reverse_edge(current_connection, temp_node, new_node);
    }
}


void insert_no_in_empty_graph(NETMAP* novo_grafo, NO* novo_no) {
    novo_no->anterior = NULL;
    novo_grafo->entryPoint = novo_no;
    novo_no->proximo = NULL;
    novo_grafo->exitPoint = novo_no;
    novo_grafo->totalNodes++;
}

void insert_no_in_non_empty_graph(NETMAP* novo_grafo, NO* novo_no) {
    novo_grafo->exitPoint->proximo = novo_no;
    novo_no->anterior = novo_grafo->exitPoint;
    novo_no->proximo = NULL;
    novo_grafo->exitPoint = novo_no;
    novo_grafo->totalNodes++;
}

void insert_node_in_map(NETMAP* novo_grafo, NO* novo_no) {
    NO* no_temporario;
    for (no_temporario = novo_grafo->entryPoint; no_temporario != NULL; no_temporario = no_temporario->proximo) {
        insert_edge_if_exists(no_temporario, novo_no);
        insert_reverse_edge_if_exists(no_temporario, novo_no);
    }

    if (novo_grafo->entryPoint == NULL) {
        insert_no_in_empty_graph(novo_grafo, novo_no);
    } else {
        insert_no_in_non_empty_graph(novo_grafo, novo_no);
    }
}


// Função que percorre a lista de conexões e verifica se há uma correspondência
int search_connection(CONEXAO* connection, int target_id) {
    if (connection == NULL) {
        return 0;  // Lista vazia, não há conexão
    }
    if (connection->indice_no == target_id) {
        return 1;  // Conexão encontrada
    }
    return search_connection(connection->proximo_no, target_id);  // Chamada recursiva
}

// Função principal que verifica se um nó já está conectado a outro
int is_connected_to_target(NO* input, int target_id) {
    return search_connection(input->inicio, target_id);
}


// Função que cria uma nova conexão entre o nó de origem e o nó de destino
CONEXAO* create_new_edge(int target_id) {
    CONEXAO* new_edge = malloc(sizeof(CONEXAO));
    new_edge->indice_no = target_id;
    new_edge->proximo_no = NULL;
    return new_edge;
}

// Função que insere a nova aresta na lista de conexões do nó de origem
void insert_new_edge_in_source(NO* input, CONEXAO* new_edge) {
    if (input->inicio == NULL) {
        new_edge->no_anterior = NULL;
        input->inicio = new_edge; 
    } else {
        input->fim->proximo_no = new_edge;  
        new_edge->no_anterior = input->fim;  
    }
    input->fim = new_edge;
}

// Função principal para conectar o nó de origem ao nó de destino
void connect_node_to_target(NO* input, int target_id) {
    if (is_connected_to_target(input, target_id)) {
        return;  // Já está conectado, nada a fazer
    }

    CONEXAO* new_edge = create_new_edge(target_id);
    insert_new_edge_in_source(input, new_edge);
    input->grau++;  // Aumenta o grau do nó de origem
}


// Função que verifica se o vértice existe no grafo
NO* find_no_by_id(NETMAP* novo_grafo, int no_id) {
    NO* no_temporario;
    for (no_temporario = novo_grafo->entryPoint; no_temporario != NULL; no_temporario = no_temporario->proximo) {
        if (no_temporario->identificador == no_id) {
            return no_temporario;
        }
    }
    return NULL;  // Vértice não encontrado
}

// Função que conecta os nós de origem e destino no grafo
void connect_no_to_target(NETMAP* novo_grafo, int no_id) {
    NO* novo_no = find_no_by_id(novo_grafo, no_id);
    if (novo_no != NULL) {
        connect_node_to_target(novo_no, no_id);
    }
}

// Função principal para conectar os nós no grafo com base nos identificadores
void connect_nodes_in_map(NETMAP* novo_grafo, int sourceVertexId, int target_id) {
    connect_no_to_target(novo_grafo, sourceVertexId);
    connect_no_to_target(novo_grafo, target_id);
}


// Verifica se a aresta corresponde ao identificador do nó de destino
int is_edge_matching(CONEXAO* connection, int target_id) {
    return connection->indice_no == target_id;
}

// Percorre as conexões e encontra a aresta a ser desconectada
CONEXAO* search_for_edge(CONEXAO* start_connection, int target_id, CONEXAO** last_connection) {
    CONEXAO* current_connection = start_connection;
    *last_connection = NULL;

    while (current_connection != NULL) {
        if (is_edge_matching(current_connection, target_id)) {
            return current_connection;  // Aresta encontrada
        }
        *last_connection = current_connection;
        current_connection = current_connection->proximo_no;
    }

    return NULL;  // Aresta não encontrada
}

// Função principal para encontrar a aresta a ser desconectada
CONEXAO* find_edge_to_disconnect(NO* node, int target_id, CONEXAO** last_connection) {
    return search_for_edge(node->inicio, target_id, last_connection);
}


// Atualiza os ponteiros após a remoção da aresta
void update_no_connections(NO* no, CONEXAO* target_connection, CONEXAO* previous_connection) {
    if (previous_connection == NULL) {
        no->inicio = target_connection->proximo_no;
    } else {
        previous_connection->proximo_no = target_connection->proximo_no;
    }

    if (target_connection->proximo_no == NULL) {
        no->fim = previous_connection;
    } else {
        target_connection->proximo_no->no_anterior = previous_connection;
    }
}

// Função principal que desconecta a aresta do nó de origem
void remove_edge_from_no(NO* no, CONEXAO* target_connection, CONEXAO* previous_connection) {
    update_no_connections(no, target_connection, previous_connection);
    no->grau--;  // Decrementa o grau do vértice
}


// Função principal para desconectar a aresta do nó de origem
void disconnect_edge_from_node(NO* novo_no, int target_id) {
    CONEXAO* last_connection = NULL;
    CONEXAO* aux_node = find_edge_to_disconnect(novo_no, target_id, &last_connection);

    if (aux_node != NULL) {
        remove_edge_from_no(novo_no, aux_node, last_connection);
        free(aux_node);  // Libera a memória da aresta desconectada
    }
}


// Função que desconecta todas as arestas do nó a ser removido
void disconnect_all_edges_from_no(NETMAP* novo_grafo, NO* novo_no) {
    NO* no_temporario;
    for (no_temporario = novo_grafo->entryPoint; no_temporario != NULL; no_temporario = no_temporario->proximo) {
        if (no_temporario != novo_no) {
            disconnect_edge_from_node(no_temporario, novo_no->identificador);
        }
    }
}

// Atualiza a entrada do grafo se necessário
void update_graph_entry(NETMAP* graph, NO* node) {
    if (graph->entryPoint == node) {
        graph->entryPoint = node->proximo;
    }
}

// Atualiza a saída do grafo se necessário
void update_graph_exit(NETMAP* graph, NO* node) {
    if (graph->exitPoint == node) {
        graph->exitPoint = node->anterior;
    }
}

// Ajusta os ponteiros do nó anterior e do próximo nó
void adjust_neighbor_links(NO* node) {
    if (node->anterior != NULL) {
        node->anterior->proximo = node->proximo;
    }
    if (node->proximo != NULL) {
        node->proximo->anterior = node->anterior;
    }
}

// Função principal que remove o nó do grafo e ajusta os ponteiros
void update_graph_links_after_removal(NETMAP* graph, NO* node) {
    update_graph_entry(graph, node);
    update_graph_exit(graph, node);
    adjust_neighbor_links(node);
}


// Função principal para remover um nó do grafo
void remove_node_from_map(NETMAP* novo_grafo, NO* novo_no) {
    disconnect_all_edges_from_no(novo_grafo, novo_no);  // Desconecta todas as arestas do nó

    update_graph_links_after_removal(novo_grafo, novo_no);  // Atualiza os links do grafo após remoção

    novo_grafo->totalNodes--;  // Decrementa o número de nós no grafo
}