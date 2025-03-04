%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>  // Inclusão do cabeçalho para funções variáveis

int line_number = 1;
int column_number = 1;
int inside_comment = 0;  // Flag para indicar se estamos dentro de um comentário multi-linha

void print_error(const char *error_char, int line, int col);
void print_warning(int line, int col);
void buffered_print(const char *format, ...);
void remove_newline(char *str);
int check_nested_comment(const char *comment, int lines);

char output_buffer[16384];  // Buffer para armazenar a saída
int buffer_length = 0;     // Comprimento atual do buffer

%}

%option noyywrap

%%

"void"                          { buffered_print("VOID\n"); column_number += yyleng; }
"int"                           { buffered_print("INT\n"); column_number += yyleng; }
"char"                          { buffered_print("CHAR\n"); column_number += yyleng; }
"return"                        { buffered_print("RETURN\n"); column_number += yyleng; }
"break"                         { buffered_print("BREAK\n"); column_number += yyleng; }
"switch"                        { buffered_print("SWITCH\n"); column_number += yyleng; }
"case"                          { buffered_print("CASE\n"); column_number += yyleng; }
"default"                       { buffered_print("DEFAULT\n"); column_number += yyleng; }
"do"                            { buffered_print("DO\n"); column_number += yyleng; }
"while"                         { buffered_print("WHILE\n"); column_number += yyleng; }
"for"                           { buffered_print("FOR\n"); column_number += yyleng; }
"if"                            { buffered_print("IF\n"); column_number += yyleng; }
"else"                          { buffered_print("ELSE\n"); column_number += yyleng; }
"typedef"                       { buffered_print("TYPEDEF\n"); column_number += yyleng; }
"struct"                        { buffered_print("STRUCT\n"); column_number += yyleng; }
"+"                             { buffered_print("PLUS\n"); column_number += yyleng; }
"-"                             { buffered_print("MINUS\n"); column_number += yyleng; }
"*"                             { buffered_print("MULTIPLY\n"); column_number += yyleng; }
"/"                             { buffered_print("DIV\n"); column_number += yyleng; }
"%"                             { buffered_print("REMAINDER\n"); column_number += yyleng; }
"++"                            { buffered_print("INC\n"); column_number += yyleng; }
"--"                            { buffered_print("DEC\n"); column_number += yyleng; }
"&"                             { buffered_print("BITWISE_AND\n"); column_number += yyleng; }
"|"                             { buffered_print("BITWISE_OR\n"); column_number += yyleng; }
"~"                             { buffered_print("BITWISE_NOT\n"); column_number += yyleng; }
"^"                             { buffered_print("BITWISE_XOR\n"); column_number += yyleng; }
"!"                             { buffered_print("NOT\n"); column_number += yyleng; }
"&&"                            { buffered_print("LOGICAL_AND\n"); column_number += yyleng; }
"||"                            { buffered_print("LOGICAL_OR\n"); column_number += yyleng; }
"=="                            { buffered_print("EQUAL\n"); column_number += yyleng; }
"!="                            { buffered_print("NOT_EQUAL\n"); column_number += yyleng; }
"<"                             { buffered_print("LESS_THAN\n"); column_number += yyleng; }
">"                             { buffered_print("GREATER_THAN\n"); column_number += yyleng; }
"<="                            { buffered_print("LESS_EQUAL\n"); column_number += yyleng; }
">="                            { buffered_print("GREATER_EQUAL\n"); column_number += yyleng; }
">>"                            { buffered_print("R_SHIFT\n"); column_number += yyleng; }
"<<"                            { buffered_print("L_SHIFT\n"); column_number += yyleng; }
"="                             { buffered_print("ASSIGN\n"); column_number += yyleng; }
"+="                            { buffered_print("ADD_ASSIGN\n"); column_number += yyleng; }
"-="                            { buffered_print("MINUS_ASSIGN\n"); column_number += yyleng; }
";"                             { buffered_print("SEMICOLON\n"); column_number += yyleng; }
","                             { buffered_print("COMMA\n"); column_number += yyleng; }
":"                             { buffered_print("COLON\n"); column_number += yyleng; }
"("                             { buffered_print("L_PAREN\n"); column_number += yyleng; }
")"                             { buffered_print("R_PAREN\n"); column_number += yyleng; }
"{"                             { buffered_print("L_CURLY_BRACKET\n"); column_number += yyleng; }
"}"                             { buffered_print("R_CURLY_BRACKET\n"); column_number += yyleng; }
"["                             { buffered_print("L_SQUARE_BRACKET\n"); column_number += yyleng; }
"]"                             { buffered_print("R_SQUARE_BRACKET\n"); column_number += yyleng; }
"?"                             { buffered_print("TERNARY_CONDITIONAL\n"); column_number += yyleng; }
"#"                             { buffered_print("NUMBER_SIGN\n"); column_number += yyleng; }
"->"                            { buffered_print("POINTER\n"); column_number += yyleng; }
"printf"                        { buffered_print("PRINTF\n"); column_number += yyleng; }
"scanf"                         { buffered_print("SCANF\n"); column_number += yyleng; }
"define"                        { buffered_print("DEFINE\n"); column_number += yyleng; }
"exit"                          { buffered_print("EXIT\n"); column_number += yyleng; }

