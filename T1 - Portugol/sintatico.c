#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexico.h"

// Variáveis globais
Token *tokens;
size_t numTokens;
size_t index = 0;

// Funções de ajuda
Token* lookahead();
void consumirToken();
void erroSintatico(const Token* token);

// Funções de parsing
void parsePrograma();
void parseBlocoVariaveis();
void parseProcedimentoFuncao();
void parseBlocoComandos();
void parseDeclaraProcedimento();
void parseDeclaraFuncao();
void parseParametros();
void parseDeclaraParametros();
void parseDeclaracoes();
void parseDeclaraTipo();
void parseDeclaraVariaveis();
void parseDeclaraIdentificador();
void parseVariavel();
void parseExprIter();
void parseExpressao();
void parseComandos();
void parseListaComandos();
void parseDimensao();
void parseTipoBasico();
void parseExpressaoSimples();
void parseTermo();
void parseFator();
void parseVetorMatriz();

// Funções de ajuda
Token* lookahead() {
    if (index < numTokens) {
        //printf("Lexema: %s Tipo: %d\n", tokens[index].lexema, tokens[index].tipo);
        return &tokens[index];
    } else {
        return NULL; // Fim dos tokens
    }
}

void consumirToken() {
    if (index < numTokens) 
    {
        //printf("Consumiu o token: %s\n",tokens[index].lexema);
        index++;
    }   
}

void erroSintatico(const Token* token) {
    
    printf("ERRO DE SINTAXE. Linha: %d Coluna: %d -> '%s'",
            token->linha, token->coluna, token->lexema);
    exit(EXIT_FAILURE);
}

void parsePrograma() {
    Token* token = lookahead();
    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "algoritmo") == 0) 
    {   
        consumirToken(); // consome o token "algoritmo"
        token = lookahead();
        
        if (token && token->tipo == TOKEN_IDENTIFICADOR) 
        {
            consumirToken(); // identificador
            token = lookahead();

            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) 
            {
                consumirToken(); // ";"
                parseBlocoVariaveis();
                parseProcedimentoFuncao();
                parseBlocoComandos();
                token = lookahead();
                
                
                if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ".") == 0) {
                    consumirToken(); // "."
                } else {
                    erroSintatico(token); // não tem ponto 
                }
            } else {
                erroSintatico(token); // não tem ";"
            }
        } else {
            erroSintatico(token); // não tem identificador após "algoritmo"
        }
    } else {
        erroSintatico(token); // não tem "algoritmo"
    }
}




void parseProcedimentoFuncao() {
    Token* token = lookahead();
    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "procedimento") == 0) 
    {
        parseDeclaraProcedimento();
        parseProcedimentoFuncao();
    }else if(token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "funcao") == 0)
    {
        parseDeclaraFuncao();
        parseProcedimentoFuncao();
    }

}


void parseDeclaraProcedimento() {
    Token* token = lookahead();
    
    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "procedimento") == 0) 
    {
        consumirToken(); // Consumir "procedimento"
        token = lookahead();
        
        if (token && token->tipo == TOKEN_IDENTIFICADOR) 
        {
            consumirToken(); // Consumir identificador
            parseParametros(); // Processar parâmetros
            token = lookahead();

            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) 
            {
                consumirToken(); // Consumir ";"
                parseDeclaraParametros(); // Processar declarações de parâmetros
                parseBlocoVariaveis(); // Processar bloco de variáveis
                parseBlocoComandos(); // Processar bloco de comandos

                if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) 
                {
                    consumirToken(); // Consome ";"
                }else 
                {
                    erroSintatico(token);  // Erro: Esperado ";"
                }  
            } else {
                erroSintatico(token);  // Erro: Esperado ";"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado identificador
        }
    } else {
        erroSintatico(token);  // Erro: Esperado "procedimento"
    }
}

