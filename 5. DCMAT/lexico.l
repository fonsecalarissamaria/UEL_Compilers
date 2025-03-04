%option noyywrap
%{

#include <sintatico.tab.h>
#include<string.h>
void limpar_buffer_atual();

char* strdup(const char* str) {
    size_t len = strlen(str) + 1;
    char* copy = (char*)malloc(len);
    if (copy) {
        memcpy(copy, str, len);
    }
    return copy;
}


%}
%%

[0-9]+"."[0-9]+ { yylval.real = atof(yytext);return REAL; }
[0-9]+          { yylval.integer = atoi(yytext); return INT;  }
[\r\t ]+          { /* Ignorar espaços e tabs */ }
\n                  { return EOL; limpar_buffer_atual(); }
"+"                 { return PLUS; }
"-"                 { return MINUS; } 
"*"                 { return TIMES; }
"/"                 { return DIVIDE; }
"^"                 { return POWER; }
"%"                 { return MOD; } 
"("                 { return LPAREN; }
")"                 { return RPAREN; }
":"                 { return COLON; }
"="                 { return EQUALS; }
":="                { return ASSIGN; }
"["                 { return LBRACKET; }
"]"                 { return RBRACKET; }
";"                 { return SEMICOLON; }
","                 { return COMMA; }
"quit"              { return QUIT; }
"about"             { return ABOUT; }
"plot"              { return PLOT; }
"integrate"         { return INTEGRATE; }
[pP][iI]            { return PI; }
[Ee]                { return E; }
"float"             { return FLOAT; }
"abs"               { return ABS; }
"h_view"            { return HVIEW; }
"show"              { return SHOW; }
"settings"          { return SETTINGS; }
"axis"              { return AXIS; }
"reset"             { return RESET; }
"integral_steps"    { return INTEGRAL_STEPS; }
"precision"         { return PRECISION; }
"solve"             { return SOLVE; }
"connect"           { return CONNECT; }
"dots"              { return DOTS; }
"sum"               { return SUM; }
"determinant"       { return DETERMINANT; }
"matrix"            { return MATRIX; }
"rpn"               { return RPN; }
"tan"               { return TAN; }
"symbols"           { return SYMBOLS;}
"off"               { return OFF; }
"sen"               { return SEN; }
"cos"               { return COS; } 
"v_view"            { return VVIEW; }
"erase"             { return ERASE; }
"on"                { return ON; }
"set"               { return SET; }
"x"                 { return X; }
"linear_system"    { return LINEAR_SYSTEM; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENTIFIER; }
.                   { printf("Invalid Symbol: %s\n", yytext); limpar_buffer_atual();  return ERROR; }


%%

// Função que limpa o conteúdo do buffer atual
void limpar_buffer_atual() {
    // Obtendo o buffer atual
    YY_BUFFER_STATE buffer_atual = YY_CURRENT_BUFFER;
    
    // Verifica se o buffer não está vazio
    if (buffer_atual != NULL) {
        // Chama a função que efetivamente limpa o buffer
        yy_flush_buffer(buffer_atual);
    } else {
        // Caso o buffer seja NULL, imprimir uma mensagem de erro
        fprintf(stderr, "Erro: Não há buffer atual para limpar.\n");
    }
}
