%{
    #include <stdio.h>
    #include <stdlib.h>
    #include "syntax_tree.h"
    #include <string.h>

    #define size 10000
    extern char* yytext;
    extern char currentLineBuffer[size];
    extern char bufferAccumulator[size];
    extern char tokenAccumulator[size];
    extern char tokenAccumulator2[size];
    extern int suporteSintatico;
    char* get_error_line();
    PROCEDURE_DETAILS *tree = NULL;
    FILE *input;
    extern int lexicalAid;
    extern int columnPosition;
    extern int currentLine;
    extern int column;
    extern int line;
    int pointerIndex = 0; 
    extern int yylex();
    extern int yyparse();
    void yyerror(void *s);
    int dimensao;
    int numeroLinha = 0;
    char bufferAuxiliar[size];
    extern int yychar;

    
%}

%union {

    QUANTUM_NODE *ParameterChain;
    char *IdentifierName;
    COSMIC_DATA *VariableCollection;
    int NumericValue;
    ACTION_NODE *ExpressionTree;
    int IntegerValue;
    ACTION_SET *CommandSequence;    
    int int_value;
    ARBITER_NODE *EvaluationNode;
    char *id_value;
    PROCEDURE_DETAILS *FunctionCatalog;
    char char_value;
    int int_num;
    char *RawString;
}


%token AST FUNCTION RETURN BREAK SWITCH CASE DEFAULT DO_WHILE WHILE FOR IF ELSE TYPEDEF STRUCT
%token VOID INT CHAR RETURN_TYPE VARIABLE TYPE PARAMETER END_FUNCTION STRING VALUE CONSTANT END_OF_FILE

%token PLUS MINUS MULTIPLY DIV REMAINDER INC DEC CHARACTER
%token BITWISE_AND BITWISE_OR BITWISE_NOT BITWISE_XOR
%token NOT LOGICAL_AND LOGICAL_OR EQUAL NOT_EQUAL LESS_THAN GREATER_THAN GREATER_EQUAL LESS_EQUAL
%token R_SHIFT L_SHIFT ASSIGN ADD_ASSIGN MINUS_ASSIGN

%token SEMICOLON COMMA L_PARENTESE R_PARENTESE L_CURLY_BRACKET R_CURLY_BRACKET
%token L_SQUARE_BRACKET R_SQUARE_BRACKET TERNARY_CONDITIONAL NUMBER_SIGN POINTER

%token PRINTF SCANF DEFINE EXIT

%token <IntegerValue> NUM_INTEGER
%token <IdentifierName> IDENTIFIER
%token <int> COLON
%token <RawString> MAX
%token <VariableCollection> GLOBAL


%type <FunctionCatalog> START main_function sub_function 
%type <NumericValue> data_type return_type pointer_check array_validation
%type <ParameterChain> func_parameter
%type <VariableCollection> var_declaration var_definition
%type <EvaluationNode> ExpressionTree calc_expr return_expr assign_target arg_list
%type <CommandSequence> CommandSequence conditional_blocks if_block while_block loop_do_while loop_for termination_cmd
%type <CommandSequence> printf scanf
%type <IntegerValue> sign_values
%start START

%%
START: AST main_function END_OF_FILE {tree = $2; displayAST(tree); exit(0);};

main_function: { $$ = NULL;} 
    | sub_function 
    {
        $$ = $1;   
    }
    | var_definition {$$ = NULL;}
    ;

