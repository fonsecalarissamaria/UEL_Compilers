#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include"matriz.h"
#include "funcoes.h"

// Funções auxiliares para decomposição e resolução de sistema
float*** decomporLU(float** matriz, int n);
int verificarConjuntoSolucao(float ***lu, float **matriz, int n);
int verificarSistemaInconsistente(float **matriz, int linha, int n);
float* resolverSistemaInferior(float **L, float **B, int n);
float* resolverSistemaSuperior(float **U, float *Y, int n);
// Funções auxiliares de impressão
void exibirResultado(float *x, int n);
void imprimirMatrizComPrecisao(estrutura_matriz_t *mat, int precision);
// Funções de alocação e desalocação
float** alocarMatriz(int n);
void desalocarMatriz(float ***lu, int n);

estrutura_matriz_t *estrutura_matriz_principal = NULL;

void liberarEstruturaMatriz(EstruturaMatriz estrutura_matriz) {
    estrutura_matriz_t *matriz = estrutura_matriz;
    if (!matriz) {
        return;  // Retorna se a matriz for nula
    }

    // Libera os elementos de cada linha da matriz
    for (int indice = 0; indice < matriz->qtdLinhas; indice++) {
        free(matriz->elementos[indice]);
    }
    
    // Libera o array de elementos (as linhas) e a estrutura da matriz
    free(matriz->elementos);
    free(matriz);
}


EstruturaMatriz criarEstruturaMatriz(TreeNode raiz) {
    estrutura_matriz_t *matriz = malloc(sizeof(estrutura_matriz_t));
    if (!matriz) {
        printf("Erro ao alocar memória para a matriz.\n");
        return NULL;
    }

    // Inicializando a estrutura da matriz com dimensões definidas
    matriz->elementos = malloc(TAMANHO_MAXIMO_MATRIZ * sizeof(float*));
    if (!matriz->elementos) {
        free(matriz);
        printf("Erro ao alocar memória para os elementos da matriz.\n");
        return NULL;
    }

    // Alocando memória para cada linha da matriz
    for (int i = 0; i < TAMANHO_MAXIMO_MATRIZ; i++) {
        matriz->elementos[i] = malloc(TAMANHO_MAXIMO_MATRIZ * sizeof(float));
        if (!matriz->elementos[i]) {
            for (int j = 0; j < i; j++) {
                free(matriz->elementos[j]);
            }
            free(matriz->elementos);
            free(matriz);
            printf("Erro ao alocar memória para a linha %d da matriz.\n", i);
            return NULL;
        }
    }

    matriz->qtdLinhas = 0;
    matriz->qtdColunas = 0;

    // Utilizando a raiz da árvore para preencher a matriz
    int linha = 0, coluna = 0;
    TreeNode atual = raiz;

    while (atual != NULL && linha < TAMANHO_MAXIMO_MATRIZ) {
        TreeNode temp = atual;
        while (temp != NULL && coluna < TAMANHO_MAXIMO_MATRIZ) {
            // Verifica se os índices ultrapassam o limite da matriz
            if (linha >= TAMANHO_MAXIMO_MATRIZ || coluna >= TAMANHO_MAXIMO_MATRIZ) {
                printf("\nERROR: Matrix limits out of boundaries.\n\n");
                for (int i = 0; i < TAMANHO_MAXIMO_MATRIZ; i++) {
                    free(matriz->elementos[i]);
                }
                free(matriz->elementos);
                free(matriz);
                return NULL;
            }
            
            // Preenchendo a matriz com os valores extraídos dos nós da árvore
            matriz->elementos[linha][coluna] = ASTNodeGetValue(temp);
            coluna++;
            if (coluna > matriz->qtdColunas) {
                matriz->qtdColunas = coluna;
            }
            temp = ASTNodeGetRight(temp);
        }
        linha++;
        matriz->qtdLinhas = linha;
        coluna = 0;
        atual = ASTNodeGetLeft(atual);
    }

    // Verifica se a quantidade de linhas ou colunas excedeu o limite máximo
    if (matriz->qtdLinhas > TAMANHO_MAXIMO_MATRIZ || matriz->qtdColunas > TAMANHO_MAXIMO_MATRIZ) {
        printf("ERROR: Matrix limits out of boundaries.\n");
        for (int i = 0; i < TAMANHO_MAXIMO_MATRIZ; i++) {
            free(matriz->elementos[i]);
        }
        free(matriz->elementos);
        free(matriz);
        return NULL;
    }

    // Libera a árvore após a utilização
    ASTNodeDelete(raiz);

    return matriz;
}


