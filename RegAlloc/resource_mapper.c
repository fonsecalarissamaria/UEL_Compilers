#include <stdio.h>
#include <stdlib.h>
#include <assert.h>    
#include <errno.h>    
#include <signal.h>    
#include <unistd.h>
#include <dirent.h>    
#include <stdbool.h>
#include <string.h> 
#include "resource_mapper.h"

// Declarações do resource_mapper
RESOURCE_ASSIGNMENT* assign_resources();
RESOURCE_ASSIGNMENT* alocar_registro_assignment();
void inicializar_map_id_e_registros(RESOURCE_ASSIGNMENT* assigner);
void configurar_network_map(RESOURCE_ASSIGNMENT* assigner);
void discard_resource_assignment(RESOURCE_ASSIGNMENT* reg_assigner);
void desalocar_assigner(RESOURCE_ASSIGNMENT* reg_assigner);
void limpar_referencias(RESOURCE_ASSIGNMENT* reg_assigner);
void execute_resource_assignment(RESOURCE_ASSIGNMENT* reg_assigner);
void exibir_informacoes_iniciais(RESOURCE_ASSIGNMENT* reg_assigner);
NETMAP* generate_map();
bool ler_vertex(int* vertex_id);
PILHA* refine_map(NETMAP* novo_grafo, int j);
void processar_vertice(NETMAP* novo_grafo, PILHA* pilha, NO* vertex, int j);
int* initialize_colors(int j);
void mark_used_colors(NETMAP* novo_grafo, NO* temp_vertex, int* colors, int j);
int assign_color(NO* temp_vertex, int* colors, int j);
char* handle_spill(NO* temp_vertex);
void processar_k_values_e_gerar_resultados(RESOURCE_ASSIGNMENT* reg_assigner, char* outcome[]);
void processar_k_atual(RESOURCE_ASSIGNMENT* reg_assigner, char* outcome[], int k);
void imprimir_inicio_processo(int k);
PILHA* criar_pilha_processamento(NETMAP* network_map, int k);
char* executar_selecao_mapa(NETMAP* network_map, PILHA* pilha, int k);
void destruir_pilha_e_finalizar(PILHA* pilha);
void processar_arestas(NO* vertex);
char obter_proximo_caractere();
int eh_espaco(char caractere);
int ler_identificador_alvo();
void conectar_vertice(NO* vertex, int alvo);
void exibir_resultados_finais(RESOURCE_ASSIGNMENT* reg_assigner, char* outcome[]);
void imprimir_divisor();
void imprimir_resultado_mapa(int map_id, int k, char* resultado);
NO* escolher_vertice(NETMAP* novo_grafo, int j);
NO* encontrar_vertice_com_menos_arestas(NETMAP* grafo);
NO* encontrar_vertice_com_mais_arestas(NETMAP* grafo);
NO* pop_vertex_from_stack(PILHA* pilha);
void add_vertex_to_map(NETMAP* novo_grafo, NO* vertex);
int* prepare_colors(int j);
void mark_taken_colors(NETMAP* novo_grafo, NO* vertex, int* colors, int j);
int try_assign_color(NO* vertex, int* colors, int j);
char* process_spill(NO* vertex);
void log_color_assignment(NO* vertex);
void release_colors(int* colors);
char* create_success_message();


void print_divider() {
    printf("----------------------------------------\n");
}

void imprimir_divisor() {
    printf("----------------------------------------");
}

RESOURCE_ASSIGNMENT* assign_resources() {
    // Alocar memória para o RESOURCE_ASSIGNMENT
    RESOURCE_ASSIGNMENT* reg_assigner = alocar_registro_assignment();

    // Inicializar os valores padrão
    inicializar_map_id_e_registros(reg_assigner);

    // Inicializar o mapa de rede como NULL
    configurar_network_map(reg_assigner);

    return reg_assigner;
}

// Função para alocar memória para RESOURCE_ASSIGNMENT
RESOURCE_ASSIGNMENT* alocar_registro_assignment() {
    return (RESOURCE_ASSIGNMENT*) malloc(sizeof(RESOURCE_ASSIGNMENT));
}

// Função para inicializar map_id e available_registers com valores padrão
void inicializar_map_id_e_registros(RESOURCE_ASSIGNMENT* assigner) {
    assigner->map_id = -1;
    assigner->available_registers = -1;
}

// Função para configurar o campo network_map como NULL
void configurar_network_map(RESOURCE_ASSIGNMENT* assigner) {
    assigner->network_map = NULL;
}


void discard_resource_assignment(RESOURCE_ASSIGNMENT* reg_assigner) {
    if (reg_assigner != NULL) {
        desalocar_assigner(reg_assigner);
    }
}

// Função para verificar e desalocar a memória do RESOURCE_ASSIGNMENT
void desalocar_assigner(RESOURCE_ASSIGNMENT* reg_assigner) {
    // Realiza outras limpezas necessárias, se houver, antes de liberar a memória
    limpar_referencias(reg_assigner);
    free(reg_assigner);
}