var_definition: CONSTANT COLON IDENTIFIER VALUE COLON NUM_INTEGER main_function { 
        //printf("Entrou aqui 1\n\n");
        $$ = createNewConstant($3, $6);
    }

    | GLOBAL VARIABLE COLON IDENTIFIER TYPE COLON data_type {
        //printf("Entrou aqui 2\n\n");
        pointerIndex = 0;
        //$$ = NULL;
    }  pointer_check array_validation {
        // Criação da expressão para o identificador da variável global
        ARBITER_NODE *id_expr = createExpressionTree(IDENTIFIER, 0, $4, NULL, NULL, 0);

        // Criação da variável global
        COSMIC_DATA *var = createNewGlobalVariable(id_expr, $7, NULL);

        // Se array_validation retornou um valor, é um array
        if ($9 != NULL) {
            // $10 contém o tamanho do array
            int array_size = $9;
            // Associe o tamanho do array à variável global (ajuste conforme necessário)
            setArrayProperties(var, array_size);
        }

        $1 = var;  // Retorna a variável criada
    } main_function 
    ;

pointer_check:
    MULTIPLY pointer_check {
        pointerIndex++;
    }
    | /* Ação para o caso vazio */
    { }
    ;

array_validation:
    /* Não é um array, apenas retorna NULL */
    { $$ = NULL; }

    | L_SQUARE_BRACKET NUM_INTEGER R_SQUARE_BRACKET {
        // Se é um array, capturamos o tamanho
        $$ = $2;  // Tamanho do array, que está em $2 (NUM_INTEGER)
    }
    ;

sub_function: {$$ = NULL;}
    | sub_function FUNCTION COLON calc_expr return_type func_parameter var_declaration CommandSequence return_expr END_FUNCTION 
    {
        //printf("Entrou aqui\n\n\n\n");
        //printf("Nome da função: %p\n", $4);
        //printf("Return data_type: %d\n", $5);
        //printf("Return ExpressionTree: %p\n", $9);
        //printf("Parameters: %p\n", $6);
        //printf("Variables: %p\n", $7);
        //printf("Commands: %p\n", $8);
        //printf("Next sub_function: %p\n", $1);

        $$ = startOperation($4, $5, $9, $6, $7, $8, $1); 
        //if ($$ == NULL)
        //    printf("Função é NULL\n\n");
    }
    ;

calc_expr: IDENTIFIER { $$ = createExpressionTree(IDENTIFIER, 0, yytext, NULL, NULL, 0);}
    | NUM_INTEGER {$$ = createExpressionTree(NUM_INTEGER, 0, NULL, NULL, NULL, (float)$1);}
    | CHARACTER { $$ = createExpressionTree(CHARACTER, 0, yytext, NULL, NULL, 0); }
    | L_PARENTESE ExpressionTree R_PARENTESE {$$ = $2; }
    | STRING {$$ = createExpressionTree(STRING, 0, yytext, NULL, NULL, 0);}

    | MINUS L_PARENTESE ExpressionTree R_PARENTESE { 
        $$ = createExpressionTree(MINUS, 0, NULL, $3, NULL, 0); 
    }
    | BITWISE_NOT L_PARENTESE ExpressionTree R_PARENTESE { 
        $$ = createExpressionTree(BITWISE_NOT, 0, NULL, $3, NULL, 0); 
    }
    //| function_call { $$ = $1; }

    
    /*| TILDE L_PARENTESE ExpressionTree R_PARENTESE { 
        $$ = createExpressionTree(TILDE, 0, NULL, $3, NULL, 0); 
    }*/
    /*| PLUS L_PARENTESE ExpressionTree R_PARENTESE { 
        $$ = createExpressionTree(PLUS, 0, NULL, $3, NULL, 0); 
    }*/
    /*| NOT L_PARENTESE ExpressionTree R_PARENTESE { 
        $$ = createExpressionTree(NOT, 0, NULL, $3, NULL, 0); 
    }*/
    /*| MINUS calc_expr { 
        $$ = createExpressionTree(MINUS, 0, NULL, $2, NULL, 0); 
    }
    /*| TILDE calc_expr { 
        $$ = createExpressionTree(TILDE, 0, NULL, $2, NULL, 0); 
    }*/
    ;

