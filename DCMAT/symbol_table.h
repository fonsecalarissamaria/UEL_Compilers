// symbol_table.h
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#define SYMBOL_TABLE_SIZE 100

typedef enum {
    SYMBOL_FLOAT,
    SYMBOL_MATRIX,
    SYMBOL_INT,
    SYMBOL_IDENTIFIER
} SymbolType;

// Estrutura de um símbolo na tabela
typedef struct Symbol {
    char *name;
    float value;
    struct Symbol *next;  // Lista encadeada
    SymbolType type;
    union {  
        void *matrix; // Para matrizes (MATRIX)
    } data;
    int is_matrix;
} Symbol;

// Tabela de símbolos (uma lista encadeada)
Symbol *symbol_table[SYMBOL_TABLE_SIZE];

void* getSymbolValue(char *name);
void addSymbol(char *name, float value);
void showSymbols();
int checkSymbol(char *name);
void addMatrixSymbol(char *name, void *matrix);
void* getMatrixSymbol(char *name);
void* sub_matriz(char *matA, char *matB);
void* add_matriz(char *matA, char *matB);
void* mul_matriz(char *matA, char *matB);
void mul_matriz_escalar(char *matA, float scalar);
void* getSymbolValue_NoPrint(char *name);
int *find_symbol(const char *name);
void init_symbol_table();

#endif