void definirMatrizGlobal(EstruturaMatriz novaMatriz) {
    estrutura_matriz_t *matriz = novaMatriz;
    if (!matriz) {
        return;  // Verificação de segurança
    }

    // Se já existir uma matriz global, liberar a memória antes de configurar a nova
    if (estrutura_matriz_principal) {
        liberarEstruturaMatriz(estrutura_matriz_principal);
    }

    estrutura_matriz_principal = matriz;  // Atualiza a matriz global para a nova matriz
}


int atribuirElemento(EstruturaMatriz matriz, int linha, int coluna, float valor) {
    estrutura_matriz_t *matrizEstruturada = matriz;
    // Verificação para garantir que a linha e a coluna não ultrapassem os limites
    if (linha < matrizEstruturada->qtdLinhas && coluna < matrizEstruturada->qtdColunas) {
        matrizEstruturada->elementos[linha][coluna] = valor;
        return 0;  // Sucesso
    } else {
        return -1;  // Falha, índice fora dos limites
    }
}


int configurarLinhas(EstruturaMatriz matriz, int totalLinhas) {
    estrutura_matriz_t *matrizEstruturada = matriz;
    // Validação de quantidade de linhas
    if (totalLinhas > 0 && totalLinhas <= TAMANHO_MAXIMO_MATRIZ) {
        matrizEstruturada->qtdLinhas = totalLinhas;
        return 0;  // Sucesso
    }
    return -1;  // Falha, quantidade de linhas inválida
}

int configurarColunas(EstruturaMatriz matriz, int totalColunas) {
    estrutura_matriz_t *matrizEstruturada = matriz;
    // Validação de quantidade de colunas
    if (totalColunas > 0 && totalColunas <= TAMANHO_MAXIMO_MATRIZ) {
        matrizEstruturada->qtdColunas = totalColunas;
        return 0;  // Sucesso
    }
    return -1;  // Falha, quantidade de colunas inválida
}


float calcularDeterminante(EstruturaMatriz matrizEntrada) {
    estrutura_matriz_t *matriz = matrizEntrada;
    
    // Se não for passada uma matriz específica, usa a matriz global
    if (!matriz) {
        if (!estrutura_matriz_principal) {
            printf("\nNenhuma matriz definida!\n\n");
            return 0.0;
        }
        matriz = estrutura_matriz_principal;
    }

    // Verifica se a matriz é quadrada
    if (matriz->qtdLinhas != matriz->qtdColunas) {
        printf("\nMatrix format incorrect!\n\n");
        return 0.0;
    }

    // Aloca uma nova matriz auxiliar para realizar a decomposição de LU
    float **matrizCopia = malloc(matriz->qtdLinhas * sizeof(float *));
    if (!matrizCopia) {
        printf("\nErro na alocação da matriz auxiliar!\n\n");
        return 0.0;
    }

    for (int i = 0; i < matriz->qtdLinhas; i++) {
        matrizCopia[i] = malloc(matriz->qtdColunas * sizeof(float));
        if (!matrizCopia[i]) {
            printf("\nErro na alocação da linha %d da matriz auxiliar!\n\n", i);
            free(matrizCopia);
            return 0.0;
        }
    }

    // Copia os elementos da matriz original para a matriz auxiliar
    for (int i = 0; i < matriz->qtdLinhas; i++) {
        for (int j = 0; j < matriz->qtdColunas; j++) {
            matrizCopia[i][j] = matriz->elementos[i][j];
        }
    }

    // Calcula o determinante utilizando o método de eliminação de Gauss
    float determinante = 1.0;
    for (int i = 0; i < matriz->qtdLinhas; i++) {
        // Verifica se o elemento na diagonal é zero (se for, a matriz é singular)
        if (matrizCopia[i][i] == 0) {
            printf("\nA matriz é singular (determinante igual a zero)!\n\n");
            for (int j = 0; j < matriz->qtdLinhas; j++) {
                free(matrizCopia[j]);
            }
            free(matrizCopia);
            return 0.0;
        }

        // Divide a linha pela diagonal
        for (int j = i + 1; j < matriz->qtdLinhas; j++) {
            float fator = matrizCopia[j][i] / matrizCopia[i][i];
            for (int k = 0; k < matriz->qtdColunas; k++) {
                matrizCopia[j][k] -= fator * matrizCopia[i][k];
            }
        }

        // Multiplica o determinante pelos elementos da diagonal
        determinante *= matrizCopia[i][i];
    }

    // Libera a memória da matriz auxiliar
    for (int i = 0; i < matriz->qtdLinhas; i++) {
        free(matrizCopia[i]);
    }
    free(matrizCopia);

    // Exibe o resultado
    printf("\n%.6f\n\n", determinante);
    
    return determinante;
}

