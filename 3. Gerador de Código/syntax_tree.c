#include "syntax_tree.h"
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <locale.h>
#include <sys/stat.h>
#include <assert.h>
#include <complex.h>
#include <sys/socket.h>
#include <math.h>
#include "sintatico.tab.h"
#include <string.h>
#include <stdio.h>
#include "mips_program.h"
int printAccumulation = 0;
#define MAX_GLOBAL_VARS 100  
int variableTracker = 0;
int countGlobalVar = 0;  


typedef struct quantumNode 
{
    int essenceType;
    ARBITER_NODE *entityReference;
    struct quantumNode *subsequentNode;
}QUANTUM_NODE;

typedef struct cosmicData 
{
    int dimensionType;
    ARBITER_NODE *entityIdentifier;
    int matrixExtent;
    struct cosmicData *nextEntity;
}COSMIC_DATA;

typedef struct valueSlot 
{
    int dataKind;
    char *entityName;
}VALUE_SLOT;
VALUE_SLOT slots[8];
VALUE_SLOT globalRegisters[MAX_GLOBAL_VARS];  

typedef struct actionNode 
{
    ARBITER_NODE *operation;
    struct actionNode *nextAction;
}ACTION_NODE;

typedef struct resourceAlloc ResourceAlloc;
struct resourceAlloc {
    char resourceType; 
    int resourceNum; 
};

typedef struct actionSet 
{
    int actionType;
    ARBITER_NODE *expressionCommand;  
    ARBITER_NODE *printCommand;
    ARBITER_NODE *scanCommand;
    ARBITER_NODE *returnCommand;

    struct actionSet *ifCommand;     
    struct actionSet *elseCommand;   
    struct actionSet *returnAction; 

    ARBITER_NODE *forInitCommand;  
    ARBITER_NODE *forIncrementCommand;   
    struct actionSet *forLoopBody; 

    struct actionSet *nextAction;  
} ACTION_SET; 