void parseDeclaraFuncao() {
    Token* token = lookahead();
    
    // Verifica se o token é "funcao"
    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "funcao") == 0) 
    {
        consumirToken(); // Consome "funcao"
        token = lookahead();
        
        // Verifica se o próximo token é um identificador
        if (token && token->tipo == TOKEN_IDENTIFICADOR) 
        {
            consumirToken(); // Consome identificador
            parseParametros(); // Processa parâmetros
            token = lookahead();
            
            // Verifica se o próximo token é ":"
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ":") == 0) 
            {
                consumirToken(); // Consome ":"
                token = lookahead();
                
                // Verifica se o próximo token é um tipo básico
                if ((token && token->tipo == TOKEN_PALAVRA_RESERVADA &&
                    (strcmp(token->lexema, "inteiro") == 0 ||
                     strcmp(token->lexema, "real") == 0 ||
                     strcmp(token->lexema, "caractere") == 0 ||
                     strcmp(token->lexema, "logico") == 0 ||
                     strcmp(token->lexema, "string") == 0)) || (token->tipo == TOKEN_IDENTIFICADOR)) 
                {
                    consumirToken(); // Consome o tipo básico
                    token = lookahead();
                    
                    // Verifica se o próximo token é ";"
                    if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) 
                    {
                        consumirToken(); // Consome ";"
                        parseDeclaraParametros(); // Processa declarações de parâmetros
                        parseBlocoVariaveis(); // Processa bloco de variáveis
                        parseBlocoComandos(); // Processa bloco de comandos

                        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) 
                        {
                            consumirToken(); // Consome ";"
                        }else 
                        {
                            erroSintatico(token);  // Erro: Esperado ";"
                        }
                        
                    } else {
                        erroSintatico(token);  // Erro: Esperado ";"
                    }
                } else {
                    erroSintatico(token);  // Erro: Esperado tipo básico
                }
            } else {
                erroSintatico(token);  // Erro: Esperado ":"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado identificador
        }
    } else {
        erroSintatico(token);  // Erro: Esperado "funcao"
    }
}


void parseParametros() {
    Token* token = lookahead();
    
    if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "(") == 0) {
        consumirToken(); // Consumir "("
        
        parseDeclaraIdentificador(); // Processar declarações de identificadores
        
        token = lookahead();
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ")") == 0) {
            consumirToken(); // Consumir ")"
        } else {
            erroSintatico(token);  // Erro: Esperado ")"
        }
    }
}



void parseDeclaraParametros() {
    // Chama parseDeclaracoes para processar declarações de parâmetros
    parseDeclaracoes();
}


void parseBlocoVariaveis() {
    Token* token = lookahead();
    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "variaveis") == 0) {
        consumirToken(); // consome o token "variaveis"
        parseDeclaracoes();
    }
    // Caso não haja variáveis, não faz nada
}


void parseDeclaracoes() {
    
    Token* token = lookahead();
    
    // Verifica se há um token válido para processar
    if (token == NULL) {
        return; // Se não há token, termina a análise
    }

    if (token && 
        ((token->tipo == TOKEN_PALAVRA_RESERVADA && 
          (strcmp(token->lexema, "inteiro") == 0 ||
           strcmp(token->lexema, "real") == 0 ||
           strcmp(token->lexema, "caractere") == 0 ||
           strcmp(token->lexema, "logico") == 0)) || 
         token->tipo == TOKEN_IDENTIFICADOR))
    {   
        parseDeclaraVariaveis(); // Processa declaração de variáveis
        parseDeclaracoes();
    }else if (strcmp(token->lexema, "tipo") == 0) 
    {
        parseDeclaraTipo(); // Processa declaração de tipo personalizado
        parseDeclaracoes();
        
    }else if(token->tipo == TOKEN_PALAVRA_RESERVADA && ((strcmp(token->lexema, "inicio") == 0 ) || (strcmp(token->lexema, "procedimento") == 0) || (strcmp(token->lexema, "variaveis") == 0)) )
    {
        // ignora 
    }else
    {
        erroSintatico(token);  // Erro: Esperado identificador ou variável ou "tipo"
    }

}



