#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>
#include "mips_program.h"
#include <sys/stat.h>
#include <assert.h>
#include <complex.h>
#include <sys/socket.h>
#include <math.h>
#include <stdarg.h>

MIPS_NODE* allocateMipsNode() {
    return (MIPS_NODE*) malloc(sizeof(MIPS_NODE));
}

char* allocateDataSegmentLabel() {
    char *data_label = (char*) malloc(sizeof(char) * 10);
    strcpy(data_label, ".data\n");
    return data_label;
}

MIPS_NODE* initializeDataSegment() {
    MIPS_NODE *data_node = allocateMipsNode();
    data_node->assembly_code = allocateDataSegmentLabel();
    data_node->next_instruction = NULL;
    return data_node;
}


MIPS_NODE* allocateTextSegmentNode() {
    return (MIPS_NODE*) malloc(sizeof(MIPS_NODE));
}

char* allocateTextSegmentLabel() {
    char *text_label = (char*) malloc(sizeof(char) * 10);
    strcpy(text_label, ".text\n");
    return text_label;
}

MIPS_NODE* initializeTextSegment() {
    MIPS_NODE *text_node = allocateTextSegmentNode();
    text_node->assembly_code = allocateTextSegmentLabel();
    text_node->next_instruction = NULL;
    return text_node;
}


MIPS_ARCHITECTURE* allocateMipsProgram() {
    return (MIPS_ARCHITECTURE*) malloc(sizeof(MIPS_ARCHITECTURE));
}

void initializeProgramSegments(MIPS_ARCHITECTURE *codeBase) {
    codeBase->static_memory = initializeDataSegment();
    codeBase->instruction_flow = initializeTextSegment();
}

MIPS_ARCHITECTURE* beginProgram() {
    MIPS_ARCHITECTURE *codeBase = allocateMipsProgram();
    initializeProgramSegments(codeBase);
    return codeBase;
}


MIPS_NODE* findLastInstruction(MIPS_NODE *instruction_chain) {
    MIPS_NODE *intermediate = instruction_chain;
    while (intermediate->next_instruction != NULL) {
        intermediate = intermediate->next_instruction; 
    }
    return intermediate;
}

void linkInstruction(MIPS_NODE *last_instruction, MIPS_NODE *new_instruction) {
    last_instruction->next_instruction = new_instruction;
}

MIPS_NODE* appendInstruction(MIPS_NODE *instruction_chain, MIPS_NODE *new_instruction) {
    MIPS_NODE *last_instruction = findLastInstruction(instruction_chain);
    linkInstruction(last_instruction, new_instruction);
    return instruction_chain;
}


void printInstructions(MIPS_NODE *instruction_chain) {
    MIPS_NODE *intermediate = instruction_chain;
    while (intermediate != NULL) {
        printf("%s", intermediate->assembly_code);
        intermediate = intermediate->next_instruction;
    }
}

void constructMipsBlueprint(MIPS_ARCHITECTURE *codeBase) {
    printInstructions(codeBase->static_memory);  // Imprime o segmento de dados
    printInstructions(codeBase->instruction_flow);  // Imprime o segmento de texto
}

char* createAdditiveCommand(char source_reg, int source_num, char target_reg, int target_num, char result_reg, int result_num) {
    char *appendData = malloc(sizeof(char) * 25);
    sprintf(appendData, "\tadd $%c%d, $%c%d, $%c%d\n", result_reg, result_num, source_reg, source_num, target_reg, target_num);
    return appendData;
}

void insertAdditiveCommand(MIPS_NODE *instruction_list, char source_reg, int source_num, char target_reg, int target_num, char result_reg, int result_num) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));
    entry->assembly_code = createAdditiveCommand(source_reg, source_num, target_reg, target_num, result_reg, result_num);
    appendInstruction(instruction_list, entry);
}


char* createSubtractionCommand(char source_reg, int source_num, char target_reg, int target_num, char result_reg, int result_num) {
    char *sub_data = malloc(sizeof(char) * 25);
    sprintf(sub_data, "\tsub $%c%d, $%c%d, $%c%d\n", result_reg, result_num, source_reg, source_num, target_reg, target_num);
    return sub_data;
}