typedef struct procedureDetails 
{
    int resultType;
    ARBITER_NODE *procName;
    ARBITER_NODE *resultExpr;
    QUANTUM_NODE *paramNodes;
    COSMIC_DATA *localVars;
    ACTION_SET *operationList;
    struct procedureDetails *nextProc;
} PROCEDURE_DETAILS;
int slot_flags[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
typedef struct argument 
{
    ARBITER_NODE *expr;         
    struct argument *next;   
} ARGUMENT;



int searchSlotByName(char *name) 
{
    int index = -1;

    // Iterando sobre os slots
    for (int i = 0; i < 8; i++) 
    {
        // Verifica se o nome corresponde
        if (strcmp(slots[i].entityName, name) == 0) 
        {
            index = i;
            break;  // Encontra o índice e sai do loop
        }
    }

    return index;  // Retorna o índice ou -1 se não encontrado
}


// Função que encontra o primeiro slot livre
int findFreeSlotIndex() {
    for (int i = 0; i < 10; i++) {
        if (isSlotAvailable(i)) {
            return i;  // Retorna o índice do slot livre encontrado
        }
    }
    return -1;  // Retorna -1 caso não haja slots livres
}

// Função que encontra o primeiro slot livre e o marca como usado
int findFirstFreeSlot() {
    int slotIndex = findFreeSlotIndex();  // Encontra o primeiro slot livre
    if (slotIndex != -1) {
        occupySlot(slotIndex);  // Marca o slot como usado
    }
    return slotIndex;  // Retorna o índice do slot, ou -1 se não encontrado
}


int isSlotAvailable(int index) {
    return slot_flags[index] == 0;
}

void occupySlot(int index) {
    slot_flags[index] = 1;
}


void _setRegisterOccupied(int i){
    slot_flags[i] = 1;
}

void _deallocateRegister(int i){
    slot_flags[i] = 0;
}

// Declarações de funções antes de seu uso
ResourceAlloc assignRegister1(ResourceAlloc output, int registerIndex);
ResourceAlloc assignSpecialResource1(ResourceAlloc output);

// Função principal
ResourceAlloc selectRegisterType(ResourceAlloc output, int nextAvailableSlot) {
    if (isValidRegister(nextAvailableSlot)) {
        output = assignRegister1(output, nextAvailableSlot);
    } else {
        output = assignSpecialResource1(output);
    }
    return output;
}

// Função para verificar se o registro é válido
int isValidRegister(int registerIndex) {
    return registerIndex != -1;
}

// Definição da função assignRegister1
ResourceAlloc assignRegister1(ResourceAlloc output, int registerIndex) {
    output.resourceNum = registerIndex;
    output.resourceType = 't';  // Tipo 't' para registro
    return output;
}

// Definição da função assignSpecialResource1
ResourceAlloc assignSpecialResource1(ResourceAlloc output) {
    output.resourceType = 's';  // Tipo 's' para recurso especial
    output.resourceNum = variableTracker + 1;  // Atribuindo um número baseado no tracker
    return output;
}

ResourceAlloc allocateRegister(ResourceAlloc *output, char type) {
    int nextAvailableSlot = findFirstFreeSlot();
    if (nextAvailableSlot) {
        _setRegisterOccupied(nextAvailableSlot);
    }
    output->resourceNum = nextAvailableSlot;
    output->resourceType = type;
    return *output;
}

ResourceAlloc getRegisterForExpression(ResourceAlloc rightResource, ResourceAlloc leftResource, ResourceAlloc *output) {
    int nextAvailableSlot;
    // Verificando as condições de tipo de registrador
    if (leftResource.resourceType == 's' && rightResource.resourceType == 's') {
        // Aloca um novo registrador
        nextAvailableSlot = findFirstFreeSlot();
        if (nextAvailableSlot) {
            _setRegisterOccupied(nextAvailableSlot);
        }
        output->resourceNum = nextAvailableSlot;
        output->resourceType = 't';
    } else if (leftResource.resourceType == 't' && rightResource.resourceType == 't') {
        output->resourceNum = leftResource.resourceNum;
        output->resourceType = leftResource.resourceType;
        _deallocateRegister(rightResource.resourceNum);
    } else if (rightResource.resourceType == 't') {
        output->resourceNum = rightResource.resourceNum;
        output->resourceType = rightResource.resourceType;
    }
    return *output;
}


void insertMipsOperation(MIPS_ARCHITECTURE *example, ResourceAlloc rightResource, ResourceAlloc leftResource, ResourceAlloc output, int operation_type) {
    switch (operation_type) {
        case 0:
            insertEqualityCheckCommand(example->instruction_flow, rightResource.resourceType, rightResource.resourceNum, leftResource.resourceType, leftResource.resourceNum, output.resourceType, output.resourceNum);
            break;
        case 1:
            insertAssignOp(example->instruction_flow, leftResource.resourceType, leftResource.resourceNum, rightResource.resourceType, rightResource.resourceNum);
            break;
        case 2:
            insertComparisonLessThanCommand(example->instruction_flow, leftResource.resourceType, leftResource.resourceNum, rightResource.resourceType, rightResource.resourceNum, output.resourceType, output.resourceNum);
            break;
        case 3:
            insertAdditiveCommand(example->instruction_flow, rightResource.resourceType, rightResource.resourceNum, leftResource.resourceType, leftResource.resourceNum, output.resourceType, output.resourceNum);
            break;
        case 4:
            insertLogicalAndCommand(example->instruction_flow, rightResource.resourceType, rightResource.resourceNum, leftResource.resourceType, leftResource.resourceNum, output.resourceType, output.resourceNum);
            break;
        case 5:
            insertLogicalOrCommand(example->instruction_flow, rightResource.resourceType, rightResource.resourceNum, leftResource.resourceType, leftResource.resourceNum, output.resourceType, output.resourceNum);
            break;
    }
}


ResourceAlloc handleIdentifier(ARBITER_NODE *expr_node) {
    ResourceAlloc result = {0}; // Inicializa a estrutura
    int slot_index = searchSlotByName(expr_node->text_data);
    
    if (slot_index != -1) {
        result.resourceNum = slot_index;
        result.resourceType = 's'; // Ou 't', conforme o caso
    } else {
        int available_register = findFirstFreeSlot();
        if (available_register) {
            _setRegisterOccupied(available_register);
        }
        result.resourceNum = available_register;
        result.resourceType = 't';
    }
    
    return result;
}



ResourceAlloc compileExpression(MIPS_ARCHITECTURE *prog_example, ARBITER_NODE *expr_node) {
    ResourceAlloc result = {0}; // Inicializando a estrutura de resultado
    ResourceAlloc right_res, left_res;
    if (expr_node) {
        switch (expr_node->kind) {
            case LESS_THAN:
                right_res = compileExpression(prog_example, expr_node->right_child_node);
                left_res = compileExpression(prog_example, expr_node->left_child_node);
                result = getRegisterForExpression(right_res, left_res, &result);
                insertMipsOperation(prog_example, right_res, left_res, result, 2);
                break;
            case IDENTIFIER:
                result = handleIdentifier(expr_node);  
                break;
            case PLUS:
                right_res = compileExpression(prog_example, expr_node->right_child_node);
                left_res = compileExpression(prog_example, expr_node->left_child_node);
                result = getRegisterForExpression(right_res, left_res, &result);
                insertMipsOperation(prog_example, right_res, left_res, result, 3);
                break;
            case LOGICAL_AND:
                right_res = compileExpression(prog_example, expr_node->right_child_node);
                left_res = compileExpression(prog_example, expr_node->left_child_node);
                result = getRegisterForExpression(right_res, left_res, &result);
                insertMipsOperation(prog_example, right_res, left_res, result, 4);
                break;
            case LOGICAL_OR:
                right_res = compileExpression(prog_example, expr_node->right_child_node);
                left_res = compileExpression(prog_example, expr_node->left_child_node);
                result = getRegisterForExpression(right_res, left_res, &result);
                insertMipsOperation(prog_example, right_res, left_res, result, 5);
                break;
            case ASSIGN:
                right_res = compileExpression(prog_example, expr_node->right_child_node);
                left_res = compileExpression(prog_example, expr_node->left_child_node);
                result = getRegisterForExpression(right_res, left_res, &result);
                insertMipsOperation(prog_example, right_res, left_res, result, 1);
                break;
            case EQUAL:
                right_res = compileExpression(prog_example, expr_node->right_child_node);
                left_res = compileExpression(prog_example, expr_node->left_child_node);
                result = getRegisterForExpression(right_res, left_res, &result);
                insertMipsOperation(prog_example, right_res, left_res, result, 0);
                break;
            
            default:
                result = allocateRegister(&result, 't');
                break;
        }
    }
    return result;
}

void handlePrintf(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives) {
    formatPrintfCommand(prog_example, "msg_", cmd_directives->expressionCommand->text_data, printAccumulation);
    printAccumulation++;
}

void handleScanf(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives, int *scanf_count) {
    insertScanfInputCommand(prog_example->instruction_flow, *scanf_count);
    (*scanf_count)++;
}

void handleIf(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives, int lbl_num) {
    ResourceAlloc temp_reg = compileExpression(prog_example, cmd_directives->expressionCommand);
    insertConditionalOperation(prog_example->instruction_flow, temp_reg.resourceType, temp_reg.resourceNum, lbl_num);
    insertMarker(prog_example->instruction_flow, "Then_", lbl_num);
    _addCommandsCode(prog_example, cmd_directives->ifCommand, lbl_num + 1);
    insertFunctionInvocationMarker(prog_example->instruction_flow, "\tj Exit_IF_", lbl_num);
    if (cmd_directives->elseCommand) {
        insertMarker(prog_example->instruction_flow, "Else_", lbl_num);
        _addCommandsCode(prog_example, cmd_directives->elseCommand, lbl_num + 1);
    }
    insertMarker(prog_example->instruction_flow, "Exit_IF_", lbl_num);
}


void handleReturn(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives) {
    ResourceAlloc temp_reg = compileExpression(prog_example, cmd_directives->expressionCommand);
    insertMoveOp(prog_example->instruction_flow, 't', temp_reg.resourceNum, 'v', 0);
    insertReturnOp(prog_example->instruction_flow);
}

void handleAssign(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives) {
    ResourceAlloc temp_reg = compileExpression(prog_example, cmd_directives->expressionCommand);
    insertAssignOp(prog_example->instruction_flow, 't', temp_reg.resourceNum, 't', temp_reg.resourceNum);
}

void handleExit(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives) {
    insertHaltSignalCode(prog_example->instruction_flow, cmd_directives->expressionCommand->integer_data);
}


void _addCommandsCode(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives, int lbl_num) {
    int scanf_count = 0;

    while (cmd_directives != NULL) {
        switch (cmd_directives->actionType) {
            case PRINTF:
                handlePrintf(prog_example, cmd_directives);
                break;

            case SCANF:
                handleScanf(prog_example, cmd_directives, &scanf_count);
                break;

            case IF:
                //printf("Entrooooooooooooooooooooooooooou\n\n");
                handleIf(prog_example, cmd_directives, lbl_num);
                break;

            case RETURN:
                handleReturn(prog_example, cmd_directives);
                return;  // Exit after handling RETURN

            case ASSIGN:
                handleAssign(prog_example, cmd_directives);
                break;

            case PARAMETER: 
                handleParameter(prog_example, cmd_directives, lbl_num);
                break;

            case EXIT:
                handleExit(prog_example, cmd_directives);
                break;

            default:
                printf(" #Esse comando está errado\n");
                break;
        }
        
        cmd_directives = cmd_directives->nextAction;
    }
}


void freeExpression(ARBITER_NODE *arbiter_node) {
    if (arbiter_node != NULL) {
        free(arbiter_node->text_data);  
        free(arbiter_node);       // Liberar a estrutura ARBITER_NODE
    }
}

void freeCommand(ACTION_SET *action_set) {
    if (action_set != NULL) {
        freeExpression(action_set->expressionCommand);
        freeExpression(action_set->printCommand);
        freeExpression(action_set->scanCommand);
        freeExpression(action_set->returnCommand);
        free(action_set);  // Liberar a estrutura ACTION_SET
    }
}


void freeFunction(PROCEDURE_DETAILS *cosmic_routine) {
    if (cosmic_routine != NULL) {
        freeExpression(cosmic_routine->procName);
        freeExpression(cosmic_routine->resultExpr);
        
        // Liberar parâmetros
        QUANTUM_NODE *param_node = cosmic_routine->paramNodes;
        while (param_node != NULL) {
            QUANTUM_NODE *temp_node = param_node;
            param_node = param_node->subsequentNode;
            freeExpression(temp_node->entityReference);
            free(temp_node);
        }

        // Liberar variáveis
        COSMIC_DATA *local_var = cosmic_routine->localVars;
        while (local_var != NULL) {
            COSMIC_DATA *temp_var = local_var;
            local_var = local_var->nextEntity;
            freeExpression(temp_var->entityIdentifier);
            free(temp_var);
        }

        // Liberar comandos
        ACTION_SET *action_step = cosmic_routine->operationList;
        while (action_step != NULL) {
            ACTION_SET *temp_action = action_step;
            action_step = action_step->nextAction;
            freeCommand(temp_action);
        }

        // Liberar a própria função
        free(cosmic_routine);
    }
}

void displayAST(PROCEDURE_DETAILS *cosmic_tree) {
    MIPS_ARCHITECTURE *prog_example = beginProgram();
    PROCEDURE_DETAILS *current_routine = cosmic_tree;
    
    while (current_routine != NULL) {
        ARBITER_NODE *expr_name = (ARBITER_NODE *) current_routine->procName;
        ACTION_SET *action_steps = current_routine->operationList;

        // Inserir função no programa MIPS
        defineNewFunction(prog_example->instruction_flow, expr_name->text_data);

        // Adicionar código dos comandos da função
        _addCommandsCode(prog_example, action_steps, 0);

        // Imprimir o programa MIPS
        insertHaltSignal(prog_example->instruction_flow);
        constructMipsBlueprint(prog_example);

        // Avançar para a próxima função na lista
        current_routine = current_routine->nextProc;
    }
}



PROCEDURE_DETAILS *allocateProcedureDetails() {
    PROCEDURE_DETAILS *cosmic_routine = (PROCEDURE_DETAILS *) malloc(sizeof(PROCEDURE_DETAILS));
    
    if (cosmic_routine == NULL) {
        printf("Erro: falha ao alocar memória para PROCEDURE_DETAILS\n");
    }
    
    return cosmic_routine;
}


void setProcedureDetails(PROCEDURE_DETAILS *cosmic_routine, ARBITER_NODE *routine_name, int return_type, 
                         ARBITER_NODE *return_expr, QUANTUM_NODE *param_nodes, 
                         COSMIC_DATA *local_vars, ACTION_SET *action_steps, PROCEDURE_DETAILS *next_routine) {
    cosmic_routine->procName = routine_name;
    cosmic_routine->resultType = return_type;
    cosmic_routine->paramNodes = param_nodes;
    cosmic_routine->localVars = local_vars;
    cosmic_routine->resultExpr = return_expr;
    cosmic_routine->operationList = action_steps;
    cosmic_routine->nextProc = next_routine;
}

PROCEDURE_DETAILS *startOperation(ARBITER_NODE *routine_name, int return_type, ARBITER_NODE *return_expr, 
                                   QUANTUM_NODE *param_nodes, COSMIC_DATA *local_vars, ACTION_SET *action_steps, 
                                   PROCEDURE_DETAILS *next_routine) {
    PROCEDURE_DETAILS *cosmic_routine = allocateProcedureDetails();

    if (cosmic_routine != NULL) {
        setProcedureDetails(cosmic_routine, routine_name, return_type, return_expr, param_nodes, local_vars, action_steps, next_routine);
    }

    return cosmic_routine;
}



ARBITER_NODE *allocateExpressionNode() {
    ARBITER_NODE *celestial_expression = (ARBITER_NODE *) malloc(sizeof(ARBITER_NODE));
    return celestial_expression;
}

void setIdentifierOrStringData(ARBITER_NODE *celestial_expression, char *string_data) {
    char *temp = (char*) malloc(sizeof(char) * (strlen(string_data) + 4));
    sprintf(temp, "%s%c", string_data, '\0');
    celestial_expression->text_data = temp;
}

void setNumericOrReturnData(ARBITER_NODE *celestial_expression, float float_value) {
    celestial_expression->decimal_data = float_value;
}

ARBITER_NODE *createExpressionTree(int resource_category, int numeric_value, char *string_data, 
                                   ARBITER_NODE *left_orbit, ARBITER_NODE *right_orbit, 
                                   float float_value) {
    ARBITER_NODE *celestial_expression = allocateExpressionNode();

    celestial_expression->kind = resource_category;
    celestial_expression->right_child_node = right_orbit;
    celestial_expression->left_child_node = left_orbit;

    if (resource_category == IDENTIFIER || resource_category == STRING) {
        setIdentifierOrStringData(celestial_expression, string_data);
    } else if (resource_category == NUM_INTEGER || resource_category == RETURN) {
        setNumericOrReturnData(celestial_expression, float_value);
    }

    return celestial_expression;
}


QUANTUM_NODE *allocateNewParameter() {
    QUANTUM_NODE *nova_variavel = (QUANTUM_NODE *) malloc(sizeof(QUANTUM_NODE));
    return nova_variavel;
}

QUANTUM_NODE *addNewParameter(ARBITER_NODE *identificador, int recursoCategoria, QUANTUM_NODE *proximo_nodo) {
    QUANTUM_NODE *nova_variavel = allocateNewParameter();
    
    nova_variavel->entityReference = identificador;
    nova_variavel->essenceType = recursoCategoria;
    nova_variavel->subsequentNode = proximo_nodo;
    
    return nova_variavel;
}

void registerNewVariable(ARBITER_NODE *identificador, int tipo_recurso) {
    VALUE_SLOT novo_registro;
    if(identificador->kind == IDENTIFIER) {
        novo_registro.entityName = identificador->text_data;
        novo_registro.dataKind = tipo_recurso;
        slots[variableTracker] = novo_registro;
        variableTracker++;
    }
}

COSMIC_DATA *insertVariable(ARBITER_NODE *identificador, int tipo_recurso, COSMIC_DATA *proximo_entidade) {
    COSMIC_DATA *nova_variavel = (COSMIC_DATA *) malloc(sizeof(COSMIC_DATA));
    
    // Registrar a variável se for um identificador
    registerNewVariable(identificador, tipo_recurso);
    
    nova_variavel->entityIdentifier = identificador;
    nova_variavel->dimensionType = tipo_recurso;
    nova_variavel->nextEntity = proximo_entidade;
    
    return nova_variavel;
}



void setupIfCommand(ACTION_SET *comando_novo, ARBITER_NODE *condicao, ACTION_SET *comando_true, ACTION_SET *comando_false) {
    comando_novo->expressionCommand = condicao;
    comando_novo->ifCommand = comando_true;
    comando_novo->elseCommand = comando_false;
}

void setupExpressionCommand(ACTION_SET *comando_novo, ARBITER_NODE *condicao, ARBITER_NODE *expressao_2, int tipo_recurso) {
    comando_novo->expressionCommand = condicao;
    switch (tipo_recurso) {
        case PRINTF:
            comando_novo->printCommand = expressao_2;
            break;
        case SCANF:
            comando_novo->scanCommand = expressao_2;
            break;
        case RETURN:
            comando_novo->returnCommand = expressao_2;
            break;
        default:
            break;
    }
}


ACTION_SET *generateCommand(int tipo_recurso, ARBITER_NODE *condicao, ACTION_SET *comando_true, ACTION_SET *comando_false, ARBITER_NODE *expressao_2) {
    ACTION_SET *comando_novo = (ACTION_SET *)malloc(sizeof(ACTION_SET));
    comando_novo->actionType = tipo_recurso;

    switch (tipo_recurso) {
        case IF:
            setupIfCommand(comando_novo, condicao, comando_true, comando_false);
            break;
        case PRINTF:
        case SCANF:
        case RETURN:
            setupExpressionCommand(comando_novo, condicao, expressao_2, tipo_recurso);
            break;
        default:
            break;
    }

    return comando_novo;
}


// Função que vincula o comando atual ao próximo comando
ACTION_SET *linkCommands(ACTION_SET *comando_atual, ACTION_SET *comando_proximo) {
    comando_atual->nextAction = comando_proximo;  // Associa o próximo comando ao comando atual
    return comando_atual;  // Retorna o comando atual com o link atualizado
}


ARBITER_NODE *createNewConstant(char *name, int value) {
    ARBITER_NODE *new_constant = (ARBITER_NODE *) malloc(sizeof(ARBITER_NODE));
    new_constant->kind = CONSTANT;  // Defina o tipo como CONSTANT (você pode ter esse tipo já definido em algum lugar)
    new_constant->decimal_data = (float)value;  // Armazene o valor da constante
    new_constant->text_data = (char*) malloc(sizeof(char)*(strlen(name)+1));  // Aloca memória para o nome da constante
    strcpy(new_constant->text_data, name);  // Copia o nome da constante para a string
    new_constant->left_child_node = NULL;  // Não tem filhos
    new_constant->right_child_node = NULL;  // Não tem filhos
    return new_constant;
}

COSMIC_DATA *createNewGlobalVariable(ARBITER_NODE *id, int resourceType, COSMIC_DATA *next) {
    COSMIC_DATA *new_var = (COSMIC_DATA *) malloc(sizeof(COSMIC_DATA));
    new_var->entityIdentifier = id;
    new_var->dimensionType = resourceType;
    new_var->nextEntity = next;
    return new_var;
}

void setArrayProperties(COSMIC_DATA *var, int array_size) {
    if (var != NULL) {
        // Defina o tipo da variável como array ou vetor
        var->dimensionType = NULL;  // Defina um tipo para arrays, por exemplo, ARRAY_TYPE

        // A variável pode ter uma propriedade adicional para armazenar o tamanho
        var->matrixExtent = array_size;  // Aqui você define o tamanho do array
    }
}


ACTION_SET *createNewAssignment( ARBITER_NODE *target, ARBITER_NODE *value) {
    // Criar a expressão de atribuição (ex: "target = value")
    ARBITER_NODE *assignment_expr = createExpressionTree(
        ASSIGN, 0, NULL, target, value, 0
    );

    // Criar o comando de atribuição e armazená-lo na AST
    ACTION_SET *new_assignment = generateCommand(
        ASSIGN, assignment_expr, NULL, NULL, NULL
    );
    
    return new_assignment;
}



ACTION_SET *createNewWhile(ARBITER_NODE *condition, ACTION_SET *directives) {
    // Alocar memória para um novo nó de comando WHILE
    ACTION_SET *new_command = (ACTION_SET *)malloc(sizeof(ACTION_SET));

    // Definir o tipo do comando como WHILE (você pode usar um tipo específico ou um código de tipo)
    new_command->actionType = WHILE;  // Ajuste esse valor conforme necessário no seu código

    // Associar a condição ao campo expressionCommand
    new_command->expressionCommand = condition;

    // Associar os comandos a serem executados no corpo do WHILE ao campo ifCommand
    // Isso é uma adaptação, já que você não tem um campo específico para WHILE, mas pode usar o ifCommand
    new_command->ifCommand = directives;  // Usando ifCommand para armazenar os comandos

    // Atribua o próximo comando (se necessário) ou NULL se não houver
    new_command->nextAction = NULL;

    return new_command;  // Retorna o comando WHILE criado
}

ACTION_SET *createNewDoWhile(ARBITER_NODE *condition, ACTION_SET *directives) {
    // Aloca memória para o comando DO_WHILE
    ACTION_SET *new_command = (ACTION_SET *)malloc(sizeof(ACTION_SET));

    // Define o tipo do comando como DO_WHILE
    new_command->actionType = DO_WHILE;

    // Atribui a expressão de condição do DO_WHILE
    new_command->expressionCommand = condition;

    // Atribui os comandos dentro do DO_WHILE
    new_command->ifCommand = directives;

    // Define o próximo comando (se necessário)
    new_command->nextAction = NULL;

    return new_command;  // Retorna o comando DO_WHILE criado
}

ACTION_SET *createNewExit(int exit_code) {
    ACTION_SET *new_command = (ACTION_SET *) malloc(sizeof(ACTION_SET));
    new_command->actionType = EXIT;  // Defina um tipo de comando para o EXIT
    new_command->expressionCommand = createExpressionTree(NUM_INTEGER, exit_code, NULL, NULL, NULL, 0.0f); // A expressão com o código de saída
    new_command->nextAction = NULL;
    return new_command;
}

ARGUMENT *createNewArgument(ARBITER_NODE *expr, ARGUMENT *next) {
    ARGUMENT *new_argument = (ARGUMENT *) malloc(sizeof(ARGUMENT));
    new_argument->expr = expr;
    new_argument->next = next;
    return new_argument;
}


ACTION_SET *createNewForLoop(
    ARBITER_NODE *initialization,
    ARBITER_NODE *condition,
    ARBITER_NODE *increment,
    ACTION_SET *body,
    ACTION_SET *next
) {
    //printf("oiiiiiiiiiiiiiiiiii\n\n\n");
    ACTION_SET *new_for = (ACTION_SET *)malloc(sizeof(ACTION_SET));
    new_for->actionType = FOR;
    new_for->expressionCommand = condition;
    new_for->forInitCommand = initialization;
    new_for->forIncrementCommand = increment;
    new_for->forLoopBody = body;
    new_for->nextAction = next;
    return new_for;
}

void printFunctionParameters(MIPS_ARCHITECTURE *prog_example, QUANTUM_NODE *parameters) {
    //printf("Entroooooooooooooooooooooooooooou\n\n");
    QUANTUM_NODE *current_param = parameters;
    int label_number = 0;

    // Percorre todos os parâmetros
    while (current_param != NULL) {
        // Criação do código MIPS para imprimir a informação do parâmetro
        char *param_info = malloc(100);  // Aloca espaço suficiente para o texto

        // Formata o código MIPS para imprimir o parâmetro
        sprintf(param_info, "PARAMETER: %s TYPE: %d\n", 
                current_param->entityReference->text_data, 
                current_param->essenceType);

        // Criação e inserção do código no segmento de dados
        MIPS_NODE *item_data = malloc(sizeof(MIPS_NODE));
        char *data_code = createPrintfDataCode(param_info, label_number); // Similar à criação do código para printf
        item_data->assembly_code = data_code;
        appendInstruction(prog_example->static_memory, item_data);

        // Criação e inserção do código no segmento de texto
        MIPS_NODE *item_text = malloc(sizeof(MIPS_NODE));
        char *text_code = createPrintfTextCode("param", label_number);  // Utiliza um rótulo único para cada parâmetro
        item_text->assembly_code = text_code;
        appendInstruction(prog_example->instruction_flow, item_text);

        // Passa para o próximo parâmetro
        current_param = current_param->subsequentNode;
        label_number++;  // Incrementa o número do label para cada parâmetro
    }
}

void handleParameter(MIPS_ARCHITECTURE *prog_example, ACTION_SET *cmd_directives, int lbl_num) {
    // Imprime os parâmetros da função
    printFunctionParameters(prog_example, cmd_directives->expressionCommand);
}
