%{

#include <stdio.h>
#include<string.h>
#include<stdbool.h>
#include"funcoes.h"
#include"tree.h"
#include"matriz.h"
#include "symbol_table.h"
#include <stdlib.h> 

extern int yylex();
extern char* yytext;
extern void limpar_buffer_atual();
void yyerror(char *s);
void resetAuxValues();
extern char* strdup(const char* str);

extern int quit;
extern void (*function_to_be_called)(void);
#define MAX_SIZE 2
extern float aux_float[MAX_SIZE];
extern int aux_int[MAX_SIZE];
extern void* aux_ptr;
int sum_lo, sum_hi;
extern int error_flag;

%}

%union{
	int integer;
	float real;
	struct ASTNode *ASTNode;
	void* mat;
	int ival;
    double dval;
    const char* sval;
    float fval;
    float* y_values;
    char var_name;
}

%token <fval> FLOAT
%token <sval> IDENTIFIER 
%token SUM 
%token PI E
%token PRECISION CONNECT DOTS RPN
%token ERASE SYMBOLS
%token ASSIGN


%token PLUS MINUS TIMES DIVIDE POWER MOD
%token SEN COS TAN ABS
%token LPAREN RPAREN
%token INT REAL X
%token SHOW RESET SETTINGS QUIT
%token SET HVIEW VVIEW AXIS
%token ON OFF
%token PLOT INTEGRAL_STEPS INTEGRATE
%token MATRIX SOLVE DETERMINANT LINEAR_SYSTEM
%token ABOUT
%token SEMICOLON COLON EQUALS
%token RBRACKET LBRACKET COMMA
%token ERROR EOL

%type <real> REAL
%type <real> floating_point
%type <real> numeric_value
%type <real> matrix_element
%type <integer> integer_value
%type <integer> X
%type <integer> INT
%type <integer> command_type
%type <ASTNode> mathematical_expression rpn_expression
%type <ASTNode> exp2_type
%type <ASTNode> exp3_type
%type <ASTNode> exp4_type
%type <ASTNode> exp5_type
%type <ASTNode> term_type
%type <ASTNode> matrix_value_structure
%type <ASTNode> re_matrix_structure
%type <mat> re_matrix_instance
%type <ASTNode> identifier_regra

%start start

%%
start: mathematical_expression EOL  
    {  
        if(verifica_is_matriz($1) == 1)
        {
            operacoes_matriz($1);
        }else
        {
            float result = evaluateExpression($1);  // Avalia a expressão
            if(error_flag != 1)
                printf("\n%.6f\n\n", result);    // Imprime o resultado
            ASTNodeDelete($1);                         // Libera memória da árvore
        }
        return 0; 
    }
    | rpn_expression SEMICOLON EOL 
    { 
        printf("\nExpression in RPN format:\n\n");
        ASTNodeRpnWalk($1);  // Chama a função ASTNodeRpnWalk para imprimir a expressão em RPN
        printf("\n");
        ASTNodeDelete($1);  // Libera a memória da árvore de sintaxe após o uso
        return 0; 
    }
    | command_type SEMICOLON EOL 
    { 
        function_to_be_called = $1; 
        return 0; 
    }
    | QUIT EOL 
    { 
        quit = true; 
        exit(0);
        return 1; 
    }
    | // erro genérico
    {
        yyerrok;  // Restaura o parser após o erro
    }
;

rpn_expression: 
    RPN LPAREN mathematical_expression RPAREN
    {
        $$ = $3;  // A expressão matemática dentro dos parênteses
    }
;