/*function_call: IDENTIFIER L_PARENTESE arguments R_PARENTESE 
    {  
        //$$ = startOperation(createExpressionTree(IDENTIFIER, 0, $1, NULL, NULL, 0), 0, NULL, $3, NULL, NULL, NULL)
    } 
    ; 
    
arguments: { $$ = NULL; } 
    | arg_list { $$ = $1; }
    ; 
    
arg_list: ExpressionTree { $$ = createArgumentList($1); } 
    | arg_list COMMA ExpressionTree { $$ = addArgument($1, $3); } 
    ;*/

return_type: { $$ = NULL;}
    | RETURN_TYPE COLON data_type{$$ = $3;}
    ;

return_expr: { $$ = NULL; /*printf("--- o RETURN é null\n\n");*/ }
    | RETURN L_PARENTESE ExpressionTree R_PARENTESE
    {
        //printf("--- o RETURN NÃO é null\n\n"); 
        //$$ = createExpressionTree(RETURN, 0, NULL, $3, NULL, $3->decimal_data);
        $$ = generateCommand(RETURN, $3, NULL, NULL, $3);
        
    }
    ;

func_parameter: { $$ = NULL;}
    | func_parameter PARAMETER COLON calc_expr TYPE COLON data_type {$$ = addNewParameter($4, $7, $1);}
    ;

var_declaration: {$$ = NULL;}
    | var_declaration VARIABLE COLON calc_expr TYPE COLON data_type {$$ = insertVariable($4, $7, $1);}
    ;

CommandSequence: {$$ = NULL;}
    | printf termination_cmd CommandSequence {$$ = linkCommands($1,$3);}
    | scanf termination_cmd CommandSequence {$$ = linkCommands($1,$3);}
    | if_block termination_cmd CommandSequence {$$ = linkCommands($1,$3);}
    | ASSIGN L_PARENTESE assign_target COMMA assign_target R_PARENTESE SEMICOLON termination_cmd CommandSequence {
        //printf("passou aqui???????????????? 1\n");
        $$ = linkCommands(createNewAssignment($3, $5), $8);
    }
    | ASSIGN L_PARENTESE assign_target COMMA ExpressionTree R_PARENTESE SEMICOLON termination_cmd CommandSequence {
        //printf("passou aqui?\n");
        $$ = linkCommands(createNewAssignment($3, $5), $8);
        
    }
    | ASSIGN L_PARENTESE assign_target COMMA ExpressionTree L_PARENTESE ExpressionTree R_PARENTESE R_PARENTESE SEMICOLON termination_cmd CommandSequence
    {
        //printf("passou aqui?\n");
        $$ = linkCommands(createNewAssignment($3, $5), $11);
        
    }
    /*| ASSIGN L_PARENTESE assign_target COMMA IDENTIFIER L_PARENTESE ExpressionTree R_PARENTESE R_PARENTESE SEMICOLON termination_cmd CommandSequence {
        // Criação da expressão para a função GenValue(1)
        ARBITER_NODE *function_expr = createExpressionTree(FUNCTION, 0, $4, $6, NULL, 0.0f);  
        
        // Criação da expressão para o target (v01)
        ARBITER_NODE *target_expr = $3;

        // Chamada à createNewAssignment para criar o comando de atribuição
        $$ = linkCommands(createNewAssignment(target_expr, function_expr), $9);
    }*/
    | WHILE L_PARENTESE ExpressionTree COMMA while_block R_PARENTESE SEMICOLON termination_cmd CommandSequence {
        //printf("passou aquiiii\n\n");
        $$ = linkCommands(createNewWhile($3, $5), $8); 
    }
    | EXIT L_PARENTESE ExpressionTree R_PARENTESE CommandSequence {
        //printf("Processando comando EXIT com valor %f\n", $3->decimal_data);
        $$ = linkCommands(createNewExit($3->decimal_data), $5); // Criação do comando EXIT com o valor
    }
    | IDENTIFIER L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE SEMICOLON termination_cmd CommandSequence  {
        //printf("------------1---------\n\n");
        ARBITER_NODE *function_call = createExpressionTree(FUNCTION, 0, $1, $3, NULL, 0.0f);
        $$ = linkCommands(function_call, $8);
    } 
    | IDENTIFIER L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE termination_cmd {
        //printf("-------------2--------\n\n");
        ARBITER_NODE *function_call = createExpressionTree(FUNCTION, 0, $1, $3, NULL, 0.0f);
        $$ = linkCommands(function_call, $7);
    }
    | FOR L_PARENTESE ExpressionTree COMMA ExpressionTree COMMA ExpressionTree COMMA loop_for R_PARENTESE SEMICOLON termination_cmd loop_for CommandSequence 
    {
        //printf("-------------FOR--------\n\n");
        $$ = createNewForLoop($3, $5, $7, $9, $12);
    } 
    ;