void parseDeclaraTipo() {
    Token* token = lookahead();

    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA &&
        (strcmp(token->lexema, "vetor") == 0 || strcmp(token->lexema, "tipo") == 0 ||
         strcmp(token->lexema, "matriz") == 0)) {
        
        consumirToken(); // Consome "vetor", "tipo" ou "matriz"
        token = lookahead();
        
        if (token && token->tipo == TOKEN_IDENTIFICADOR) {
            consumirToken(); // Consome identificador
            token = lookahead();

            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "=") == 0) {
                consumirToken(); // Consome "="
                parseVetorMatriz();
                
                token = lookahead(); // Atualiza o token após parseVetorMatriz
                
                if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "[") == 0) {
                    consumirToken(); // Consome "["
                    parseDimensao();
                    
                    token = lookahead(); // Atualiza o token após parseDimensao
                    
                    if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "]") == 0) {
                        consumirToken(); // Consome "]"
                        parseTipoBasico();
                        
                        token = lookahead(); // Atualiza o token após parseTipoBasico
                        
                        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) {
                            consumirToken(); // Consome ";"
                        } else {
                            erroSintatico(token);  // Erro: Esperado ";"
                        }
                    } else {
                        erroSintatico(token);  // Erro: Esperado "]"
                    }
                } else {
                    erroSintatico(token);  // Erro: Esperado "["
                }
            } else {
                erroSintatico(token);  // Erro: Esperado "="
            }
        } else {
            erroSintatico(token);  // Erro: Esperado identificador
        }
    } else {
        erroSintatico(token);  // Erro: Esperado "vetor", "tipo" ou "matriz"
    }
}


void parseVetorMatriz() {
    Token* token = lookahead();

    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA &&
        (strcmp(token->lexema, "vetor") == 0 || strcmp(token->lexema, "matriz") == 0)) {
        
        consumirToken(); // Consome "vetor" ou "matriz"
    } else {
        erroSintatico(token);  // Erro: Esperado "vetor" ou "matriz"
    }
}



void parseDeclaraVariaveis() {
    Token* token = lookahead();
    
    if (token && 
        ((token->tipo == TOKEN_PALAVRA_RESERVADA && 
          (strcmp(token->lexema, "inteiro") == 0 ||
           strcmp(token->lexema, "real") == 0 ||
           strcmp(token->lexema, "caractere") == 0 ||
           strcmp(token->lexema, "logico") == 0)) || 
         token->tipo == TOKEN_IDENTIFICADOR)) {
        
        consumirToken(); // Consome o tipo básico
        token = lookahead();
        
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ":") == 0) {
            consumirToken(); // Consome ":"
            parseDeclaraIdentificador(); // Processa identificadores
            token = lookahead();
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) {
                consumirToken(); // Consome ";"
            } else {
                erroSintatico(token);  // Erro: Esperado ";"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado ":"
        }
    } else {
        erroSintatico(token);  // Erro: Esperado tipo básico
    }
}


void parseDeclaraIdentificador() {
    Token* token = lookahead();

    if (token && token->tipo == TOKEN_IDENTIFICADOR) {
        consumirToken(); // Consumir identificador
        
        token = lookahead();
        while (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ",") == 0) {
            consumirToken(); // Consumir ","
            
            token = lookahead();
            if (token && token->tipo == TOKEN_IDENTIFICADOR) {
                consumirToken(); // Consumir identificador
            } else {
                erroSintatico(token);  // Erro: Esperado identificador
            }
            
            token = lookahead();
        }
    } else {
        erroSintatico(token);  // Erro: Esperado identificador
    }
}