command_type: SHOW SETTINGS  
    { 
        $$ = show_Settings; 
    }
    | RESET SETTINGS  
    { 
        $$ = reset_Settings; 
    }
    | SET HVIEW numeric_value COLON numeric_value 
    { 
        $$ = setHorizontalView; 
        aux_float[0] = $3; 
        aux_float[1] = $5; 
    }
    | SET VVIEW numeric_value COLON numeric_value 
    { 
        $$ = setVerticalView; 
        aux_float[0] = $3; 
        aux_float[1] = $5; 
    }
    | SET ERASE PLOT OFF  
    { 
        erase_plot = 0; printf("Erase plot off: novo gráfico não apagará o anterior.\n"); 
        $$ = set_erase;
    }
    | SET ERASE PLOT ON    
    { 
        erase_plot = 1; printf("Erase plot on: gráfico será apagado ao plotar novo gráfico.\n"); 
        $$ = set_erase;
    }
    | SET AXIS ON  
    { 
        $$ = enableAxis; 
    }
    | SET AXIS OFF  
    { 
        $$ = disableAxis; 
    }
    | PLOT  
    { 
        $$ = plotGraph; 
        aux_ptr = NULL; 
    }
    | PLOT LPAREN mathematical_expression RPAREN  
    { 
        $$ = plotGraph; 
        aux_ptr = $3; 
    }
    | SET INTEGRAL_STEPS integer_value  
    { 
        $$ = setIntegralSteps; 
        aux_int[0] = $3; 
    }
    | INTEGRATE LPAREN numeric_value COLON numeric_value COMMA mathematical_expression RPAREN  
    { 
        $$ = integrateFunction; 
        aux_float[0] = $3; 
        aux_float[1] = $5; 
        aux_ptr = $7; 
    }
    | SUM LPAREN identifier_regra COMMA integer_value COLON integer_value COMMA mathematical_expression RPAREN SEMICOLON EOL 
    { 
        $$ = $9; 
        addSymbol($3->name, $5);         
        aux_float[0] = $5; 
        aux_float[1] = $7; 
        aux_ptr = $9;
        summation($9, $5, $7);
        ASTNodeDelete($9);  // Libera a memória da árvore de sintaxe após o uso
        $$ = set_erase;
        return 0;
    }
    | MATRIX EQUALS re_matrix_structure  
    { 
        $$ = matrixOperations; 
        aux_ptr = $3; 
    }
    | SHOW MATRIX  
    { 
        $$ = displayMatrix; 
    }
    | SOLVE DETERMINANT  
    { 
        $$ = solveDeterminant; 
    }
    | SOLVE LINEAR_SYSTEM  
    { 
        $$ = solveLinearSystem; 
    }
    | ABOUT  
    { 
        $$ = aboutInfo; 
    }
    | identifier_regra ASSIGN mathematical_expression 
    { 
        // Verifica se o identificador é válido
        if ($1->name == NULL) {
            fprintf(stderr, "Erro: identificador inválido!\n");
            YYABORT;
        }
        
        float value = evaluateExpression($3);
        addSymbol($1->name, value);          
    }
    | identifier_regra ASSIGN re_matrix_structure
    {
        if ($1->name == NULL) {
            fprintf(stderr, "Erro: identificador inválido!\n");
            YYABORT;
        }
        
        printf("\n");
        addMatrixSymbol($1->name, $3); 
    }
    | SHOW SYMBOLS
    {
        showSymbols();
        $$ = set_erase;
    }
    | identifier_regra 
    {
        getSymbolValue($1->name);
        //printf("Chegou aqui?\n");
        $$ = set_erase;
    }
    | SET FLOAT PRECISION INT
    {
        setFloatPrecision($4);
        $$ = set_erase; 
    }
    | SET FLOAT PRECISION MINUS INT
    {
        printf("\nERROR: float precision must be from 0 to 8\n\n");
        $$ = set_erase; 
    }
    
;

numeric_value: integer_value  
    { 
        $$ = $1; 
    }
    | floating_point 
    { 
        $$ = $1; 
    }
;

integer_value: PLUS INT  
    { 
        $$ = $2; 
    }
    | MINUS INT  
    { 
        $$ = $2 * (-1); 
    }
    | INT 
    { 
        $$ = $1; 
    }
;

floating_point: PLUS REAL  
    { 
        $$ = $2; 
    }
    | MINUS REAL  
    { 
        $$ = $2 * (-1); 
    }
    | REAL  
    { 
        $$ = $1; 
    }
;

re_matrix_structure: LBRACKET LBRACKET matrix_element matrix_value_structure RBRACKET re_matrix_instance RBRACKET  
    { 
        $$ = criarEstruturaMatriz(createNode(REAL, $3, $6, $4, NULL)); 
    }
;

re_matrix_instance: COMMA LBRACKET matrix_element matrix_value_structure RBRACKET re_matrix_instance  
    { 
        $$ = createNode(REAL, $3, $6, $4, NULL); 
    }
    | /* epsilon */  
    { 
        $$ = NULL; 
    }