arg_list:
      ExpressionTree { $$ = createNewArgument($1, NULL); }
    | ExpressionTree COMMA arg_list { $$ = createNewArgument($1, $3); }
    | { $$ = NULL; } 
    ;

while_block:
    { $$ = NULL; }
    | loop_do_while { $$ = $1; }
    | while_block loop_do_while { $$ = linkCommands($1, $2); }
    | printf termination_cmd scanf termination_cmd {$$ = $1;}
    | scanf termination_cmd printf termination_cmd {$$ = $1;}
    | scanf termination_cmd {$$ = $1;}
    | printf termination_cmd {$$ = $1;}
    ;

loop_for:
    { $$ = NULL; }
    | printf termination_cmd scanf termination_cmd {$$ = $1;}
    | scanf termination_cmd printf termination_cmd {$$ = $1;}
    | scanf termination_cmd {$$ = $1;}
    | printf termination_cmd CommandSequence {$$ = $1;}
    ;

loop_do_while: DO_WHILE L_PARENTESE ExpressionTree COMMA CommandSequence R_PARENTESE 
    {
        $$ = createNewDoWhile($3, $5);
    }
    | DO_WHILE L_PARENTESE ExpressionTree COMMA LESS_THAN L_PARENTESE assign_target COMMA ExpressionTree R_PARENTESE R_PARENTESE termination_cmd while_block
    {
        $$ = createNewDoWhile($3, $9);
    }
    | DO_WHILE L_PARENTESE ExpressionTree COMMA GREATER_THAN L_PARENTESE assign_target COMMA ExpressionTree R_PARENTESE R_PARENTESE termination_cmd while_block
    {
        $$ = createNewDoWhile($3, $9);
    }
    | if_block {}
    ;

assign_target:
    IDENTIFIER { 
        $$ = createExpressionTree(IDENTIFIER, 0, $1, NULL, NULL, 0); 
    }
    | IDENTIFIER L_SQUARE_BRACKET ExpressionTree R_SQUARE_BRACKET { 
        // Em vez de criar um novo tipo de expressão, podemos simplesmente tratar o acesso ao índice
        $$ = createExpressionTree(IDENTIFIER, 0, $1, $3, NULL, 0); 
    }
    | calc_expr {}
    ;


if_block: IF L_PARENTESE ExpressionTree COMMA conditional_blocks COMMA conditional_blocks R_PARENTESE {$$ = generateCommand(IF, $3, $5, $7, NULL); /*printf("caiu no if 1\n\n");*/}
    | IF L_PARENTESE ExpressionTree COMMA conditional_blocks R_PARENTESE {$$ = generateCommand(IF, $3, $5, NULL, NULL); /*printf("Caiu no if 2\n\n");*/}
    | IF L_PARENTESE ExpressionTree COMMA ExpressionTree COMMA ExpressionTree COMMA assign_target R_PARENTESE {$$ = generateCommand(IF, $3, $5, NULL, NULL); }
    | IF L_PARENTESE ExpressionTree COMMA ExpressionTree COMMA ExpressionTree SEMICOLON ExpressionTree SEMICOLON ExpressionTree R_PARENTESE {/*printf("Caiu no if 3\n\n");*/ $$ = generateCommand(IF, $3, $5, NULL, NULL); }
    //| IF L_PARENTESE ExpressionTree COMMA ExpressionTree COMMA ExpressionTree SEMICOLON ASSIGN L_PARENTESE assign_target COMMA assign_target R_PARENTESE SEMICOLON ASSIGN L_PARENTESE assign_target COMMA ExpressionTree R_PARENTESE R_PARENTESE {printf("Caiu no if 3\n\n"); $$ = generateCommand(IF, $3, $5, NULL, NULL); }
    //| IF L_PARENTESE ExpressionTree COMMA conditional_blocks COMMA conditional_blocks R_PARENTESE {printf("caiu no if 3\n\n"); $$ = generateCommand(IF, $3, $5, $7, NULL);}
    ;