void insertSubtractionOperation(MIPS_NODE *instruction_list, char source_reg, int source_num, char target_reg, int target_num, char result_reg, int result_num) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));
    entry->assembly_code = createSubtractionCommand(source_reg, source_num, target_reg, target_num, result_reg, result_num);
    appendInstruction(instruction_list, entry);
}


// Função auxiliar para calcular a multiplicação
char* createMultiplicationCommand(char operand_one_reg, int operand_one_num, char operand_two_reg, int operand_two_num, char result_reg, int result_num) {
    char *mul_data = malloc(sizeof(char) * 30);  // Aumentando o tamanho para garantir que caiba o comando
    sprintf(mul_data, "\tmul $%c%d, $%c%d, $%c%d\n", result_reg, result_num, operand_one_reg, operand_one_num, operand_two_reg, operand_two_num);
    return mul_data;
}

// Função principal para inserir a operação de multiplicação
void insertMultiplicationOperation(MIPS_NODE *instruction_list, char operand_one_reg, int operand_one_num, char operand_two_reg, int operand_two_num, char result_reg, int result_num) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));
    entry->assembly_code = createMultiplicationCommand(operand_one_reg, operand_one_num, operand_two_reg, operand_two_num, result_reg, result_num);
    appendInstruction(instruction_list, entry);
}


// Função auxiliar para criar a operação de divisão
char* createDivisionCommand(char divisor_reg, int divisor_num, char dividend_reg, int dividend_num) {
    char *div_data = malloc(sizeof(char) * 50);  // Aumentando o tamanho para garantir que caiba o comando
    sprintf(div_data, "\tdiv $%c%d, $%c%d\n", divisor_reg, divisor_num, dividend_reg, dividend_num);
    return div_data;
}

// Função auxiliar para adicionar a instrução do resto (mfhi)
char* appendRemainderCommand(char* div_data, char remainder_reg, int remainder_num) {
    char *remainder_data = malloc(sizeof(char) * 30);  // Aumentando o tamanho para garantir que caiba o comando
    sprintf(remainder_data, "%s \tmfhi $%c%d", div_data, remainder_reg, remainder_num);
    return remainder_data;
}

// Função auxiliar para adicionar a instrução do quociente (mflo)
char* appendQuotientCommand(char* div_data, char quotient_reg, int quotient_num) {
    char *quotient_data = malloc(sizeof(char) * 30);  // Aumentando o tamanho para garantir que caiba o comando
    sprintf(quotient_data, "%s \tmflo $%c%d", div_data, quotient_reg, quotient_num);
    return quotient_data;
}

// Função principal para inserir a operação de divisão
void insertDivisionOperationWithRemainder(MIPS_NODE *instruction_list, char divisor_reg, int divisor_num, char dividend_reg, int dividend_num, char result_reg, int result_num, int includeRemainder) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Criando o comando de divisão
    char *div_data = createDivisionCommand(divisor_reg, divisor_num, dividend_reg, dividend_num);

    // Condicional para adicionar o restante ou o quociente
    if (includeRemainder == 1) {
        div_data = appendRemainderCommand(div_data, result_reg, result_num);
    } else {
        div_data = appendQuotientCommand(div_data, result_reg, result_num);
    }

    entry->assembly_code = div_data;
    appendInstruction(instruction_list, entry);
}


// Função auxiliar para criar o rótulo
char* createLabel(char *identifier, int index) {
    // Calcula o tamanho do label com base no número
    int length = (index == 0) ? 1 : log10(index) + 1;
    // Aloca o espaço necessário para armazenar o rótulo
    char *label_data = malloc(sizeof(char) * (strlen(identifier) + length + 5)); // +5 para "\n" + ":" + "\n"
    // Cria o rótulo no formato adequado
    sprintf(label_data, "\n%s%d:\n", identifier, index);
    return label_data;
}

// Função principal para inserir a marcação
void insertMarker(MIPS_NODE *instruction_list, char *identifier, int index) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Cria o rótulo
    char *appendData = createLabel(identifier, index);

    // Atribui o código gerado ao entry e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(instruction_list, entry);
}