;

matrix_value_structure: COMMA matrix_element matrix_value_structure  
    { 
        $$ = createNode(REAL, $2, NULL, $3, NULL); 
    }
    | /* epsilon */  
    { 
        $$ = NULL; 
    }
;

matrix_element: numeric_value  
    { 
        $$ = $1; 
    }
;

mathematical_expression: exp2_type  
    { 
        $$ = $1; 
    }
    | mathematical_expression PLUS exp2_type  
    { 
        $$ = createNode(PLUS, 0.0, $1, $3, NULL); 
    }
    | mathematical_expression MINUS exp2_type  
    { 
        $$ = createNode(MINUS, 0.0, $1, $3, NULL); 
    }
;

exp2_type: exp3_type  
    { 
        $$ = $1; 
    }
    | exp2_type TIMES exp3_type  
    { 
        $$ = createNode(TIMES, 0.0, $1, $3, NULL); 
    }
    | exp2_type DIVIDE exp3_type  
    { 
        $$ = createNode(DIVIDE, 0.0, $1, $3, NULL); 
    }
    | exp2_type MOD exp3_type  
    { 
        $$ = createNode(MOD, 0.0, $1, $3, NULL); 
    }
;

exp3_type: exp4_type  
    { 
        $$ = $1; 
    }
    | exp3_type POWER exp4_type  
    { 
        $$ = createNode(POWER, 0.0, $1, $3, NULL); 
    }
    | PLUS exp4_type  
    { 
        $$ = createNode(PLUS, 0.0, createNode(INT, 0.0, NULL, NULL, NULL), $2, NULL); 
    }
    | MINUS exp4_type  
    { 
        $$ = createNode(MINUS, 0.0, createNode(INT, 0.0, NULL, NULL, NULL), $2, NULL); 
    }
;

exp4_type: exp5_type  
    { 
        $$ = $1; 
    }
    | SEN LPAREN mathematical_expression RPAREN  
    { 
        $$ = createNode(SEN, 0.0, $3, NULL, NULL); 
    }
    | COS LPAREN mathematical_expression RPAREN  
    { 
        $$ = createNode(COS, 0.0, $3, NULL, NULL); 
    }
    | TAN LPAREN mathematical_expression RPAREN  
    { 
        $$ = createNode(TAN, 0.0, $3, NULL, NULL); 
    }
    | ABS LPAREN mathematical_expression RPAREN  
    { 
        $$ = createNode(ABS, 0.0, $3, NULL, NULL); 
    }
;

exp5_type: term_type  
    { 
        $$ = $1; 
    }
    | LPAREN mathematical_expression RPAREN  
    { 
        $$ = $2; 
    }
;

term_type: INT  
    { 
        $$ = createNode(INT, $1, NULL, NULL, NULL); 
    }
    | REAL  
    { 
        $$ = createNode(REAL, $1, NULL, NULL, NULL); 
    }
    | X  
    { 
        $$ = createNode(X, 0.0, NULL, NULL, NULL); 
    }
    | PI
    {
        $$ = createNode(PI, 3.14159265, NULL, NULL, NULL);
    }
    | ASSIGN 
    {
        $$ = createNode(ASSIGN, 0.0, NULL, NULL, ":=");
    }
    | identifier_regra
    { 
               
    }
;

identifier_regra: IDENTIFIER
    { 
        
        $1 = strdup(yytext);
        char quoted[1024];
        strcpy(quoted, yytext);
        $$ = createNode(IDENTIFIER, 0.0, NULL, NULL, quoted);        
    }
;

%%



void yyerror(char *s)
{
    // Se yytext for NULL ou contiver apenas um '\n', não faz nada
    if (yytext != NULL && strcmp(yytext, "\n") == 0)
    {
        return; // Ignora erros de linha vazia
    }else if (strcmp(yytext, "\n") == 0)
    {
        // Ignora erros de linha vazia
    }else if (yytext != NULL && strcmp(yytext, "") != 0)
    {
        printf("SYNTAX ERROR: [%s]\n", *yytext == '\n' ? "\\n" : yytext);
    }else
    {
        printf("SYNTAX ERROR: Incomplete Command\n");
    }
    resetAuxValues();
    limpar_buffer_atual();
}