conditional_blocks: {$$ = NULL;}
    | if_block {$$ = $1;}
    | printf {$$ = $1;}
    | scanf {$$ = $1;}
    | return_expr {$$ = $1;}
    ;

scanf: SCANF L_PARENTESE calc_expr COMMA BITWISE_AND L_PARENTESE calc_expr R_PARENTESE R_PARENTESE termination_cmd {$$ = generateCommand(SCANF, $3,NULL, NULL,$7);} 
    
    | SCANF L_PARENTESE calc_expr COMMA BITWISE_AND L_PARENTESE calc_expr R_PARENTESE R_PARENTESE termination_cmd ASSIGN L_PARENTESE assign_target COMMA assign_target R_PARENTESE termination_cmd 
    {
        printf("Entrou no scanf 2\n\n");
        ACTION_SET *scanf_cmd = generateCommand(SCANF, $3, NULL, NULL, $7);
        ACTION_SET *assign_cmd = generateCommand(ASSIGN, $13, NULL, NULL, $15);
        $$ = linkCommands(scanf_cmd, assign_cmd);
    }
   
    ;


printf: PRINTF L_PARENTESE calc_expr COMMA ExpressionTree R_PARENTESE {$$ = generateCommand(PRINTF, $3,NULL, NULL,$5);}
    | PRINTF L_PARENTESE calc_expr R_PARENTESE  CommandSequence {/*printf("o print caiu aqui <---------------------\n\n");*/ $$ = generateCommand(PRINTF, $3, NULL, NULL,NULL);}
    | PRINTF L_PARENTESE calc_expr COMMA ExpressionTree COMMA ExpressionTree R_PARENTESE
    {
        $$ = generateCommand(PRINTF, $3, NULL, NULL, $7); 
    }
    | PRINTF L_PARENTESE calc_expr COMMA ExpressionTree COMMA ExpressionTree L_PARENTESE ExpressionTree R_PARENTESE R_PARENTESE
    {
        $$ = generateCommand(PRINTF, $3, NULL, NULL, $9); 
    }
    | PRINTF L_PARENTESE calc_expr COMMA assign_target R_PARENTESE R_PARENTESE
    {
        $$ = generateCommand(PRINTF, $3, NULL, NULL, $5); 
    }
    ;

termination_cmd: {}
    | SEMICOLON {}
    | COMMA {}
    ;