void parseDimensao() {
    Token* token = lookahead();

    if (token && token->tipo == TOKEN_NUM_INTEIRO) {
        consumirToken(); // Consumir número inteiro
        
        token = lookahead();
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ":") == 0) {
            consumirToken(); // Consumir ":"
            
            token = lookahead();
            if (token && token->tipo == TOKEN_NUM_INTEIRO) {
                consumirToken(); // Consumir número inteiro
                
                // Verifica se há mais dimensões
                token = lookahead();
                if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ",") == 0) {
                    consumirToken(); // Consumir ","
                    
                    parseDimensao(); // Processar dimensões adicionais
                }
            } else {
                erroSintatico(token);  // Erro: Esperado número inteiro
            }
        } else {
            erroSintatico(token);  // Erro: Esperado ":"
        }
    } else {
        erroSintatico(token);  // Erro: Esperado número inteiro
    }
}


void parseTipoBasico() {
    Token* token = lookahead();

    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && 
        (strcmp(token->lexema, "inteiro") == 0 ||
         strcmp(token->lexema, "real") == 0 ||
         strcmp(token->lexema, "caractere") == 0 ||
         strcmp(token->lexema, "logico") == 0)) {
        consumirToken(); // Consumir tipo básico
        //token = lookahead();
    } else if (token && token->tipo == TOKEN_IDENTIFICADOR) {
        consumirToken(); // Consumir identificador (tipo definido pelo usuário)
        //token = lookahead();
    } else {
        erroSintatico(token);  // Erro: Esperado tipo básico
    }
}



void parseBlocoComandos() {
    Token* token = lookahead();

    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "inicio") == 0) 
    {
        consumirToken(); // Consumir "inicio"
        parseListaComandos(); // Processar lista de comandos
        token = lookahead();
        
        if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "fim") == 0) 
        {
            consumirToken(); // Consumir "fim"
            token = lookahead();
        } else {
            erroSintatico(token);  // Erro: Esperado "fim"
        }
    } else {
        erroSintatico(token);  // Erro: Esperado "inicio"
    }
}




void parseListaComandos() {
    parseComandos(); // Parse o primeiro comando

    
    Token* token = lookahead();
    while (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ";") == 0) {
        consumirToken(); // Consome o ponto e vírgula
        parseComandos(); // Parse o próximo comando
        token = lookahead();
    }
}