// Função para limpar referências internas do RESOURCE_ASSIGNMENT, se necessário
void limpar_referencias(RESOURCE_ASSIGNMENT* reg_assigner) {
    reg_assigner->network_map = NULL;  
}


void execute_resource_assignment(RESOURCE_ASSIGNMENT* reg_assigner){
    // Inicializando o mapa de rede
    reg_assigner->network_map = generate_map();

    // Exibindo as informações iniciais sobre o grafo
    exibir_informacoes_iniciais(reg_assigner);

    // Processando os diferentes valores de K e obtendo os resultados
    char* outcome[reg_assigner->available_registers - 1];
    processar_k_values_e_gerar_resultados(reg_assigner, outcome);

    // Exibindo os resultados finais
    exibir_resultados_finais(reg_assigner, outcome);
}

// Função para exibir as informações iniciais do grafo
void exibir_informacoes_iniciais(RESOURCE_ASSIGNMENT* reg_assigner) {
    printf("Graph %d -> Physical Registers: %d\n", reg_assigner->map_id, reg_assigner->available_registers);
    print_divider();
    print_divider();
}

void processar_k_values_e_gerar_resultados(RESOURCE_ASSIGNMENT* reg_assigner, char* outcome[]) {
    int k = reg_assigner->available_registers;

    while (k > 1) {
        processar_k_atual(reg_assigner, outcome, k);
        k--;
    }
}

void processar_k_atual(RESOURCE_ASSIGNMENT* reg_assigner, char* outcome[], int k) {
    imprimir_inicio_processo(k);

    PILHA* pilha = criar_pilha_processamento(reg_assigner->network_map, k);
    outcome[k - 1] = executar_selecao_mapa(reg_assigner->network_map, pilha, k);

    destruir_pilha_e_finalizar(pilha);
}

void imprimir_inicio_processo(int k) {
    printf("K = %d\n\n", k);
}

PILHA* criar_pilha_processamento(NETMAP* network_map, int k) {
    return refine_map(network_map, k);
}

char* executar_selecao_mapa(NETMAP* network_map, PILHA* pilha, int k) {
    return select_map(network_map, pilha, k);
}

void destruir_pilha_e_finalizar(PILHA* pilha) {
    destruir_pilha(pilha);
    print_divider();
}


void exibir_resultados_finais(RESOURCE_ASSIGNMENT* reg_assigner, char* outcome[]) {
    imprimir_divisor();
    int i;
    for (i = reg_assigner->available_registers; i > 1; i--) {
        imprimir_resultado_mapa(reg_assigner->map_id, i, outcome[i - 1]);
    }
}

void imprimir_resultado_mapa(int map_id, int k, char* resultado) {
    printf("\nGraph %d -> K = %2d: %s", map_id, k, resultado);
}

NETMAP* generate_map() {
    NETMAP* novo_grafo = initialize_map();
    
    while(!feof(stdin)){
        int vertex_id;
        if (!ler_vertex(&vertex_id)) break;

        NO* vertex = add_new_node(vertex_id);
        processar_arestas(vertex);
        insert_node_in_map(novo_grafo, vertex);
    }

    return novo_grafo;
}

// Função para ler um vertex a partir da entrada padrão
bool ler_vertex(int* vertex_id) {
    return fscanf(stdin, "%d -->", vertex_id) == 1;
}

void processar_arestas(NO* vertex) {
    char caractere_aux = obter_proximo_caractere();

    while (eh_espaco(caractere_aux)) {
        int aux = ler_identificador_alvo();
        conectar_vertice(vertex, aux);
        caractere_aux = obter_proximo_caractere();
    }
}

char obter_proximo_caractere() {
    return fgetc(stdin);
}

int eh_espaco(char caractere) {
    return caractere == ' ';
}

int ler_identificador_alvo() {
    int aux;
    fscanf(stdin, "%d", &aux);
    return aux;
}

void conectar_vertice(NO* vertex, int alvo) {
    connect_node_to_target(vertex, alvo);
}


PILHA* refine_map(NETMAP* novo_grafo, int j) {
    PILHA* pilha = iniciar_pilha();
    int total_nodes = novo_grafo->totalNodes;
    int i;
    for (i = 0; i < total_nodes; i++) {
        NO* vertex_to_process = escolher_vertice(novo_grafo, j);
        processar_vertice(novo_grafo, pilha, vertex_to_process, j);
    }

    return pilha;
}

NO* escolher_vertice(NETMAP* novo_grafo, int j) {
    NO* min_vertex = encontrar_vertice_com_menos_arestas(novo_grafo);
    NO* max_vertex = encontrar_vertice_com_mais_arestas(novo_grafo);

    return (min_vertex->grau < j) ? min_vertex : max_vertex;
}