// Função auxiliar para criar a string do marcador de invocação de função
char* createFunctionInvocationLabel(char *function_identifier, int invocation_index) {
    // Calcula o tamanho necessário para o rótulo com o número
    int length = (invocation_index == 0) ? 1 : log10(invocation_index) + 1;
    // Aloca memória para o espaço da string final
    char *label_data = malloc(sizeof(char) * (strlen(function_identifier) + length + 2)); // +2 para o "\n"
    // Formata a string do rótulo
    sprintf(label_data, "%s%d\n", function_identifier, invocation_index);
    return label_data;
}

// Função principal para inserir o marcador de invocação de função
void insertFunctionInvocationMarker(MIPS_NODE *instruction_list, char *function_identifier, int invocation_index) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Cria o rótulo de invocação de função
    char *appendData = createFunctionInvocationLabel(function_identifier, invocation_index);

    // Atribui a string gerada ao código da instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(instruction_list, entry);
}


// Função auxiliar para gerar o código assembly do sinal de parada
char* createHaltSignalCode() {
    char *halt_signal = malloc(sizeof(char) * 40); // Tamanho suficiente para a instrução
    sprintf(halt_signal, "\taddi $v0, $zero, 10\n");
    sprintf(halt_signal, "%s\tsyscall\n", halt_signal);
    return halt_signal;
}

// Função principal para inserir o sinal de parada na lista
void insertHaltSignal(MIPS_NODE *list) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Gera o código do sinal de parada
    char *appendData = createHaltSignalCode();

    // Atribui o código gerado à instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(list, entry);
}


// Função auxiliar para gerar o código do comando scanf
char* createScanfInputCode(int s_register_number) {
    char *scanf_code = malloc(sizeof(char) * 100); // Tamanho suficiente para as instruções

    // Carrega o valor 5 em $v0 para indicar a operação de leitura (scanf)
    sprintf(scanf_code, "\tli $v0, 5\n");

    // Executa a syscall para ler um valor inteiro
    sprintf(scanf_code, "%s\tsyscall\n", scanf_code);

    // Armazena o valor lido em $v0 no registrador $s[register_number]
    sprintf(scanf_code, "%s\tadd $s%d, $zero, $v0\n", scanf_code, s_register_number);

    return scanf_code;
}

// Função principal para inserir o comando scanf na lista
void insertScanfInputCommand(MIPS_NODE *list, int s_register_number) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Gera o código do comando scanf
    char *appendData = createScanfInputCode(s_register_number);

    // Atribui o código gerado à instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(list, entry);
}


// Função auxiliar para criar o código de operação condicional
char* createConditionalOperationCode(char register_one, int number_one, int label_id) {
    int label_length = (label_id == 0) ? 1 : log10(label_id) + 1;
    char *conditional_code = malloc(sizeof(char) * (18 + label_length)); // Tamanho suficiente para a instrução

    // Gera a instrução beqz (branch if equal to zero)
    sprintf(conditional_code, "\tbeqz $%c%d, Else_%d\n", register_one, number_one, label_id);

    return conditional_code;
}

// Função principal para inserir a operação condicional na lista
void insertConditionalOperation(MIPS_NODE *instruction_list, char register_one, int number_one, int label_id) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Gera o código para a operação condicional
    char *appendData = createConditionalOperationCode(register_one, number_one, label_id);

    // Atribui o código gerado à instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(instruction_list, entry);
}


// Função auxiliar para criar o código para a operação lógica "AND"
char* createLogicalAndCode(char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    // Tamanho do buffer necessário para armazenar o código
    char *and_code = malloc(sizeof(char) * 100);

    // Gera o código para a operação lógica "AND"
    sprintf(and_code, "\tsne $%c%d, $zero, $%c%d\n", register_one, number_one, register_one, number_one); // Define o registrador um para 1 se não for igual a zero
    sprintf(and_code, "%s\tsne $%c%d, $zero, $%c%d\n", and_code, register_two, number_two, register_two, number_two); // Define o registrador dois para 1 se não for igual a zero
    sprintf(and_code, "%s\tand $%c%d, $%c%d, $%c%d\n", and_code, result_register, result_number, register_one, number_one, register_two, number_two); // Realiza a operação AND

    return and_code;
}