void parseComandos() {
    Token* token = lookahead();
    //printf("--------------ENTROU NA FUNÇÃO COMANDOS\n");

    if (token && token->tipo == TOKEN_IDENTIFICADOR) {
        // Comando: identificador
        consumirToken();
        token = lookahead();
        //parseComandos();
        
        // Verifica se o próximo token é uma chamada de função ou expressão
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "(") == 0) 
        {
            consumirToken(); // Consumir "("
            parseExprIter(); // Processa expressão iterável
            token = lookahead();
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ")") == 0) {
                consumirToken(); // Consumir ")"
            } else {
                erroSintatico(token);  // Erro: Esperado ")"
            }
        }else if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "[") == 0) 
        {
            consumirToken(); // Consumir "["
            parseExprIter(); // Processar a expressão iterável dentro dos colchetes
            token = lookahead();
            
            // Verifica se o próximo token é um "]"
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "]") == 0) {
                consumirToken(); // Consumir "]"
                token = lookahead();

                if (token && token->tipo == TOKEN_OPERADOR && strcmp(token->lexema, "<-") == 0) 
                {
                    consumirToken(); // Consumir "<-"
                    parseExpressao(); // Processa expressão
                    token = lookahead();
                }
            } else {
                erroSintatico(token);  // Erro: Esperado "]"
            }
        }else if (token && token->tipo == TOKEN_OPERADOR && ((strcmp(token->lexema, "<-") == 0) || (strcmp(token->lexema, "<")))) 
        {   
            consumirToken(); // Consumir "<-"
            parseExpressao(); // Processa expressão
            token = lookahead();
        }


    }else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "se") == 0) {
        // Comando: se Expressao entao ListaComandos [senao ListaComandos] fim se
        consumirToken(); // Consumir "se"

        parseExpressao(); // Processa expressão
        token = lookahead();

        if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "entao") == 0) {
            consumirToken(); // Consumir "entao"
            parseListaComandos(); // Processa lista de comandos
            token = lookahead();
            
            if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "fim") == 0) {
                consumirToken(); // Consumir "fim"
                token = lookahead();
                if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "se") == 0) {
                    consumirToken(); // Consumir "se"
                    
                } else {
                    token->coluna++;
                    erroSintatico(token);  // Erro: Esperado "se"
                }
            }else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "senao") == 0)
            {   
                consumirToken(); // Consumir "senao"
                parseListaComandos(); // Processa lista de comandos
                token = lookahead();
                if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "fim") == 0) 
                {
                    consumirToken(); // Consumir "fim"
                    token = lookahead();
                    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "se") == 0) {
                        consumirToken(); // Consumir "se"
                    } else {
                        erroSintatico(token);  // Erro: Esperado "se" após "fim"
                    }
                }else 
                {
                    erroSintatico(token);  // Erro: Esperado "fim"
                }
            }else {
                erroSintatico(token);  // Erro: Esperado "fim se" ou "senao"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado "entao"
        }
    } else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "enquanto") == 0) {
        // Comando: enquanto Expressao faca ListaComandos fim enquanto
        consumirToken(); // Consumir "enquanto"
        parseExpressao(); // Processa expressão
        token = lookahead();
        if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "faca") == 0) {
            consumirToken(); // Consumir "faca"
            parseListaComandos(); // Processa lista de comandos
            token = lookahead();
            if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "fim") == 0) {
                consumirToken(); // Consumir "fim"
                token = lookahead();
                if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "enquanto") == 0) {
                    consumirToken(); // Consumir "enquanto"
                } else {
                    erroSintatico(token);  // Erro: Esperado "enquanto"
                }
            } else {
                //printf("Token atual: %s\n\n", token->lexema);
                Token *tokenAux = lookahead(); // Token atual 
                consumirToken();
                Token *token2 = lookahead(); // Próximo 
                tokenAux->coluna = token2->coluna;
                tokenAux->linha = token2->linha; 
                erroSintatico(tokenAux);  // Erro: Esperado "fim"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado "faca"
        }
    } else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "para") == 0) {
        // Comando: para identificador de Expressao ate Expressao [passo Expressao] faca ListaComandos fim para
        consumirToken(); // Consumir "para"
        token = lookahead();
        if (token && token->tipo == TOKEN_IDENTIFICADOR) {
            consumirToken(); // Consumir identificador
            token = lookahead();
            if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "de") == 0) {
                consumirToken(); // Consumir "de"
                parseExpressao(); // Processa expressão
                token = lookahead();
                if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "ate") == 0) 
                {
                    consumirToken(); // Consumir "ate"
                    parseExpressao(); // Processa expressão
                    token = lookahead();
                   
                    if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "faca") == 0) 
                    {

                        consumirToken(); // Consumir "faca"
                        parseListaComandos(); // Processa lista de comandos
                        token = lookahead();
                        if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "fim") == 0) {
                            consumirToken(); // Consumir "fim"
                            token = lookahead();
                            if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "para") == 0) {
                                consumirToken(); // Consumir "para"
                            } else {
                                erroSintatico(token);  // Erro: Esperado "para"
                            }
                        } else {
                            erroSintatico(token);  // Erro: Esperado "fim"
                        }
                    } else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "passo") == 0) {
                        consumirToken(); // Consumir "passo"
                        parseExpressao(); // Processa expressão
                        token = lookahead();
                        if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "faca") == 0) {
                            consumirToken(); // Consumir "faca"
                            parseListaComandos(); // Processa lista de comandos
                            token = lookahead();
                            if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "fim") == 0) {
                                consumirToken(); // Consumir "fim"
                                token = lookahead();
                                if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "para") == 0) {
                                    consumirToken(); // Consumir "para"
                                } else {
                                    erroSintatico(token);  // Erro: Esperado "para"
                                }
                            } else {
                                erroSintatico(token);  // Erro: Esperado "fim"
                            }
                        } else {
                            erroSintatico(token);  // Erro: Esperado "faca"
                        }
                    }
                } else 
                {
                    erroSintatico(token);  // Erro: Esperado "faca" ou "passo"
                }
            } else {
                erroSintatico(token);  // Erro: Esperado "de"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado identificador
        }
    } else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "repita") == 0) {
        // Comando: repita ListaComandos ate Expressao
        consumirToken(); // Consumir "repita"
        parseListaComandos(); // Processa lista de comandos
        token = lookahead();
        if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "ate") == 0) {
            consumirToken(); // Consumir "ate"
            parseExpressao(); // Processa expressão
        } else {
            erroSintatico(token);  // Erro: Esperado "ate"
        }
    } else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "leia") == 0) {
        // Comando: leia ( Variavel )
        consumirToken(); // Consumir "leia"
        token = lookahead();
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "(") == 0) {
            consumirToken(); // Consumir "("
            parseVariavel(); // Processa variável
            token = lookahead();
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ")") == 0) {
                consumirToken(); // Consumir ")"
            } else {
                erroSintatico(token);  // Erro: Esperado ")"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado "("
        }
    } else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "imprima") == 0) 
    {
        // Comando: imprima ( ExprIter )
        consumirToken(); // Consumir "imprima"
        token = lookahead();
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "(") == 0) {
            consumirToken(); // Consumir "("
            parseExprIter(); // Processa expressão iterável
            token = lookahead();
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ")") == 0) {
                consumirToken(); // Consumir ")"
            } else {
                erroSintatico(token);  // Erro: Esperado ")"
            }
        } else {
            erroSintatico(token);  // Erro: Esperado "("
        }
    } 

    if(token && token->tipo == TOKEN_ESPACO)
    {
        consumirToken();
        token = lookahead();
    }
}