void tratarErroDeSolucao(int tipoErro) {
    switch(tipoErro) {
        case -2:
            printf("SI - The Linear System has no solution\n");
            break;
        case -1:
            printf("SPI - The Linear System has infinitely many solutions\n");
            break;
        default:
            printf("Erro desconhecido.\n");
    }
}


EstruturaMatriz addMatrices(EstruturaMatriz m1, EstruturaMatriz m2) {
    if (m1 == NULL || m2 == NULL) {
        printf("Erro: Matrizes inválidas.\n");
        return NULL;
    }

    estrutura_matriz_t *mat1 = (estrutura_matriz_t *)m1;
    estrutura_matriz_t *mat2 = (estrutura_matriz_t *)m2;

    // Verifique se as matrizes têm o mesmo tamanho
    if (mat1->qtdLinhas != mat2->qtdLinhas || mat1->qtdColunas != mat2->qtdColunas) {
        printf("\nIncorrect dimensions for operator ’+’ - have MATRIX [%d][%d] and MATRIX [%d][%d]\n\n", mat1->qtdLinhas, mat1->qtdColunas, mat2->qtdLinhas, mat2->qtdColunas);
        return NULL;
    }

    // Cria uma nova matriz para armazenar o resultado da soma
    estrutura_matriz_t *result = malloc(sizeof(estrutura_matriz_t));
    if (!result) {
        printf("Erro de alocação de memória para a matriz resultado.\n");
        return NULL;
    }

    result->qtdLinhas = mat1->qtdLinhas;
    result->qtdColunas = mat1->qtdColunas;
    result->elementos = malloc(result->qtdLinhas * sizeof(float *));
    for (int i = 0; i < result->qtdLinhas; i++) {
        result->elementos[i] = malloc(result->qtdColunas * sizeof(float));
    }

    // Soma as matrizes elemento por elemento
    for (int i = 0; i < mat1->qtdLinhas; i++) {
        for (int j = 0; j < mat1->qtdColunas; j++) {
            result->elementos[i][j] = mat1->elementos[i][j] + mat2->elementos[i][j];
        }
    }

    printf("\n");
    exibirEstruturaMatriz(result);
    printf("\n\n");
    return result;
}