NO* encontrar_vertice_com_menos_arestas(NETMAP* grafo) {
    NO* min_vertex = NULL;
    int min_edges = 0;
    NO* current_vertex;
    for (current_vertex = grafo->entryPoint; current_vertex != NULL; current_vertex = current_vertex->proximo) {
        if (min_vertex == NULL || current_vertex->grau < min_edges ||
            (current_vertex->grau == min_edges && current_vertex->identificador < min_vertex->identificador)) {
            min_vertex = current_vertex;
            min_edges = current_vertex->grau;
        }
    }

    return min_vertex;
}

NO* encontrar_vertice_com_mais_arestas(NETMAP* grafo) {
    NO* max_vertex = NULL;
    int max_edges = 0;
    NO* current_vertex; 

    for (current_vertex = grafo->entryPoint; current_vertex != NULL; current_vertex = current_vertex->proximo) {
        if (max_vertex == NULL || current_vertex->grau > max_edges ||
            (current_vertex->grau == max_edges && current_vertex->identificador < max_vertex->identificador)) {
            max_vertex = current_vertex;
            max_edges = current_vertex->grau;
        }
    }

    return max_vertex;
}


// Função para processar o vértice escolhido
void processar_vertice(NETMAP* novo_grafo, PILHA* pilha, NO* vertex, int j) {
    if (vertex->grau < j) {
        vertex->cor = -1;
        adicionar_na_pilha(pilha, vertex);
        remove_node_from_map(novo_grafo, vertex);
        printf("Push: %d\n", vertex->identificador);
    } else {
        vertex->cor = -1;
        adicionar_na_pilha(pilha, vertex);
        remove_node_from_map(novo_grafo, vertex);
        printf("Push: %d *\n", vertex->identificador);
    }
}


char* select_map(NETMAP* novo_grafo, PILHA* pilha, int j) {
    char* resultado_final;
    int allocation_successful = 1;

    while (pilha->quantidade > 0) {
        NO* current_vertex = pop_vertex_from_stack(pilha);
        add_vertex_to_map(novo_grafo, current_vertex);

        if (!allocation_successful) {
            continue;
        }

        int* available_colors = prepare_colors(j);
        mark_taken_colors(novo_grafo, current_vertex, available_colors, j);

        if (!try_assign_color(current_vertex, available_colors, j)) {
            allocation_successful = 0;
            resultado_final = process_spill(current_vertex);
        } else {
            log_color_assignment(current_vertex);
        }

        release_colors(available_colors);
    }

    if (allocation_successful) {
        resultado_final = create_success_message();
    }

    return resultado_final;
}

NO* pop_vertex_from_stack(PILHA* pilha) {
    return remover_da_pilha(pilha);
}

void add_vertex_to_map(NETMAP* novo_grafo, NO* vertex) {
    insert_node_in_map(novo_grafo, vertex);
}

int* prepare_colors(int j) {
    return initialize_colors(j);
}

void mark_taken_colors(NETMAP* novo_grafo, NO* vertex, int* colors, int j) {
    mark_used_colors(novo_grafo, vertex, colors, j);
}

int try_assign_color(NO* vertex, int* colors, int j) {
    return assign_color(vertex, colors, j);
}

char* process_spill(NO* vertex) {
    return handle_spill(vertex);
}

void log_color_assignment(NO* vertex) {
    printf("Pop: %d -> %d\n", vertex->identificador, vertex->cor);
}

void release_colors(int* colors) {
    free(colors);
}

char* create_success_message() {
    char* success_message = malloc(sizeof(char) * 50);
    strcpy(success_message, "Successful Allocation");
    return success_message;
}


int* initialize_colors(int j) {
    int* colors = malloc(sizeof(int) * j);
    int i;
    for (i = 0; i < j; i++) {
        colors[i] = 1;
    }
    return colors;
}

void mark_used_colors(NETMAP* novo_grafo, NO* temp_vertex, int* colors, int j) {
    CONEXAO* a;
    for (a = temp_vertex->inicio; a != NULL; a = a->proximo_no) {
        if (a->indice_no < j) {
            colors[a->indice_no] = 0;
        } else {
            NO* v;
            for (v = novo_grafo->entryPoint; v != NULL; v = v->proximo) {
                if (v->identificador == a->indice_no) {
                    if (v->cor > -1) {
                        colors[v->cor] = 0;
                    }
                    break;
                }
            }
        }
    }
}

int assign_color(NO* temp_vertex, int* colors, int j) {
    int i;
    for (i = 0; i < j; i++) {
        if (colors[i]) {
            temp_vertex->cor = i;
            return 1;
        }
    }
    return 0;
}

char* handle_spill(NO* temp_vertex) {
    printf("Pop: %d -> NO COLOR AVAILABLE\n", temp_vertex->identificador);
    char* resultado_aux = malloc(sizeof(char) * 50);
    strcpy(resultado_aux, "SPILL");
    return resultado_aux;
}