void parseExpressao() {
    parseExpressaoSimples(); // Processa a expressão inicial
    
    Token* token = lookahead();
    while (token && (token->tipo == TOKEN_OPERADOR || token->tipo == TOKEN_DELIMITADOR) && 
          (strcmp(token->lexema, "=") == 0 || 
           strcmp(token->lexema, "<>") == 0 ||
           strcmp(token->lexema, "<") == 0 ||
           strcmp(token->lexema, "<=") == 0 ||
           strcmp(token->lexema, ">=") == 0 ||
           strcmp(token->lexema, ">") == 0)) 
    {
        consumirToken(); // Consumir operador de comparação
        parseExpressaoSimples(); // Analisar a expressão à direita do operador
        token = lookahead(); // Verificar o próximo token
    }
}


void parseExpressaoSimples() {
    Token* token = lookahead();
    
    // Opção para sinais + ou - antes da expressão
    if (token && token->tipo == TOKEN_DELIMITADOR && 
          (strcmp(token->lexema, "+") == 0 || 
           strcmp(token->lexema, "-") == 0)) {
        consumirToken(); // Consumir sinal
    }
    
    parseTermo(); // Processa o termo inicial
    
    // Atualiza o token
    token = lookahead();
    
    // Processa operadores e termos adicionais
    while (token && 
          ((token->tipo == TOKEN_DELIMITADOR && 
            (strcmp(token->lexema, "+") == 0 || 
             strcmp(token->lexema, "-") == 0)) || 
           (token->tipo == TOKEN_PALAVRA_RESERVADA && 
            strcmp(token->lexema, "ou") == 0))) {
        
        consumirToken(); // Consumir operador +, - ou OU
        parseTermo(); // Analisar o termo à direita do operador
        
        // Atualiza o token após processar o termo
        token = lookahead();
    }
}



void parseTermo() {
    parseFator(); // Processa o fator inicial
    Token* token = lookahead();

    while (token && ((token->tipo == TOKEN_DELIMITADOR && (strcmp(token->lexema, "*") == 0 || strcmp(token->lexema, "/") == 0)) || 
    (token->tipo == TOKEN_PALAVRA_RESERVADA && (strcmp(token->lexema, "div") == 0 || strcmp(token->lexema, "e") == 0)))) 
    {
        consumirToken(); // Consumir operador * / DIV E
        parseFator(); // Analisar o fator à direita do operador
        token = lookahead(); // Verificar o próximo token
    }
}