EstruturaMatriz subtractMatrices(EstruturaMatriz m1, EstruturaMatriz m2) {
    if (m1 == NULL || m2 == NULL) {
        printf("Erro: Matrizes inválidas.\n");
        return NULL;
    }

    estrutura_matriz_t *mat1 = (estrutura_matriz_t *)m1;
    estrutura_matriz_t *mat2 = (estrutura_matriz_t *)m2;

    // Verifique se as matrizes têm o mesmo tamanho
    if (mat1->qtdLinhas != mat2->qtdLinhas || mat1->qtdColunas != mat2->qtdColunas) {
        printf("\nIncorrect dimensions for operator ’-’ - have MATRIX [%d][%d] and MATRIX [%d][%d]\n\n", mat1->qtdLinhas, mat1->qtdColunas, mat2->qtdLinhas, mat2->qtdColunas);
        return NULL;
    }

    // Cria uma nova matriz para armazenar o resultado da subtração
    estrutura_matriz_t *result = malloc(sizeof(estrutura_matriz_t));
    if (!result) {
        printf("Erro de alocação de memória para a matriz resultado.\n");
        return NULL;
    }

    result->qtdLinhas = mat1->qtdLinhas;
    result->qtdColunas = mat1->qtdColunas;
    result->elementos = malloc(result->qtdLinhas * sizeof(float *));
    for (int i = 0; i < result->qtdLinhas; i++) {
        result->elementos[i] = malloc(result->qtdColunas * sizeof(float));
    }

    // Subtrai as matrizes elemento por elemento
    for (int i = 0; i < mat1->qtdLinhas; i++) {
        for (int j = 0; j < mat1->qtdColunas; j++) {
            result->elementos[i][j] = mat1->elementos[i][j] - mat2->elementos[i][j];
        }
    }

    printf("\n");
    exibirEstruturaMatriz(result);
    printf("\n\n");
    return result;
}


EstruturaMatriz multiplyMatrices(EstruturaMatriz m1, EstruturaMatriz m2) {
    if (m1 == NULL || m2 == NULL) {
        printf("Erro: Matrizes inválidas.\n");
        return NULL;
    }

    estrutura_matriz_t *mat1 = (estrutura_matriz_t *)m1;
    estrutura_matriz_t *mat2 = (estrutura_matriz_t *)m2;

    // Verifique se o número de colunas da primeira matriz é igual ao número de linhas da segunda
    if (mat1->qtdColunas != mat2->qtdLinhas) {
        printf("\nIncorrect dimensions for operator ’*’ - have MATRIX [%d][%d] and MATRIX [%d][%d]\n\n", mat1->qtdLinhas, mat1->qtdColunas, mat2->qtdLinhas, mat2->qtdColunas);
        return NULL;
    }

    // Cria uma nova matriz para armazenar o resultado da multiplicação
    estrutura_matriz_t *result = malloc(sizeof(estrutura_matriz_t));
    if (!result) {
        printf("Erro de alocação de memória para a matriz resultado.\n");
        return NULL;
    }

    result->qtdLinhas = mat1->qtdLinhas;
    result->qtdColunas = mat2->qtdColunas;
    result->elementos = malloc(result->qtdLinhas * sizeof(float *));
    for (int i = 0; i < result->qtdLinhas; i++) {
        result->elementos[i] = malloc(result->qtdColunas * sizeof(float));
    }

    // Multiplica as matrizes elemento por elemento
    for (int i = 0; i < mat1->qtdLinhas; i++) {
        for (int j = 0; j < mat2->qtdColunas; j++) {
            result->elementos[i][j] = 0; // Inicializa o elemento da posição (i,j)
            for (int k = 0; k < mat1->qtdColunas; k++) {
                result->elementos[i][j] += mat1->elementos[i][k] * mat2->elementos[k][j];
            }
        }
    }

    printf("\n");
    exibirEstruturaMatriz(result);  // Supondo que essa função exibe a matriz
    printf("\n\n");
    return result;
}