\'(\\.|[^\'\\\n])\'             {
                                    char buffer[2];  // Buffer para armazenar o caractere processado
                                    char *ptr = yytext + 1;  // Pular o apóstrofo inicial
                                    if (*ptr == '\\') {
                                        ptr++;
                                        switch (*ptr) {
                                            case 'a': buffered_print("CHARACTER(\\a)\n"); break;
                                            case 'b': buffered_print("CHARACTER(\\b)\n"); break;
                                            case 'f': buffered_print("CHARACTER(\\f)\n"); break;
                                            case 'n': buffered_print("CHARACTER(\\n)\n"); break;
                                            case 'r': buffered_print("CHARACTER(\\r)\n"); break;
                                            case 't': buffered_print("CHARACTER(\\t)\n"); break;
                                            case 'v': buffered_print("CHARACTER(\\v)\n"); break;
                                            case '\\': buffered_print("CHARACTER(\\\\)\n"); break;
                                            case '\'': buffered_print("CHARACTER(\\')\n"); break;
                                            case '\"': buffered_print("CHARACTER(\\\")\n"); break;
                                            case '?': buffered_print("CHARACTER(\\?)\n"); break;
                                            case '0': buffered_print("CHARACTER(\\0)\n"); break;
                                            default: buffer[0] = *ptr; buffer[1] = '\0'; buffered_print("CHARACTER(%c)\n", buffer[0]); break;
                                        }
                                    } else {
                                        buffered_print("CHARACTER(%c)\n", *ptr);
                                    }
                                    column_number += yyleng;
                                }
[a-zA-Z_][a-zA-Z0-9_]{0,254}    { buffered_print("IDENTIFIER(%s)\n", yytext); column_number += yyleng; }
[a-zA-Z_][a-zA-Z0-9_]{255,}     { print_error("identifier too long", line_number, column_number); column_number += yyleng; }

0[1-7][0-7]*                    { buffered_print("NUM_OCTAL(%s)\n", yytext); column_number += yyleng; } // Números octais
0[0-9]*                         { buffered_print("NUM_INTEGER(%s)\n", yytext); column_number += yyleng; } // Números inteiros começando com 0
[1-9][0-9]*                     { buffered_print("NUM_INTEGER(%s)\n", yytext); column_number += yyleng; } // Números inteiros
0[xX][0-9a-fA-F]+               { buffered_print("NUM_HEXA(%s)\n", yytext); column_number += yyleng; } // Números hexadecimais
\"([^\"\\\n]|\\.)*\"            {
    				   char buffer[1024];
                                  // Remove as aspas duplas das extremidades e processa a string
                                  snprintf(buffer, sizeof(buffer), "%s", yytext + 1);
                                  buffer[strlen(buffer) - 1] = '\0'; // Remove a última aspas dupla
                                  remove_newline(buffer);
                                  buffered_print("STRING(%s)\n", buffer);
                                  column_number += yyleng;
                                }


\/\/[^\n]*                      { column_number += yyleng; } // Ignorar single-line comments
\/\*([^*]|\*+[^/*])*\*+\/       { line_number = check_nested_comment(yytext, line_number); column_number += yyleng;  } // Ignore multi-line comments

\/\*                            { 
                                  buffered_print("error:lexical:%d:%d: unterminated comment", line_number, column_number); // Imprime o buffer de saída
                                  // Remove qualquer '\n' no final do buffer antes de imprimir
                                  remove_newline(output_buffer);
                                  if (buffer_length > 0) 
                                  {
                                       printf("%s", output_buffer);
                                  }
                                  exit(EXIT_FAILURE); // Finaliza o programa com erro
                                }

\n                              { line_number++; column_number = 1; }
[ \t]+                          { column_number += yyleng; }
.                               { print_error(yytext, line_number, column_number); column_number += yyleng; }

%%

void print_error(const char *error_char, int line, int col) {
    buffered_print("error:lexical:%d:%d: %s\n", line, col, error_char);
}


void buffered_print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    int written = vsnprintf(output_buffer + buffer_length, sizeof(output_buffer) - buffer_length, format, args);
    if (written > 0) {
        buffer_length += written;
    }
    va_end(args);
    if (buffer_length > sizeof(output_buffer) - 1) {
        fprintf(stderr, "Buffer overflow\n");
        exit(EXIT_FAILURE);
    }
}

// Remove o caractere '\n' do final da string:
void remove_newline(char *str) {
    int length = strlen(str);
    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';  
    }
}


int check_nested_comment(const char *comment, int lines) {
    const char *ptr = comment;
    int cols = 1; 
    
    // Encontrar o primeiro "/*"
    ptr = strstr(ptr, "/*");
    // Avançar o ponteiro para além do primeiro "/*"
    ptr += 2;

    // Percorrer o comentário caractere por caractere
    while (*ptr) {
        // Verifica se encontramos o início de um comentário de bloco
        if (ptr[0] == '/' && ptr[1] == '*') {
            if (ptr[0] == '/' && ptr[1] == '*' && ptr[2] == '/') {
                ptr += 3; // Avançar o ponteiro para além de "/*/"
                cols += 3;
                continue; // Ignorar a checagem para este trecho
            } else {
                // Encontrou "/*", avisa
                buffered_print("warning:%d:%d: '/*' within block comment\n", lines, cols);
                ptr += 2; // Avançar o ponteiro para além de "/*"
                cols += 2;
                continue;
            }
        }

        cols++;

        if (*ptr == '\n') {
            lines++;
            cols = 1; 
        }

        ptr++;
    }
    
    return lines;
}




int main(int argc, char **argv) {
    yylex();
    // Remove qualquer '\n' no final do buffer antes de imprimir
    remove_newline(output_buffer);
    if (buffer_length > 0) {
        printf("%s", output_buffer);
    }
    return 0;
}

