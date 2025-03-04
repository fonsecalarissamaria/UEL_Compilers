#ifndef LEXICO_H
#define LEXICO_H

#include <stdio.h>

// Definição do tipo de token
typedef enum {
    TOKEN_PALAVRA_RESERVADA,
    TOKEN_IDENTIFICADOR,
    TOKEN_NUM_INTEIRO,
    TOKEN_NUM_REAL,
    TOKEN_OPERADOR,
    TOKEN_DELIMITADOR,
    TOKEN_STRING,
    TOKEN_FIM_ARQUIVO,
    TOKEN_ESPACO,
    TOKEN_ERRO
} TipoToken;

// Definição da estrutura Token
typedef struct {
    TipoToken tipo;
    char* lexema;
    int linha;
    int coluna;
} Token;

// Função para obter o próximo token do analisador léxico
Token proximoToken(FILE* fonte, int *linha, int *coluna);

// Função para inicializar a análise sintática
void inicializaSintatico(Token* tokens, size_t numTokens);

#endif // LEXICO_H
