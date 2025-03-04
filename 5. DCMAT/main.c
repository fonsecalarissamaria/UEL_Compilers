#include <stdio.h>
#include <stdbool.h>
#include "funcoes.h"
#include "tree.h"
#include "matriz.h"
#include "symbol_table.h"
#include <stdlib.h>
#define MAX_SIZE 2

// Variáveis globais de controle
bool quit = false;
int function_to_be_called = -1;
float aux_float[MAX_SIZE] = {0.0f};
int aux_int[MAX_SIZE] = {0};         
void* aux_ptr = NULL;
int error_flag = 0;

// Declarações de funções
void resetAuxValues();
void aboutPrint();
void cleanUp();
 
// Funções manipuladoras
void plotGraphHandler(void);
void displayMatrixHandler(void);
void matrixOperationsHandler(void);
void solveDeterminantHandler(void);
void solveLinearSystemHandler(void);
void integrateFunctionHandler(void);
void setIntegralStepsHandler(void);
void setHorizontalViewHandler(void);
void setVerticalViewHandler(void);
void enableAxisHandler(void);
void disableAxisHandler(void);
void showSettingsHandler(void);
void resetSettingsHandler(void);
void setErase(void);
void sumFunctionMain(void);

// Mapeamento das funções
void (*functionMap[])(void) = {
    [plotGraph] = plotGraphHandler,
    [displayMatrix] = displayMatrixHandler,
    [matrixOperations] = matrixOperationsHandler,
    [solveDeterminant] = solveDeterminantHandler,
    [solveLinearSystem] = solveLinearSystemHandler,
    [integrateFunction] = integrateFunctionHandler,
    [setIntegralSteps] = setIntegralStepsHandler,
    [setHorizontalView] = setHorizontalViewHandler,
    [setVerticalView] = setVerticalViewHandler,
    [enableAxis] = enableAxisHandler,
    [disableAxis] = disableAxisHandler,
    [show_Settings] = showSettingsHandler,
    [reset_Settings] = resetSettingsHandler,
    [aboutInfo] = aboutPrint,
    [set_erase] = setErase
};

int main(int argc, char **argv) {

    init_symbol_table(); // inicializa todos com NULLO
    

    while (!quit) {
        // Solicita a entrada do usuário
        printf("> ");
        fflush(stdout);
        if (yyparse() == 0) {
            // Se o parsing foi bem-sucedido
        }

        // Executa a função correspondente usando a função mapeada
        if (function_to_be_called >= 0 && function_to_be_called < sizeof(functionMap) / sizeof(functionMap[0])) {
            functionMap[function_to_be_called]();
        }

        // Reseta os valores auxiliares após o comando
        resetAuxValues();
    }

    
    
    // Finaliza o programa
    cleanUp();
    return 0;
}

// Funções auxiliares

void resetAuxValues() {
    function_to_be_called = -1;
    for (int i = 0; i < MAX_SIZE; i++) {
        aux_int[i] = 0;
    }
    for (int i = 0; i < MAX_SIZE; i++) {
        aux_float[i] = 0.0;
    }
    aux_ptr = NULL;
}

void aboutPrint() {
    print_cat();
}

void cleanUp() {
    function_global_delete();
    liberarMatrizGlobal();
}

// Funções de manipulação de cada caso

void plotGraphHandler() {
    plot(aux_ptr);
}

void displayMatrixHandler() {
    exibirEstruturaMatriz(NULL);
}

void matrixOperationsHandler() {
    definirMatrizGlobal(aux_ptr);
}

void solveDeterminantHandler() {
    calcularDeterminante(NULL);
}

void solveLinearSystemHandler() {
    resolverSistemaLinear(NULL);
}

void integrateFunctionHandler() {
    integrate(aux_ptr, aux_float[0], aux_float[1]);
    ASTNodeDelete(aux_ptr);
}



void setIntegralStepsHandler() {
    integral_steps_set(aux_int[0]);
}

void setHorizontalViewHandler() {
    set_h_view(&aux_float[0], &aux_float[1]);
}

void setVerticalViewHandler() {
    set_v_view(&aux_float[0], &aux_float[1]);
}

void enableAxisHandler() {
    set_axis(true);
}

void disableAxisHandler() {
    set_axis(false);
}

void showSettingsHandler() {
    show_settings();
}

void resetSettingsHandler() {
    reset_settings();
}


void setErase()
{
    //teste
}