void parseFator() {
    Token* token = lookahead();
    // Verifica se o token é uma expressão entre parênteses
    if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "(") == 0) 
    {
        
        consumirToken(); // Consumir "("
        parseExpressao(); // Processar a expressão dentro dos parênteses
        token = lookahead();
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ")") == 0) 
        {
            consumirToken(); // Consumir ")"
        }else {
            erroSintatico(token);  // Erro: Esperado ")"
        }
    } else if (token && token->tipo == TOKEN_PALAVRA_RESERVADA && strcmp(token->lexema, "nao") == 0) 
    {
        consumirToken(); // Consumir "NAO"
        parseFator(); // Processar o fator após "NAO"
    } else if (token && (token->tipo == TOKEN_NUM_INTEIRO || 
                         token->tipo == TOKEN_NUM_REAL || 
                         (token->tipo == TOKEN_PALAVRA_RESERVADA && 
                          (strcmp(token->lexema, "verdadeiro") == 0 || 
                           strcmp(token->lexema, "falso") == 0)) || 
                         token->tipo == TOKEN_STRING)) 
    {
        consumirToken(); // Consumir número, verdadeiro, falso, string
        
    } else if (token && token->tipo == TOKEN_IDENTIFICADOR) 
    {
        consumirToken(); // Consumir identificador
        token = lookahead();
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "(") == 0) {
            consumirToken(); // Consumir "("
            parseExprIter(); // Processar expressões iteráveis
            token = lookahead();

            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ")") == 0) {
                consumirToken(); // Consumir ")"
            } else {
                erroSintatico(token);  // Erro: Esperado ")"
            }
        }else if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "[") == 0)
        {
            consumirToken(); // Consumir "["
            parseExprIter(); // Processar a expressão iterável dentro dos colchetes
            token = lookahead();
            
            // Verifica se o próximo token é um "]"
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "]") == 0) {
                consumirToken(); // Consumir "]"
                token = lookahead();
            } else {
                erroSintatico(token);  // Erro: Esperado "]"
            }
        }
    }
     else if (token && token->tipo == TOKEN_IDENTIFICADOR) {
        parseVariavel(); // Processar variável
    } else {

        Token *tokenAux = lookahead(); consumirToken(); Token *token2 = lookahead(); tokenAux->coluna = token2->coluna; tokenAux->linha = token2->linha; erroSintatico(tokenAux); 
    }
}


void parseVariavel() {
    Token* token = lookahead();
    
    // Verifica se o token é um identificador e consome-o
    if (token && token->tipo == TOKEN_IDENTIFICADOR) {
        consumirToken(); // Consumir identificador
        token = lookahead();
        
        // Verifica se o próximo token é um "["
        if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "[") == 0) {
            consumirToken(); // Consumir "["
            parseExprIter(); // Processar a expressão iterável dentro dos colchetes
            token = lookahead();
            
            // Verifica se o próximo token é um "]"
            if (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, "]") == 0) {
                consumirToken(); // Consumir "]"
            } else {
                erroSintatico(token);  // Erro: Esperado "]"
            }
        }
    } else {
        erroSintatico(token);  // Erro: Esperado identificador
    }
}


void parseExprIter() {
    parseExpressao(); // Processa a primeira expressão iterável
    
    Token* token = lookahead();
    // Verifica se há mais expressões iteráveis separadas por ","
    while (token && token->tipo == TOKEN_DELIMITADOR && strcmp(token->lexema, ",") == 0) {
        consumirToken(); // Consumir ","
        parseExpressao(); // Processar a expressão iterável adicional
        token = lookahead();
    }
}



// Função para iniciar a análise sintática
void inicializaSintatico(Token* tokensArray, size_t count) {
    tokens = tokensArray;
    numTokens = count;
    index = 0;
 
    parsePrograma();
}
