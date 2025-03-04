%option noyywrap

%x COMMENT_BLOCO

%{
#include <sintatico.tab.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define tam 10000

int i = 0;
int lexicalAid = 0;
int suporteSintatico = 0;
int column2 = 1;
int currentLine2 = 0;
int currentToken = 0; 
int currentLine = 1; 
int columnPosition = 0;
int column = 1; 
int increment_line = 0;

char currentLineBuffer[tam];
char bufferAccumulator[tam];
char *string;
char tokenAccumulator[tam];
char tokenAccumulator2[tam];

void buffered_print();


%}

%%

"void"     {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(VOID);}
"int"      {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(INT);}
"char"     {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(CHAR);}
"return"   {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(RETURN);}
"break"    {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(BREAK);}
"switch"   {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(SWITCH);}
"case"     {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(CASE);}
"default"  {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(DEFAULT);}
"do"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(DO);}
"while"    {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(WHILE);}
"for"      {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(FOR);}
"if"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(IF);}
"else"     {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(ELSE);}
"typedef"  {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(TYPEDEF);}
"struct"   {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(STRUCT);}
"+"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(PLUS);}
"-"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(MINUS);}
"*"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(MULTIPLY);}
"/"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(DIV);}
"%"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(REMAINDER);}
"++"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(INC);}
"--"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(DEC);}
"&"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(BITWISE_AND);}
"|"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(BITWISE_OR);}
"~"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(BITWISE_NOT);}
"^"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(BITWISE_XOR);}
"!"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(NOT);}
"&&"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(LOGICAL_AND);}
"||"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(LOGICAL_OR);}
"=="       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(EQUAL);}
"!="       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(NOT_EQUAL);}
"<"  	   {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(LESS_THAN);}
">"	   {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(GREATER_THAN);}
"<="       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(LESS_EQUAL);}
">="       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(GREATER_EQUAL);}
">>"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(R_SHIFT);}
"<<"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(L_SHIFT);}
"="        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(ASSIGN);}
"+="       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(ADD_ASSIGN);}
"-="       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(MINUS_ASSIGN);}
";"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(SEMICOLON);}
","        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(COMMA);}
":"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(COLON);}
"("        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(L_PAREN);}
")"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(R_PAREN);}
"{"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(L_CURLY_BRACKET);}
"}"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(R_CURLY_BRACKET);}
"["        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(L_SQUARE_BRACKET);}
"]"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(R_SQUARE_BRACKET);}
"?"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(TERNARY_CONDITIONAL);}
"#"        {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(NUMBER_SIGN);}
"->"       {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(POINTER);}
"printf"   {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(PRINTF);}
"scanf"    {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(SCANF);}
"define"   {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(DEFINE);}
"exit"     {buffered_print();strcat(tokenAccumulator, yytext);column+=strlen(yytext);currentToken++;return(EXIT);}



[_a-zA-Z][_a-zA-Z0-9]*   {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    column += strlen(yytext);
    currentToken++;
    return IDENTIFIER;
}

" "                      {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    column += strlen(yytext);
}

"\t"                     {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    column += strlen(yytext);
}

"//"(.)*"\n"             {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    currentLine++;
    column = 1;
    yylineno++;
}

"\n"                     {
    if (suporteSintatico == 1 && lexicalAid == 0) {
        lexicalAid = 1;
        strcpy(tokenAccumulator2, tokenAccumulator);
    }
    strcpy(tokenAccumulator, "");
    buffered_print();
    currentLine++;
    column = 1;
    yylineno++;
}

'[\\]?(.)'               {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    string = malloc(strlen(yytext) + 1);
    strcpy(string, yytext);
    for (i = 0; string[i] != '\0'; i++) {
        string[i] = string[i + 1];
    }
    string[i - 2] = '\0';
    column += strlen(yytext);
    currentToken++;
    free(string);
    return CHARACTER;
}

\"([^\\\"]|\\.)*\"       {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    string = malloc(strlen(yytext) + 1);
    strcpy(string, yytext);
    for (i = 0; string[i] != '\0'; i++) {
        string[i] = string[i + 1];
    }
    string[i - 2] = '\0';
    column += strlen(yytext);
    currentToken++;
    free(string);
    return STRING;
}

0[Xx][0-9a-fA-F]+        {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    column += strlen(yytext);
    currentToken++;
    return NUM_HEXA;
}

[0-9]+                   {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    column += strlen(yytext);
    currentToken++;
    return NUM_INTEGER;
}

