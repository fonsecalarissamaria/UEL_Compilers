#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matriz.h"
#include "symbol_table.h"
#include "funcoes.h"

#define SYMBOL_TABLE_SIZE 100

// Função de hash simples para gerar índices para a tabela de símbolos
unsigned int hash(char *name) {
    unsigned int hash_value = 0;
    while (*name) {
        hash_value = (hash_value * 31) + *name++;
    }
    return hash_value % SYMBOL_TABLE_SIZE;
}

void init_symbol_table() {
    for (int i = 0; i < SYMBOL_TABLE_SIZE; i++) {
        symbol_table[i] = NULL; // Inicializa todos os ponteiros como NULL
    }
}

int *find_symbol(const char *name) 
{
    unsigned int index = hash(name);
    Symbol *current = symbol_table[index];
    
    // Percorre a lista encadeada para verificar se o símbolo existe
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            if(current->is_matrix == 1) // O símbolo foi encontrado
                return 1;
            else
                return 0; 
              
        }
        current = current->next;
    }

    return 0;
}

// Função para obter o valor de um símbolo
void* getSymbolValue(char *name) {
    unsigned int index = hash(name);
    Symbol *current = symbol_table[index];
    int precision_print = getFloatPrecision();

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // Verifica o tipo e retorna o valor ou a matriz correspondente
            if (current->type == SYMBOL_FLOAT) {
                printf("\n%.*f\n\n", precision_print, current->value);
                return &current->value;  // Retorna ponteiro para o valor do tipo float
            } else if (current->type == SYMBOL_MATRIX) {
                printf("\n");
                exibirEstruturaMatriz(current->data.matrix);
                printf("\n\n");
                return current->data.matrix;  // Retorna ponteiro para a matriz
            } else {
                printf("\nUndefined symbol\n\n");
                return NULL;
            }
        }
        current = current->next;
    }

    // Se o símbolo não for encontrado
    printf("\nUndefined symbol\n\n");
    return NULL;
}

void* getSymbolValue_NoPrint(char *name) {
    unsigned int index = hash(name);
    Symbol *current = symbol_table[index];
    //int precision_print = getFloatPrecision();

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            // Verifica o tipo e retorna o valor ou a matriz correspondente
            if (current->type == SYMBOL_FLOAT) {
                return &current->value;  // Retorna ponteiro para o valor do tipo float
            } else if (current->type == SYMBOL_MATRIX) {
                return current->data.matrix;  // Retorna ponteiro para a matriz
            } else {
                return NULL;
            }
        }
        current = current->next;
    }

    return NULL;
}

// Função para adicionar um símbolo à tabela
void addSymbol(char *name, float value) 
{
    unsigned int index = hash(name);
    Symbol *new_symbol = malloc(sizeof(Symbol));
    if (!new_symbol) {
        printf("Erro de alocação de memória para símbolo.\n");
        exit(1);
    }

    new_symbol->name = name;
    new_symbol->value = value;
    new_symbol->next = symbol_table[index];  // Adiciona no início da lista
    symbol_table[index] = new_symbol;       // Atualiza a tabela de símbolos
    new_symbol->is_matrix = 0;
    printf("\n%f\n\n", new_symbol->value);
    //printf("\nindice: %d\n\n", index);
}

void addMatrixSymbol(char *name, void *matrix) {
    
    unsigned int index = hash(name);
    
    // Cria um novo símbolo para a matriz
    Symbol *new_symbol = malloc(sizeof(Symbol));
    if (!new_symbol) {
        printf("Erro de alocação de memória para símbolo.\n");
        exit(1);
    }

    new_symbol->name = name;  // Copia o nome do símbolo
    new_symbol->type = SYMBOL_MATRIX;       // Define o tipo como SYMBOL_MATRIX
    new_symbol->data.matrix = matrix; // Armazena o ponteiro para a matriz
    new_symbol->next = symbol_table[index]; // Insere no início da lista encadeada
    symbol_table[index] = new_symbol;      // Atualiza a tabela de símbolos
    new_symbol->is_matrix = 1; // é matriz

    //printf("Matriz %s inserida com sucesso\n\n", new_symbol->name);
    exibirEstruturaMatriz(matrix);
    printf("\n");
}

