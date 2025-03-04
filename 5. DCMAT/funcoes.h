#ifndef FUNCOES_H
#define FUNCOES_H

#include <stdbool.h>
#include "tree.h"

// Enum para as funções disponíveis
enum functions {
    aboutInfo,
    plotGraph,
    displayMatrix,
    matrixOperations,
    solveDeterminant,
    solveLinearSystem,
    integrateFunction,
    sumFunction,
    setIntegralSteps,
    setHorizontalView,
    setVerticalView,
    enableAxis,
    disableAxis,
    show_Settings,
    reset_Settings,
    set_erase
};

// Declaração de variáveis globais de configuração para o gráfico
extern float h_view_lo;
extern float h_view_hi;
extern float v_view_lo;
extern float v_view_hi;
extern int integral_steps;
extern int float_precision;
extern int draw_axis;  // 1 = ON, 0 = OFF
extern int erase_plot; // 1 = ON, 0 = OFF
extern int connect_dots; // 0 = OFF, 1 = ON

// Declaração das funções principais
void show_settings();
void reset_settings();
void set_h_view(float* low, float* high);
void set_v_view(float* low, float* high);
void integral_steps_set(int steps);
void set_axis(int status);
void print_cat();
void summation(TreeNode* function, int start, int end);
int getFloatPrecision();
void setFloatPrecision(int precision);

// Função para integração utilizando soma de Riemann
void integrate(TreeNode* function, float inf_limit, float sup_limit);

// Função para plotar a função no gráfico
void plot(TreeNode function);

// Função para deletar a função global
void function_global_delete();

#endif // FUNCOES_H