0[1-7][0-7]* {
    buffered_print();
    strcat(tokenAccumulator, yytext);
    column += strlen(yytext);
    currentToken++;
    return NUM_OCTAL;
}

<INITIAL>{
    "/*"  {
        buffered_print();
        currentLine2 = currentLine;
        column2 = column;
        column += 2;
        BEGIN(COMMENT_BLOCO);
    }
}

<COMMENT_BLOCO>{
    /* Fim do comentário de bloco */
    "*/" {
        buffered_print();
        column += 2;
        BEGIN(INITIAL);
    }

    /* Aviso sobre '/*' dentro de um comentário de bloco */
    "/*" {
        printf("warning:%d:%d: '/*' within block comment\n", currentLine, (column+1));
        buffered_print();
        column += strlen(yytext);
    }

    /* Qualquer caractere que não seja '*' ou '/' e não quebre linha */
    [^*\/\n]+ {
        buffered_print();
        column += strlen(yytext);
    }

    /* Nova linha */
    \n {
        buffered_print();
        yylineno++;
        currentLine++;
        column = 0;
    }

    /* Apenas '*' */
    "*" {
        buffered_print();
        column += strlen(yytext);
    }

    /* Apenas '/' */
    "/" {
        buffered_print();
        column += strlen(yytext);
    }

    /* Fim do arquivo sem terminar o comentário */
    <<EOF>> {
        printf("error:lexical:%d:%d: unterminated comment\n", currentLine2, column2);
        exit(EXIT_FAILURE);
    }
}


\/\/([^\n])* {
    // Chama a função para processamento de buffers
    buffered_print();

    // Atualiza a posição da coluna com o comprimento do texto atual
    column += strlen(yytext);
}

. {
    // Verifica se o suporte sintático está desativado
    if (suporteSintatico == 0) {
        // Exibe a mensagem de erro formatada
        printf("error:lexical:%d:%d: %s", currentLine, column, yytext);

        // Atualiza a posição da coluna com o comprimento do texto atual
        column += strlen(yytext);

        // Incrementa o contador de linhas e encerra o programa
        increment_line++;
        exit(0);
    }
}

%%    

void buffered_print() {
    int i = 0;
    char buffer[2]; // Para armazenar um caractere

    strcat(bufferAccumulator, yytext);

    // Itera sobre cada caractere em yytext
	for (i = 0; yytext[i] != '\0'; i++) 
	{
    		char currentChar = yytext[i]; // Caractere atual para simplificar a leitura

    		switch (currentChar) 
    		{
        	case '\n':
            	// Se for uma nova linha, limpa os buffers e reinicia a posição da coluna
            	strcpy(currentLineBuffer, "");
            	strcpy(bufferAccumulator, "");
            	columnPosition = 0;
            	break;

        	case '\t':
            		// Se for um caractere de tabulação, ajusta a posição da coluna para o próximo múltiplo de 8
            		columnPosition += 8 - (columnPosition % 8);
           		 break;

        	default:
            		// Para outros caracteres, atualiza o buffer da linha atual e incrementa a posição da coluna
            		strcpy(currentLineBuffer, yytext);
            		columnPosition++;
            		break;
    		}
	}


    // Processar caracteres especiais escapados
    i = 0; // Reiniciar o índice para percorrer novamente a string
    while (yytext[i] != '\0') {
        if (yytext[i] == '\\') {
            i++; // Avançar para o próximo caractere após a barra invertida
            if (yytext[i] != '\0') {
                switch (yytext[i]) {
                    case 'a': buffered_print("CHARACTER(\\a)\n"); break;
                    case 'b': buffered_print("CHARACTER(\\b)\n"); break;
                    case 'f': buffered_print("CHARACTER(\\f)\n"); break;
                    case 'r': buffered_print("CHARACTER(\\r)\n"); break;
                    case 't': buffered_print("CHARACTER(\\t)\n"); break;
                    case 'v': buffered_print("CHARACTER(\\v)\n"); break;
                    case '\\': buffered_print("CHARACTER(\\\\)\n"); break;
                    case '\'': buffered_print("CHARACTER(\\')\n"); break;
                    case '\"': buffered_print("CHARACTER(\\\")\n"); break;
                    case '?': buffered_print("CHARACTER(\\?)\n"); break;
                    case '0': buffered_print("CHARACTER(\\0)\n"); break;
                    default: 
                        buffer[0] = yytext[i]; 
                        buffer[1] = '\0'; 
                        break;
                }
            }
        }
        i++;
    }
}
 