ExpressionTree: 
    { $$ = NULL; }
    | LESS_EQUAL L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(LESS_EQUAL, 0, NULL, $3, $5, 0); }
    | ASSIGN L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(ASSIGN, 0, NULL, $3, $5, 0); }
    | ASSIGN L_PARENTESE ExpressionTree COMMA assign_target R_PARENTESE 
        { $$ = createExpressionTree(ASSIGN, 0, NULL, $3, $5, 0); }
    | EQUAL L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(EQUAL, 0, NULL, $3, $5, 0); }
    | NOT_EQUAL L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(NOT_EQUAL, 0, NULL, $3, $5, 0); }
    | LOGICAL_AND L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(LOGICAL_AND, 0, NULL, $3, $5, 0); }
    | LOGICAL_OR L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(LOGICAL_OR, 0, NULL, $3, $5, 0); }
    | MULTIPLY L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(MULTIPLY, 0, NULL, $3, $5, 0); }
    | MULTIPLY L_PARENTESE ExpressionTree COMMA ExpressionTree L_PARENTESE sign_values L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE R_PARENTESE R_PARENTESE 
        { $$ = createExpressionTree(MULTIPLY, 0, NULL, $3, $5, 0.0f); }
    | DIV L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(DIV, 0, NULL, $3, $5, 0); }
    | R_SHIFT L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(R_SHIFT, 0, NULL, $3, $5, 0); }
    | calc_expr 
        { $$ = $1; }
    | BITWISE_OR L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(BITWISE_OR, 0, NULL, $3, $5, 0); }
    | BITWISE_XOR L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(BITWISE_XOR, 0, NULL, $3, $5, 0); }
    | BITWISE_NOT L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(BITWISE_NOT, 0, NULL, $3, $5, 0); }
    | PLUS L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(PLUS, 0, NULL, $3, $5, 0); }
    | MINUS L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(MINUS, 0, NULL, $3, $5, 0); }
    | GREATER_THAN L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(GREATER_THAN, 0, NULL, $3, $5, 0); }
    | GREATER_EQUAL L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(GREATER_EQUAL, 0, NULL, $3, $5, 0); }
    | LESS_THAN L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(LESS_THAN, 0, NULL, $3, $5, 0); }
    | L_PARENTESE ExpressionTree R_PARENTESE INC 
        { $$ = createExpressionTree(INC, 0, NULL, $2, NULL, 0); }
    | L_PARENTESE ExpressionTree R_PARENTESE DEC 
        { $$ = createExpressionTree(DEC, 0, NULL, $2, NULL, 0); }
    | L_SHIFT L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(L_SHIFT, 0, NULL, $3, $5, 0); }
    | REMAINDER L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(REMAINDER, 0, NULL, $3, $5, 0); }
    | BITWISE_AND L_PARENTESE ExpressionTree COMMA ExpressionTree R_PARENTESE 
        { $$ = createExpressionTree(BITWISE_AND, 0, NULL, $3, $5, 0); }
    ;


data_type: VOID 
    {
        $$ = VOID; 
    }
    | CHAR 
    {
        $$ = CHAR; 
    }
    | INT 
    {
        $$ = INT; 
    }
    ;

sign_values: MINUS 
    { 
        $$ = MINUS; 
    }
    |  PLUS 
    { 
        $$ = PLUS;
    } 
    | { $$ = PLUS; } ;

%%
// Inicializa a entrada do analisador sintático
void initialize_parser() {
    input = stdin;
    yyparse();
}

// Lê uma linha específica do arquivo sem alocação dinâmica
const char* fetch_line_from_file(int target_line) {
    static char buffer[1024] = {0};  
    rewind(input);

    for (int i = 0; i < target_line; i++) {
        if (!fgets(buffer, sizeof(buffer), input)) {
            return NULL;
        }
    }
    return buffer;
}

// Exibe a mensagem de erro e destaca a posição
void display_syntax_error(const char *message, int error_column) {
    printf("error:syntax:%d:%d: %s\n", line, error_column, message);

    const char* error_line = fetch_line_from_file(line);
    if (error_line) {
        printf("%s", error_line);
        if (!strchr(error_line, '\n')) {
            printf("\n");
        }
    }

    for (int i = 1; i < error_column; i++) {
        printf(" ");
    }
    printf("^\n");
}

// Manipulador de erro
void yyerror(void *s) {
    int error_column = (yychar == END_OF_FILE) ? column : column - (int)strlen(yytext);
    const char *message = (yychar == END_OF_FILE) ? 
        "unexpected end of file: missing statement or declaration" : yytext;

    display_syntax_error(message, error_column);
}

// Função principal
int main(int argc, char** argv) {
    initialize_parser();
    return 0;
}