void* getMatrixSymbol(char *name) {
    unsigned int index = hash(name);
    Symbol *current = symbol_table[index];
    while (current != NULL) {
        if (strcmp(current->name, name) == 0 && current->type == SYMBOL_MATRIX) {
            return current->data.matrix;  // Retorna o ponteiro para a matriz
        }
        current = current->next;
    }
    printf("Erro: Matriz com nome '%s' não encontrada.\n", name);
    return NULL;
}



void showSymbols() {
    printf("\n\n");
    
    // Itera por toda a tabela de símbolos
    for (int i = 10; i < SYMBOL_TABLE_SIZE; i++) {
        Symbol *current = symbol_table[i];

        // Verifica se o símbolo existe e se ele é de um tipo válido
        if (current != NULL) {
            
            if(current->type == 0)
                printf("%s - FLOAT\n", current->name);
            else if (current->type == 1)
            {
                estrutura_matriz_t *mat = current->data.matrix;
                printf("%s - MATRIX [%d][%d]\n", current->name, mat->qtdLinhas, mat->qtdColunas);
            }
        }
    }

    printf("\n\n");
}



// Função para verificar se um símbolo está na tabela de símbolos
int checkSymbol(char *name) {
    unsigned int index = hash(name);
    Symbol *current = symbol_table[index];
    
    // Percorre a lista encadeada para verificar se o símbolo existe
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return 1;  // O símbolo foi encontrado
        }
        current = current->next;
    }
    
    // O símbolo não foi encontrado
    printf("Undefined symbol [%s]\n", name);
    return 0;  // Símbolo não encontrado
}


void* add_matriz(char *matA, char *matB) 
{
    //printf("Nome da 1ª matriz: %s \nNome da 2ª matriz: %s", matA, matB);
    unsigned int indexA = hash(matA);
    Symbol *currentA = symbol_table[indexA];
    estrutura_matriz_t *mat_A = currentA->data.matrix;
    unsigned int indexB = hash(matB);
    Symbol *currentB = symbol_table[indexB];
    estrutura_matriz_t *mat_B = currentB->data.matrix;
    addMatrices(mat_A, mat_B);
}

void* sub_matriz(char *matA, char *matB) 
{
    unsigned int indexA = hash(matA);
    Symbol *currentA = symbol_table[indexA];
    estrutura_matriz_t *mat_A = currentA->data.matrix;
    unsigned int indexB = hash(matB);
    Symbol *currentB = symbol_table[indexB];
    estrutura_matriz_t *mat_B = currentB->data.matrix;
    subtractMatrices(mat_A, mat_B);
}

void* mul_matriz(char *matA, char *matB) 
{
    unsigned int indexA = hash(matA);
    Symbol *currentA = symbol_table[indexA];
    estrutura_matriz_t *mat_A = currentA->data.matrix;
    unsigned int indexB = hash(matB);
    Symbol *currentB = symbol_table[indexB];
    estrutura_matriz_t *mat_B = currentB->data.matrix;
    multiplyMatrices(mat_A, mat_B);
}


void mul_matriz_escalar(char *matA, float scalar) {
    //printf("Chegou aqui matriz %s e escalar %f\n\n", matA, scalar);
    unsigned int indexA = hash(matA);
    Symbol *currentA = symbol_table[indexA];
    estrutura_matriz_t *mat_A = currentA->data.matrix;

    estrutura_matriz_t *result = multiplyMatrixByScalar(mat_A, scalar);  // Usando a função fornecida
    // A matriz resultante pode ser armazenada em alguma estrutura de dados ou retornar
}