EstruturaMatriz multiplyMatrixByScalar(EstruturaMatriz elementos, float scalar) {
    if (elementos == NULL) {
        printf("Erro: Matriz inválida.\n");
        return NULL;
    }

    estrutura_matriz_t *mat = (estrutura_matriz_t *)elementos;

    // Cria uma nova matriz para armazenar o resultado da multiplicação
    estrutura_matriz_t *result = malloc(sizeof(estrutura_matriz_t));
    if (!result) {
        printf("Erro de alocação de memória para a matriz resultado.\n");
        return NULL;
    }

    result->qtdLinhas = mat->qtdLinhas;
    result->qtdColunas = mat->qtdColunas;
    result->elementos = malloc(result->qtdLinhas * sizeof(float *));
    for (int i = 0; i < result->qtdLinhas; i++) {
        result->elementos[i] = malloc(result->qtdColunas * sizeof(float));
    }

    // Multiplica cada elemento da matriz pelo escalar
    for (int i = 0; i < mat->qtdLinhas; i++) {
        for (int j = 0; j < mat->qtdColunas; j++) {
            result->elementos[i][j] = mat->elementos[i][j] * scalar;
        }
    }

    printf("\n");
    exibirEstruturaMatriz(result);  // Supondo que essa função exibe a matriz
    printf("\n\n");
    return result;
}

void resolverSistemaLinear(EstruturaMatriz m) {
    estrutura_matriz_t *mat = m;
    if (!mat) {
        if (!estrutura_matriz_principal) {
            printf("\nNo EstruturaMatriz defined!\n\n");
            return;
        }
        mat = estrutura_matriz_principal;
    }

    // Verifica se a matriz está no formato correto (n x n+1)
    if (mat->qtdColunas != mat->qtdLinhas + 1) {
        printf("\nMatrix format incorrect!\n\n");
        return;
    }

    // Realiza a decomposição LU
    float ***lu = decomporLU(mat->elementos, mat->qtdLinhas);
    if (!lu) {
        printf("\nError: LU decomposition failed.\n\n");
        return;
    }

    // Verifica o conjunto de soluções
    int sol_set = verificarConjuntoSolucao(lu, mat->elementos, mat->qtdLinhas);
    if (sol_set != 0) {
        tratarErroDeSolucao(sol_set);
        desalocarMatriz(lu, mat->qtdLinhas);
        return;
    }

    // Resolve o sistema L*Y = B, depois U*X = Y
    float *y = resolverSistemaInferior(lu[0], mat->elementos, mat->qtdLinhas);
    if (!y) {
        desalocarMatriz(lu, mat->qtdLinhas);
        return;
    }

    float *x = resolverSistemaSuperior(lu[1], y, mat->qtdLinhas);
    if (!x) {
        free(y);
        desalocarMatriz(lu, mat->qtdLinhas);
        return;
    }

    // Exibe o resultado
    exibirResultado(x, mat->qtdLinhas);

    // Limpeza de memória
    free(x);
    free(y);
    desalocarMatriz(lu, mat->qtdLinhas);
}

void exibirEstruturaMatriz(EstruturaMatriz m) {
    estrutura_matriz_t *mat = m;
    if (!mat) {
        mat = estrutura_matriz_principal;
        if (!mat) {
            printf("\nNo Matrix defined!\n\n");
            return;
        }
    }

    int precision = getFloatPrecision();
    imprimirMatrizComPrecisao(mat, precision);
}


// Decomposição LU (semelhante a original, mas com outra estrutura)
float*** decomporLU(float** matriz, int n) {
    float ***lu = malloc(2 * sizeof(float**));
    if (!lu) return NULL;

    // Alocação para as matrizes L e U
    float **L = alocarMatriz(n);
    float **U = alocarMatriz(n);
    if (!L || !U) {
        free(lu);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        U[i] = calloc(n, sizeof(float));
        L[i] = calloc(n, sizeof(float));
        L[i][i] = 1.0f; // A diagonal de L é 1
    }

    // Copiar a matriz original para U
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            U[i][j] = matriz[i][j];
        }
    }

    // Decomposição LU
    for (int k = 0; k < n; k++) {
        for (int i = k + 1; i < n; i++) {
            L[i][k] = U[i][k] / U[k][k];
            for (int j = k; j < n; j++) {
                U[i][j] -= L[i][k] * U[k][j];
            }
        }
    }

    lu[0] = L;
    lu[1] = U;
    return lu;
}

