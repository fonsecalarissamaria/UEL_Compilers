#ifndef MATRIZ_H
#define MATRIZ_H

#include"tree.h"
#include <math.h>
typedef void* Matriz;
#define TAMANHO_MAXIMO_MATRIZ 10

typedef void* EstruturaMatriz;

typedef struct estrutura_matriz{
    float **elementos;
    int qtdLinhas;
    int qtdColunas;
} estrutura_matriz_t;

void liberarEstruturaMatriz(EstruturaMatriz);
EstruturaMatriz addMatrices(EstruturaMatriz m1, EstruturaMatriz m2);
int isMatrix(void *value);
EstruturaMatriz multiplyMatrixByScalar(EstruturaMatriz m, float scalar);
EstruturaMatriz criarEstruturaMatriz(TreeNode raiz);
void definirMatrizGlobal(EstruturaMatriz);
int atribuirElemento(EstruturaMatriz mat, int linha, int coluna, float valor);
EstruturaMatriz multiplyMatrices(EstruturaMatriz m1, EstruturaMatriz m2);
void resolverSistemaLinear(EstruturaMatriz m);
float calcularDeterminante(EstruturaMatriz m);
void exibirEstruturaMatriz(EstruturaMatriz);
int configurarLinhas(EstruturaMatriz mat, int linhas);
int configurarColunas(EstruturaMatriz mat, int colunas);
void liberarMatrizGlobal();
EstruturaMatriz subtractMatrices(EstruturaMatriz m1, EstruturaMatriz m2);

#endif