// Função principal para inserir o comando lógico "AND" na lista
void insertLogicalAndCommand(MIPS_NODE *instruction_list, char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Gera o código para a operação lógica AND
    char *appendData = createLogicalAndCode(register_one, number_one, register_two, number_two, result_register, result_number);

    // Atribui o código gerado à instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(instruction_list, entry);
}



// Função auxiliar para criar o código para a operação lógica "OR"
char* createLogicalOrCode(char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    // Tamanho do buffer necessário para armazenar o código
    char *or_code = malloc(sizeof(char) * 100);

    // Gera o código para a operação lógica "OR"
    sprintf(or_code, "\tsne $%c%d, $zero, $%c%d\n", register_one, number_one, register_one, number_one); // Define o registrador um para 1 se não for igual a zero
    sprintf(or_code, "%s\tsne $%c%d, $zero, $%c%d\n", or_code, register_two, number_two, register_two, number_two); // Define o registrador dois para 1 se não for igual a zero
    sprintf(or_code, "%s\tor $%c%d, $%c%d, $%c%d\n", or_code, result_register, result_number, register_one, number_one, register_two, number_two); // Realiza a operação OR

    return or_code;
}

// Função principal para inserir o comando lógico "OR" na lista
void insertLogicalOrCommand(MIPS_NODE *instruction_list, char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Gera o código para a operação lógica OR
    char *appendData = createLogicalOrCode(register_one, number_one, register_two, number_two, result_register, result_number);

    // Atribui o código gerado à instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(instruction_list, entry);
}


// Função auxiliar para criar o código para a operação de verificação de igualdade
char* createEqualityCheckCode(char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    // Tamanho do buffer necessário para armazenar o código
    char *equality_code = malloc(sizeof(char) * 30);

    // Gera o código para a operação de verificação de igualdade
    sprintf(equality_code, "\tseq $%c%d, $%c%d, $%c%d\n", result_register, result_number, register_one, number_one, register_two, number_two);

    return equality_code;
}

// Função principal para inserir o comando de verificação de igualdade na lista
void insertEqualityCheckCommand(MIPS_NODE *instruction_list, char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Gera o código para a operação de verificação de igualdade
    char *appendData = createEqualityCheckCode(register_one, number_one, register_two, number_two, result_register, result_number);

    // Atribui o código gerado à instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(instruction_list, entry);
}



// Função auxiliar para criar o código para a operação de comparação "menor que"
char* createComparisonLessThanCode(char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    // Tamanho do buffer necessário para armazenar o código
    char *comparison_code = malloc(sizeof(char) * 30);

    // Gera o código para a operação de comparação "menor que"
    sprintf(comparison_code, "\tslt $%c%d, $%c%d, $%c%d\n", result_register, result_number, register_one, number_one, register_two, number_two);

    return comparison_code;
}

// Função principal para inserir o comando de comparação "menor que" na lista
void insertComparisonLessThanCommand(MIPS_NODE *instruction_list, char register_one, int number_one, char register_two, int number_two, char result_register, int result_number) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));

    // Gera o código para a operação de comparação "menor que"
    char *appendData = createComparisonLessThanCode(register_one, number_one, register_two, number_two, result_register, result_number);

    // Atribui o código gerado à instrução e adiciona à lista
    entry->assembly_code = appendData;
    appendInstruction(instruction_list, entry);
}

// Função auxiliar para criar o código para o segmento de dados (printf)
char* createPrintfDataCode(char *string, int label_number) {
    // Tamanho necessário para armazenar o código
    char *data_code = malloc(sizeof(char) * (strlen(string) + 30));

    // Gera o código para o segmento de dados (instrução .asciiz)
    sprintf(data_code, "\tmsg_%d:\t .asciiz %s\n", label_number, string);

    return data_code;
}

// Função auxiliar para criar o código para o segmento de texto (chamada de printf)
char* createPrintfTextCode(char *label, int label_number) {
    // Tamanho necessário para armazenar o código
    char *text_code = malloc(sizeof(char) * (strlen(label) + 90));

    // Gera o código para o segmento de texto (instrução syscall)
    sprintf(text_code, "\taddi $v0, $zero, 4\n");
    sprintf(text_code, "%s\tla $a0, %s%d\n", text_code, label, label_number);
    sprintf(text_code, "%s\tsyscall\n", text_code);

    return text_code;
}

