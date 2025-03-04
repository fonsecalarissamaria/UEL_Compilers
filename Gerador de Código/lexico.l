%option noyywrap

%{
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "syntax_tree.h"
    #include "sintatico.tab.h"
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
    int line = 1;
    int line_comment_error = 0;
    int column_comment_error = 0;
    int i, j = 0;

    char currentLineBuffer[tam];
    char bufferAccumulator[tam];
    char *string;
    char tokenAccumulator[tam];
    char tokenAccumulator2[tam];

    void buffered_print();
    
%}

%%
"+"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(PLUS);}
"-"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(MINUS);}
"*"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(MULTIPLY);}
"/"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(DIV);}
"%"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(REMAINDER);}
"++"                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(INC);}
"--"                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(DEC);} 
"&"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(BITWISE_AND);}
"|"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(BITWISE_OR);}
"~"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(BITWISE_NOT);}
"^"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(BITWISE_XOR);}
"!"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(NOT);}
"&&"                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(LOGICAL_AND);}
"||"                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(LOGICAL_OR);}
"=="                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(EQUAL);}
"!="                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(NOT_EQUAL);}
"<"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(LESS_THAN);}
">"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(GREATER_THAN);}
"<="                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(LESS_EQUAL);}
">="                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(GREATER_EQUAL);}
">>"                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(R_SHIFT);}
"<<"                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(L_SHIFT);}
"="                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(ASSIGN);}
"+="                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(ADD_ASSIGN);}
"-="                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(MINUS_ASSIGN);}
";"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(SEMICOLON);}
","                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(COMMA);}
":"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(COLON);}
"("                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(L_PARENTESE);}
")"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(R_PARENTESE);}
"{"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(L_CURLY_BRACKET);}
"}"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(R_CURLY_BRACKET);}
"["                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(L_SQUARE_BRACKET);}
"]"                         {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(R_SQUARE_BRACKET);}
"RETURN_TYPE"               {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(RETURN_TYPE);}
"DO_WHILE"                  {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(DO_WHILE);}
"GLOBAL"                    {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(GLOBAL);}
"SCANF"                     {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(SCANF);}
"char"                      {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(CHAR);}
"IF"                        {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(IF);}
"FOR"                       {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(FOR);}
"VARIABLE"                  {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(VARIABLE);}
"PRINTF"                    {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(PRINTF);}
"VALUE"                     {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(VALUE);}
"EXIT"                      {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(EXIT);}
"FUNCTION"                  {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(FUNCTION);}
"AST -->"                   {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(AST);} //
"END_FUNCTION"              {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(END_FUNCTION);}
"RETURN"                    {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(RETURN);}
"WHILE"                     {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(WHILE);}
"int"                       {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(INT);}
"PARAMETER"                 {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(PARAMETER);}
"void"                      {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(VOID);}
"TYPE"                      {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(TYPE);}
"CONSTANT"                  {buffered_print();strcat(tokenAccumulator, yytext);column += strlen(yytext);return(CONSTANT);}

["]([^"\\\n]|\\.|\\\n)*["]  {   
    buffered_print();
    yylval.id_value = strdup(yytext);
    currentToken++;
    column += strlen(yytext);  
    return(STRING);
}
                            
[']"\\"?.[']                 {
    buffered_print();
    yylval.char_value = atof(yytext); 
    column += strlen(yytext);  
    return(CHARACTER);
}

[0-9]+                      {
    buffered_print();
    buffered_print(); yylval.int_num = atof(yytext); 
    column += strlen(yytext);  
    return(NUM_INTEGER);
}

[_a-zA-Z]+[_a-zA-Z0-9]*     {   
    buffered_print();
    yylval.id_value = strdup(yytext);
    column += strlen(yytext);  
    return(IDENTIFIER);
}

[ ]+                        {
    buffered_print();
    column += strlen(yytext);
} 

[\n]                        {
    buffered_print();
    line += 1; column = 1;
} 

[\t]+                       {
    buffered_print();
}

<<EOF>>                     {
    buffered_print();
    return(END_OF_FILE);
}

.                           {   
    printf("error:lexical:%d:%d: %s", line, column, yytext);
    column += strlen(yytext);
    exit(0);
}

%%


void buffered_print() {
    
    int i = 0;
    char buffer[2]; // Para armazenar um caractere

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

}
 