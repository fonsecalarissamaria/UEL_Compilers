#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lexico.h"


// Funções auxiliares
int ehLetra(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

int ehDigito(char c) {
    return c >= '0' && c <= '9';
}

int ehEspaco(char c) {
    return c == ' ' || c == '\n' || c == '\t';
}

int ehDelimitador(const char *lexema) {
    const char *delimitadores[] = { ";", ",", ":", ".", "[", "]", "(", ")", "=", "<>", ">", ">=", "<-", "<=", "+", "-", "*", "<" };
    size_t numDelimitadores = sizeof(delimitadores) / sizeof(delimitadores[0]);
    size_t i;

    for (i = 0; i < numDelimitadores; ++i) 
    {
        if (strcmp(lexema, delimitadores[i]) == 0) {
            return 1;
        }
    }
    return 0;
}



int ehPalavraReservada(const char *lexema) {
    const char *palavrasReservadas[] = { "algoritmo", "inicio", "fim", "variaveis", "inteiro", "real", "caractere", "logico", 
                                         "vetor", "matriz", "tipo", "funcao", "procedimento", "se", "entao", "senao", 
                                         "enquanto", "faca", "para", "de", "ate", "passo", "repita", "leia", "imprima", 
                                         "verdadeiro", "falso", "e", "ou", "nao", "div" };
    size_t numPalavrasReservadas = sizeof(palavrasReservadas) / sizeof(palavrasReservadas[0]);
    size_t i;

    for (i = 0; i < numPalavrasReservadas; ++i) {
        if (strcmp(lexema, palavrasReservadas[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void converterParaMinusculas(char *str) {
    char *p = str;
    while (*p) {
        *p = tolower((unsigned char) *p);
        p++;
    }
}

void ignorarComentario(FILE* fonte) {
    char c;

    while ((c = fgetc(fonte)) != EOF && c != '\n') {
    }
    if (c == '\n') {
        ungetc(c, fonte);
    }
}

void ignorarComentarioBloco(FILE* fonte, int *linha, int *coluna) 
{
    char c;
    while ((c = fgetc(fonte)) != EOF) {
        (*coluna)++;
        if (c == '\n')
        {
            (*linha)++;
        }
        if (c == '}') {
            (*coluna)++;
            // Fim do comentário de bloco
            return;
        }
    }
}

Token proximoToken(FILE* fonte, int *linha, int *coluna) {
    Token token;
    token.lexema = NULL;
    token.linha = *linha;
    token.coluna = *coluna;

    int estado = 0;
    char c;
    int i;

    while ((c = fgetc(fonte)) != EOF) {
        
        switch (estado) {
            case 0:    
                if (ehEspaco(c)) 
                {
                    
                    if (c == '\n') {
                        (*linha)++;
                        *coluna = 0;
                    }else if(c == ' ')
                    {
                        (*coluna) = (*coluna) + 1;
                    }
                    continue;  // Ignora espaços e quebras de linha
                }else if (c == '"') 
                {
                    // Começa a ler uma string
                    estado = 2;
                    token.lexema = (char*) malloc(sizeof(char) * 256);
                    if (token.lexema == NULL) 
                    {
                        fprintf(stderr, "Erro de alocação de memória\n");
                        exit(1);
                    }
                    // add aqui
                    int x = strlen(token.lexema); 
                    (*coluna) = (*coluna) + x;
                    i = 0;
                }else if (c == '/')
                {   
                    char d; 
                    if((d = fgetc(fonte)) == '/')
                    {
                        // Começa a ler um comentário de linha
                        ignorarComentario(fonte);
                        continue;  // Volta ao início para processar o próximo token
                    }else 
                    {
                        token.tipo = TOKEN_DELIMITADOR;
                        token.lexema = (char*) malloc(sizeof(char) * 256);
                        token.lexema[0] = c;
                        token.lexema[1] = '\0';
                        int x = strlen(token.lexema); 
                        (*coluna) = (*coluna) + x;

                        return token;
                    }
                    
                    
                }else if (c == '{') 
                {
                    
                    // Começa a ler um comentário de bloco
                    ignorarComentarioBloco(fonte, linha, coluna);
                    continue;  // Volta ao início para processar o próximo token
                } else if (ehLetra(c)) 
                {
                    
                    // Reconhece identificadores e palavras reservadas
                    estado = 1;
                    token.lexema = (char*) malloc(sizeof(char) * 256);
                    if (token.lexema == NULL) 
                    {
                        fprintf(stderr, "Erro de alocação de memória\n");
                        exit(1);
                    }
                    token.lexema[0] = c;
                    i = 1;
                    while ((c = fgetc(fonte)) != EOF && (ehLetra(c) || ehDigito(c))) {
                        if (i >= 255) {
                            token.lexema = (char*) realloc(token.lexema, sizeof(char) * (i + 1));
                            if (token.lexema == NULL) {
                                fprintf(stderr, "Erro de alocação de memória\n");
                                exit(1);
                            }
                        }
                        token.lexema[i++] = c;
                    }
                    ungetc(c, fonte);  // Devolve o caractere extra ao buffer
                    token.lexema[i] = '\0';

                    // Converte lexema para minúsculas para comparação case-insensitive
                    converterParaMinusculas(token.lexema);

                    if (ehPalavraReservada(token.lexema)) {
                    
                        token.tipo = TOKEN_PALAVRA_RESERVADA;
                        int x = strlen(token.lexema); 
                        (*coluna) = (*coluna) + x;
                        //printf("Coluna que a palavra reservada está até então: %d\n", *(coluna));
                    } else {
                        token.tipo = TOKEN_IDENTIFICADOR;
                        int x = strlen(token.lexema); 
                        (*coluna) = (*coluna) + x;
                        //printf("Coluna que o identificador está até então: %d\n", *(coluna));
                    }
                    return token;
                } else if (ehDigito(c)) 
                {
                    
                    // Processar números
                    estado = 3;
                    token.lexema = (char*) malloc(sizeof(char) * 256);
                    if (token.lexema == NULL) {
                        fprintf(stderr, "Erro de alocação de memória\n");
                        exit(1);
                    }
                    token.lexema[0] = c;
                    i = 1;
                    while ((c = fgetc(fonte)) != EOF && ehDigito(c)) {
                        if (i >= 255) {
                            token.lexema = (char*) realloc(token.lexema, sizeof(char) * (i + 1));
                            if (token.lexema == NULL) {
                                fprintf(stderr, "Erro de alocação de memória\n");
                                exit(1);
                            }
                        }
                        token.lexema[i++] = c;
                    }
                    if (c == '.') {
                        token.lexema[i++] = c;
                        while ((c = fgetc(fonte)) != EOF && ehDigito(c)) {
                            if (i >= 255) {
                                token.lexema = (char*) realloc(token.lexema, sizeof(char) * (i + 1));
                                if (token.lexema == NULL) {
                                    fprintf(stderr, "Erro de alocação de memória\n");
                                    exit(1);
                                }
                            }
                            token.lexema[i++] = c;
                        }
                        ungetc(c, fonte);  // Devolve o caractere extra ao buffer
                        token.lexema[i] = '\0';
                        token.tipo = TOKEN_NUM_REAL;
                        int x = strlen(token.lexema); 
                        (*coluna) = (*coluna) + x;
                    } else {
                        ungetc(c, fonte);  // Devolve o caractere extra ao buffer
                        token.lexema[i] = '\0';
                        token.tipo = TOKEN_NUM_INTEIRO;
                        int x = strlen(token.lexema); 
                        (*coluna) = (*coluna) + x;
                    }
                    return token;
                } else if(c) {
                    // Reconhece delimitadores e operadores
                    if(c == 32)
                        token.tipo = TOKEN_ESPACO;
                    
                    token.lexema = (char*) malloc(sizeof(char) * 2);
                    if (token.lexema == NULL) {
                        fprintf(stderr, "Erro de alocação de memória\n");
                        exit(1);
                    }
                    token.lexema[0] = c;
                    token.lexema[1] = '\0';

                    if (ehDelimitador(token.lexema)) 
                    {   
                        if (c == 60) // Se for '<'
                        {
                            // Verifica se o próximo caractere é '-'
                            int d = fgetc(fonte);
                            
                            if (d == 45) { // <- 
                                //printf("Valor do próximo token: %d\n", d);
                                token.lexema = (char*) realloc(token.lexema, sizeof(char) * 3);
                                token.lexema[0] = c;
                                token.lexema[1] = 45;
                                token.lexema[2] = '\0'; // Terminar a string
                                token.tipo = TOKEN_OPERADOR;
                                (*coluna) += 2; // Ajusta a coluna para o caractere lido
                                //printf("1 Lexema guardado: %s\n", token.lexema);
                            } else if (d == 62) { // <>
                                //printf("Valor do próximo token: %d\n", d);
                                token.lexema = (char*) realloc(token.lexema, sizeof(char) * 3);
                                token.lexema[0] = c;
                                token.lexema[1] = 62;
                                token.lexema[2] = '\0'; // Terminar a string
                                token.tipo = TOKEN_OPERADOR;
                                (*coluna) += 2; // Ajusta a coluna para o caractere lido
                                //printf("2 Lexema guardado: %s\n", token.lexema);
                            }else if (d == 61) {  // <=
                                //printf("Valor do próximo token: %d\n", d);
                                token.lexema = (char*) realloc(token.lexema, sizeof(char) * 3);
                                token.lexema[0] = c;
                                token.lexema[1] = 61;
                                token.lexema[2] = '\0'; // Terminar a string
                                token.tipo = TOKEN_OPERADOR;
                                (*coluna) += 2; // Ajusta a coluna para o caractere lido
                                //printf("2 Lexema guardado: %s\n", token.lexema);
                            } else {
                                // Se não for nenhum, volta o caractere lido para o buffer
                                ungetc(d, fonte);
                                //printf("3 Lexema guardado: %s\n", token.lexema);
                                token.tipo = TOKEN_DELIMITADOR;
                                (*coluna) += 1;
                            }
                        }else if(c == 62) // Se for '>'
                        {
                            int e = fgetc(fonte);

                            if (e == 61) { // >=
                                token.lexema = (char*) realloc(token.lexema, sizeof(char) * 3);
                                token.lexema[0] = c;
                                token.lexema[1] = 61;
                                token.lexema[2] = '\0'; // Terminar a string
                                token.tipo = TOKEN_OPERADOR;
                                (*coluna) += 2; // Ajusta a coluna para o caractere lido
                                //printf("1 Lexema guardado: %s\n", token.lexema);
                            }else {
                                // Se não for nenhum, volta o caractere lido para o buffer
                                ungetc(e, fonte);
                                //printf("3 Lexema guardado: %s\n", token.lexema);
                                token.tipo = TOKEN_DELIMITADOR;
                                (*coluna) += 1;
                            }


                        }else {
                            token.tipo = TOKEN_DELIMITADOR;
                            (*coluna) += 1;
                        }
                    } else if (c != 32) {
                        // Caso não reconheça o token, reporta erro léxico
                        token.tipo = TOKEN_ERRO;
                        int x = strlen(token.lexema); 
                        (*coluna) = (*coluna) + x;
                        token.linha = *linha;
                        token.coluna = *coluna;
                        token.lexema = (char*) realloc(token.lexema, sizeof(char) * 2);
                        if (token.lexema == NULL) {
                            fprintf(stderr, "Erro de alocação de memória\n");
                            exit(1);
                        }
                        token.lexema[0] = c;
                        token.lexema[1] = '\0';
                        return token;
                    }
                    return token;
                }
                break;

            case 1:
                // Estado para processar identificadores e palavras reservadas
                break;

            case 2:
                if (c == '"') {
                    // Fim da string
                    token.lexema[i] = '\0';
                    token.tipo = TOKEN_STRING;
                    //printf("token: %s\n", token.lexema);
                    estado = 0;  // Volta ao estado inicial
                    return token;
                } else if (c == EOF) {
                    // Se encontrar EOF antes do fim da string
                    token.tipo = TOKEN_ERRO;
                    return token;
                } else {
                    // Continua lendo a string
                    if (i >= 255) {
                        token.lexema = (char*) realloc(token.lexema, sizeof(char) * (i + 1));
                        if (token.lexema == NULL) {
                            fprintf(stderr, "Erro de alocação de memória\n");
                            exit(1);
                        }
                    }
                    token.lexema[i++] = c;
                }
                break;
        }
    }

    // Caso atinja o final do arquivo
    token.tipo = TOKEN_FIM_ARQUIVO;
    return token;
}

// Função para remover a quebra de linha final do lexema
void removerQuebraDeLinhaFinal(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
}


int main() {
    Token *tokens = NULL;
    size_t numTokens = 0;
    size_t capacidade = 10;
    int linha = 1;
    int coluna = 0;
    int programaCorreto = 1;

    // Aloca memória inicial para tokens
    tokens = (Token*) malloc(capacidade * sizeof(Token));
    if (tokens == NULL) {
        fprintf(stderr, "Erro de alocação de memória\n");
        return 1;
    }

    Token token;
    while (1) {
        token = proximoToken(stdin, &linha, &coluna);
        if (token.tipo == TOKEN_FIM_ARQUIVO) {
            break;
        }

        // Realoca memória se necessário
        if (numTokens >= capacidade) {
            capacidade *= 2;
            Token *temp = (Token*) realloc(tokens, capacidade * sizeof(Token));
            if (temp == NULL) {
                fprintf(stderr, "Erro de alocação de memória\n");
                return 1;
            }
            tokens = temp;
        }

        // Armazena o token
        tokens[numTokens++] = token;
        //fprintf(stderr, "mais um\n");
    }

    // Imprime os erros léxicos
    size_t i;
    size_t tamanhoBuffer = 1024;
    char *bufferErros = (char *)malloc(tamanhoBuffer);
    if (bufferErros == NULL) {
        fprintf(stderr, "Erro de alocação de memória\n");
        return 1;
    }
    size_t offsetBuffer = 0;

    // Itera sobre os tokens para acumular os erros
    for (i = 0; i < numTokens; i++) 
    {
        if (tokens[i].tipo == TOKEN_ERRO) {
            programaCorreto = 0;
            int len = snprintf(bufferErros + offsetBuffer, tamanhoBuffer - offsetBuffer, 
                               "ERRO LEXICO. Linha: %d Coluna: %d -> '%s'\n", 
                               tokens[i].linha, tokens[i].coluna, tokens[i].lexema);
            if ((size_t)len >= (tamanhoBuffer - offsetBuffer)) {
                tamanhoBuffer *= 2;
                char *temp = (char *)realloc(bufferErros, tamanhoBuffer);
                if (temp == NULL) {
                    fprintf(stderr, "Erro de alocação de memória\n");
                    free(bufferErros);
                    return 1;
                }
                bufferErros = temp;
            }
            offsetBuffer += len;
        }
    }

    // Remove o último '\n' do buffer, se presente
    if (offsetBuffer > 0 && bufferErros[offsetBuffer - 1] == '\n') {
        bufferErros[offsetBuffer - 1] = '\0';
        offsetBuffer--;
    }

    // Imprime os erros armazenados no buffer
    if (offsetBuffer > 0) {
        printf("%s", bufferErros);
    }

    // Libera a memória do buffer de erros
    free(bufferErros);

    
    // Chama o analisador sintático
    if(programaCorreto != 0)
        inicializaSintatico(tokens, numTokens);

    if (programaCorreto) {
        printf("PROGRAMA CORRETO.");
    }

    // Libera memória dos tokens
    for (i = 0; i < numTokens; i++) {
        free(tokens[i].lexema);
    }
    free(tokens);

    return 0;
}