// Verifica o conjunto de soluções (modificado para maior clareza)
int verificarConjuntoSolucao(float ***lu, float **matriz, int n) {
    for (int i = 0; i < n; i++) {
        int zeroCount = 0;
        for (int j = 0; j < n; j++) {
            if (lu[1][i][j] == 0 || isinf(lu[1][i][j]) || isnan(lu[1][i][j])) {
                zeroCount++;
            }
        }

        if (zeroCount == n) {
            if (verificarSistemaInconsistente(matriz, i, n)) return -1; // Sistema sem solução
            return -2; // Sistema com infinitas soluções
        }
    }
    return 0; // Sistema bem condicionado
}

// Verifica se o sistema é inconsistente
int verificarSistemaInconsistente(float **matriz, int linha, int n) {
    float resultado = matriz[linha][n];
    for (int c = 0; c < linha; c++) {
        resultado -= matriz[c][n] * matriz[linha][c];
    }
    return (resultado == 0) ? 1 : 0; // Retorna 1 se inconsistente, 0 caso contrário
}

// Funções auxiliares para resolver sistemas inferiores e superiores
float* resolverSistemaInferior(float **L, float **B, int n) {
    float *Y = malloc(n * sizeof(float));
    if (!Y) return NULL;

    for (int i = 0; i < n; i++) {
        Y[i] = B[i][n];
        for (int j = 0; j < i; j++) {
            Y[i] -= L[i][j] * Y[j];
        }
    }

    return Y;
}

float* resolverSistemaSuperior(float **U, float *Y, int n) {
    float *X = malloc(n * sizeof(float));
    if (!X) return NULL;

    X[n - 1] = Y[n - 1] / U[n - 1][n - 1];
    for (int i = n - 2; i >= 0; i--) {
        X[i] = Y[i];
        for (int j = i + 1; j < n; j++) {
            X[i] -= U[i][j] * X[j];
        }
        X[i] /= U[i][i];
    }

    return X;
}

// Função para exibir os resultados
void exibirResultado(float *x, int n) {
    printf("\nMatrix x:\n");
    for (int i = 0; i < n; i++) {
        printf("% .6f\n", x[i]);
    }
    printf("\n");
}

// Função para desalocar a matriz LU
void desalocarMatriz(float ***lu, int n) {
    if (lu) {
        for (int i = 0; i < n; i++) {
            free(lu[0][i]);
            free(lu[1][i]);
        }
        free(lu[0]);
        free(lu[1]);
        free(lu);
    }
}

// Função para alocar a matriz
float** alocarMatriz(int n) {
    float **matriz = malloc(n * sizeof(float*));
    if (!matriz) return NULL;

    for (int i = 0; i < n; i++) {
        matriz[i] = malloc(n * sizeof(float));
        if (!matriz[i]) return NULL;
    }

    return matriz;
}

// Função para imprimir a matriz com precisão
void imprimirMatrizComPrecisao(estrutura_matriz_t *mat, int precision) {
    printf("+--");
    for (int i = 0; i < (mat->qtdColunas * precision) + 4; i++) {
        printf(" ");
    }
    printf("--+\n");

    for (int i = 0; i < mat->qtdLinhas; i++) {
        printf("| ");
        for (int j = 0; j < mat->qtdColunas; j++) {
            printf("%.*f ", precision, mat->elementos[i][j]);
        }
        printf(" |\n");
    }

    printf("+--");
    for (int i = 0; i < (mat->qtdColunas * precision) + 4; i++) {
        printf(" ");
    }
    printf("--+\n");
}


void liberarMatrizGlobal(EstruturaMatriz mat){
    estrutura_matriz_t *m = mat;
    if(!m)
        { return; }
    for(int i=0;i<TAMANHO_MAXIMO_MATRIZ;i++)
        free(m->elementos[i]);
    free(m);
}


int isMatrix(void *value) {
    // Verifica se o ponteiro é nulo ou se não é do tipo matriz
    if (value == NULL) {
        return 0;  // Não é uma matriz
    }

    estrutura_matriz_t *mat = (estrutura_matriz_t *)value;
    return mat->elementos != NULL;  // Se a matriz não é nula, então é uma matriz válida
}