// Função principal para formatar o comando printf
void formatPrintfCommand(MIPS_ARCHITECTURE *codeBase, char *label, char* string, int label_number) {
    // Criação e inserção do código no segmento de dados
    MIPS_NODE *item_data = malloc(sizeof(MIPS_NODE));
    char *data_code = createPrintfDataCode(string, label_number);
    item_data->assembly_code = data_code;
    appendInstruction(codeBase->static_memory, item_data);

    // Criação e inserção do código no segmento de texto
    MIPS_NODE *item_text = malloc(sizeof(MIPS_NODE));
    char *text_code = createPrintfTextCode(label, label_number);
    item_text->assembly_code = text_code;
    appendInstruction(codeBase->instruction_flow, item_text);
}


// Função auxiliar para criar o código do rótulo de uma nova função
char* createFunctionLabel(char *name) {
    // Criação do código para o rótulo da função
    char *label_code = malloc(sizeof(char) * (strlen(name) + 10));
    sprintf(label_code, "%s:\n", name);
    return label_code;
}

// Função principal para definir uma nova função
void defineNewFunction(MIPS_NODE *list, char *name) {
    // Criação do entry de função
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));
    
    // Criação do código do rótulo da função
    char *function_label = createFunctionLabel(name);
    
    // Atribuindo o código à instrução e adicionando ao fluxo
    entry->assembly_code = function_label;
    appendInstruction(list, entry);
}

void insertCommand(MIPS_NODE **list, const char *format, ...) {
    char buffer[256]; // Buffer para armazenar o comando formatado
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    // Criando um novo nó na lista encadeada
    MIPS_NODE *new_item = (MIPS_NODE *)malloc(sizeof(MIPS_NODE));
    new_item->assembly_code = strdup(buffer);
    new_item->next_instruction = NULL;

    // Inserindo na lista de comandos (final da lista)
    if (*list == NULL) {
        *list = new_item;
    } else {
        MIPS_NODE *temp = *list;
        while (temp->next_instruction != NULL) {
            temp = temp->next_instruction;
        }
        temp->next_instruction = new_item;
    }
}

void insertReturnOp(MIPS_NODE *list) {
    insertCommand(&list, "\tjr $ra\n");
}

void insertMoveOp(MIPS_NODE *list, char source_register, int source_number, char dest_register, int dest_number) {
    insertCommand(&list, "\tmove $%c%d, $%c%d\n", dest_register, dest_number, source_register, source_number);
}


void insertHaltSignalCode(MIPS_NODE *list, float exit_value) {
    MIPS_NODE *entry = malloc(sizeof(MIPS_NODE));
    char *appendData = malloc(sizeof(char) * 60);

    // Carregar o valor de saída (exit_value) no registrador $a0
    sprintf(appendData, "\tli $a0, %d\n", (int) exit_value);

    // Chamar a syscall 10 (exit)
    strcat(appendData, "\tli $v0, 10\n");  // Syscall para terminar o programa
    strcat(appendData, "\tsyscall\n");

    entry->assembly_code = appendData;
    appendInstruction(list, entry);
}

void insertAssignOp(MIPS_NODE *list, char target_register, int target_number, char value_register, int value_number) {
    // Aloca memória para o novo entry
    MIPS_NODE *entry = (MIPS_NODE *) malloc(sizeof(MIPS_NODE));
    if (!entry) {
        fprintf(stderr, "Erro de alocação de memória para MIPS_NODE.\n");
        return;
    }

    // Aloca memória para armazenar a string de código MIPS
    char *assign_code = (char *) malloc(50);
    if (!assign_code) {
        fprintf(stderr, "Erro de alocação de memória para assign_code.\n");
        free(entry);
        return;
    }

    // Gera o código MIPS para a atribuição: move $target, $value
    snprintf(assign_code, 50, "\tmove $%c%d, $%c%d\n", target_register, target_number, value_register, value_number);

    entry->assembly_code = assign_code;
    appendInstruction(list, entry);
}
