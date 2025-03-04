#include <stdio.h>
#include <math.h>
#include "symbol_table.h"
#include "tree.h"
#include <string.h>
#include "matriz.h"
#include "sintatico.tab.h"
#include <stdlib.h>

// Enum para os tipos de nós
enum NodeType {
    CONSTANT = 1,   // Tipo para constantes (valores numéricos)
    OPERATOR        // Tipo para operadores (+, -, *, /, ^)
};

float RpnSolve(TreeNode node, float x);

float variables[26] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0,
                           11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
                           21.0, 22.0, 23.0, 24.0, 25.0, 26.0}; 


TreeNode createNode(int type, float value, TreeNode leftChild, TreeNode rightChild, const char *name) {   
    // Aloca memória para o novo nó
    struct ASTNode *newNode = malloc(sizeof(struct ASTNode));
    if (!newNode) {
        printf("Erro de alocação de memória para o nó!\n");
        exit(1);  // Saída em caso de falha na alocação de memória
    }

    // Inicializa os campos da estrutura
    newNode->left = leftChild;
    newNode->right = rightChild;
    newNode->type = type;
    newNode->value = value;
    newNode->name = NULL;  

    // Verifica se um nome de identificador foi fornecido
    if (name != NULL) 
    {
        
        // Aloca memória para armazenar o nome do identificador
        newNode->name = malloc(strlen(name) + 1);  // +1 para o '\0'
        
        if (!newNode->name) {
            // Se falhar na alocação, libera a memória já alocada
            printf("Erro ao alocar memória para o nome do identificador!\n");
            free(newNode);  // Libera a memória alocada para o nó
            exit(1);    // Saída em caso de falha na alocação
        }

        strcpy(newNode->name, name);
    }

    // Retorna o nó criado
    return newNode;
}


int verifica_is_matriz(struct ASTNode *node) {
    if (node == NULL) {
        fprintf(stderr, "Erro: Nó nulo encontrado durante a avaliação.\n");
        exit(1);
    }

   
    // Avalia os nós esquerdo e direito, se existirem
    int left_is_matrix = 0;
    int right_is_matrix = 0;

    if (node->left != NULL) {
        left_is_matrix = verifica_is_matriz(node->left);
    }
    if (node->right != NULL) {
        right_is_matrix = verifica_is_matriz(node->right);
    }
    
    switch (node->type) {
        case IDENTIFIER: {
        
        int find = find_symbol(node->name); 
        if (find == 1) 
        {
            return 1; // É uma matriz
        } else {
            return 0; // Não é matriz ou símbolo não encontrado
        }
        }
        case PLUS: 
        case MINUS: {
            //printf("==============3==============");
            if(left_is_matrix && right_is_matrix)
                return 1; // Se os dois operandos for matriz
        }
        case TIMES: {
            if(left_is_matrix || right_is_matrix)
                return 1; // Se qualquer operando for matriz
        }
        case INT: {
            return 0; // Inteiros não são matrizes
        }
        default:
            //printf("Não é matriz ou operador válido, type: %d\n", node->type);
            return 0;
    }
}



int operacoes_matriz(struct ASTNode *node) {
    if (node == NULL) {
        fprintf(stderr, "Erro: Nó nulo encontrado durante a avaliação.\n");
        exit(1);
    }
    
    // Avalia os nós esquerdo e direito, se existirem
    float left_value = 0.0f;
    float right_value = 0.0f;

    // Verifica se o nó esquerdo é um número (escalar)
    if (node->left != NULL) {
        if (node->left->type == IDENTIFIER) {
            left_value = verifica_is_matriz(node->left);  // Para matrizes
        } else {
            left_value = node->left->value;  // Para números
        }
    }

    // Verifica se o nó direito é uma matriz
    if (node->right != NULL) {
        if (node->right->type == IDENTIFIER) {
            right_value = verifica_is_matriz(node->right);  // Para matrizes
        }
    }

    switch (node->type) {
        case IDENTIFIER:
            if (node->type == MATRIX) {
                // Verifica se é uma matriz
                return 1;
            } else {
                return 0;
            }
        case PLUS:
            if (node->left->type == IDENTIFIER && node->right->type == IDENTIFIER)
            {
                add_matriz(node->left->name, node->right->name);
                return 1;
            }else
            {
                printf("\nIncorrect type for operator\n\n");
                return 0;
            }
        case MINUS:
            if (node->left->type == IDENTIFIER && node->right->type == IDENTIFIER)
            {
                sub_matriz(node->left->name, node->right->name);
                return 1;
            }else
            {
                printf("\nIncorrect type for operator\n\n");
                return 0;
            }
        case TIMES:
            if (node->left != NULL && node->right != NULL) {
                // Caso o operando esquerdo seja um número (escalar) e o direito uma matriz
                if (node->left->type == INT && node->right->type == IDENTIFIER) {
                    mul_matriz_escalar(node->right->name, (float)node->left->value);  // Função de multiplicação escalar
                    return 1;
                }else if (node->left->type == IDENTIFIER && node->right->type == INT) {
                    mul_matriz_escalar(node->left->name, (float)node->right->value);  // Função de multiplicação escalar
                    return 1;
                }

                // Caso contrário, apenas multiplica as duas matrizes
                mul_matriz(node->left->name, node->right->name);
            }
            return 1;
        case INT:
            return 1;

        default:
            printf("\nIncorrect type for operator\n\n");
            return NAN;
    }
}




// Função para avaliar a expressão (usando a tabela de símbolos)
float evaluateExpression(struct ASTNode *node) {
    
    if (node == NULL) {
        fprintf(stderr, "Erro: Nó nulo encontrado durante a avaliação.\n");
        exit(1);
    }

    // Caso o nó seja uma constante (valor numérico)
    if (node->type == INT || node->type == REAL || node->type == X) {
        return node->value;
    }

    error_flag = 0;

    // Avalia os nós esquerdo e direito, se existirem
    float left_value = 0.0f;
    float right_value = 0.0f;

    if (node->left != NULL) {
        left_value = evaluateExpression(node->left);
    }
    if (node->right != NULL) {
        right_value = evaluateExpression(node->right);
    }

    // Lógica para as operações matemáticas
    switch (node->type) {
        case PLUS: 
            if (node->right->type == X || node->left->type == X) {
                printf("\nThe x variable cannot be present on expressions.\n\n");
                error_flag = 1;
                return 0;
            } 
            return left_value + right_value;

        case MINUS:  
            if (node->right->type == X || node->left->type == X) {
                printf("\nThe x variable cannot be present on expressions.\n\n");
                error_flag = 1;
                return 0;
            } 
            return left_value - right_value;

        case TIMES: 
            if (node->right->type == X || node->left->type == X) {
                printf("\nThe x variable cannot be present on expressions.\n\n");
                error_flag = 1;
                return 0;
            }  
            return left_value * right_value;

        case DIVIDE:  
            if (node->right->type == X || node->left->type == X) {
                printf("\nThe x variable cannot be present on expressions.\n\n");
                error_flag = 1;
                return 0;
            } 
            if (right_value == 0) {
                printf("\ninf\n\n");
                error_flag = 1;
                return 0;
            }
            return left_value / right_value;

        case POWER:
            if (node->right->type == X || node->left->type == X) {
                printf("\nThe x variable cannot be present on expressions.\n\n");
                error_flag = 1;
                return 0;
            }   
            return pow(left_value, right_value);

        case MOD: 
            return fmodf(left_value, right_value);

        case SEN:  
            return sinf(left_value);  // Operação seno

        case COS:  
            return cosf(left_value);  // Operação cosseno

        case TAN:  
            return tanf(left_value);  // Operação tangente
        case ABS:  
            return fabsf(left_value);  // Valor absoluto
        case PI:
            return 3.14159265;
        case E:
            return 2.71828;
        case X:
            printf("The x variable cannot be present on expressions.\n\n");
            error_flag = 1;
            return 0;
        case IDENTIFIER: 
            if (node->name == NULL || strlen(node->name) != 1) {
                fprintf(stderr, "Erro: Identificador inválido.");
                error_flag = 1;
                return 0;
            }

            // Obtém o valor da variável da tabela de símbolos
            float* var_value = getSymbolValue_NoPrint(node->name);
            if (var_value == NULL) {
                printf("\nUndefined symbol [%s]\n", node->name);
                error_flag = 1;
                break;
                return NAN;
            }
            
            return *var_value;
            break;  // Retorna o valor da variável    

        default:
            fprintf(stderr, "Erro: Tipo de nó desconhecido.\n");
            error_flag = 1;
            return 0;
    }
}


void printIdentifier(TreeNode node) {
    if (node != NULL && node->name != NULL) {
        printf("Nome do identificador: %s\n", node->name);
    } else {
        printf("Nenhum identificador encontrado.\n");
    }
}


TreeNode ASTNodeGetLeft(TreeNode node) {
    struct ASTNode *aux = NULL; // Inicializa com NULL
    if (node) {
        aux = node->left; // Atualiza aux com o filho à esquerda
    }
    return aux; // Retorna aux (ou NULL se node for NULL)
}

TreeNode ASTNodeGetRight(TreeNode node) {
    struct ASTNode *aux = NULL; // Inicializa com NULL
    if (node) {
        aux = node->right; // Atualiza aux com o filho à direita
    }
    return aux; // Retorna aux (ou NULL se node for NULL)
}

float ASTNodeGetValue(TreeNode node) {
    struct ASTNode *aux = NULL; // Inicializa com NULL
    if (node) {
        aux = node; // Atualiza aux com o nó atual
        return aux->value; // Retorna o valor do nó
    }
    fprintf(stderr, "Erro: Nó nulo não possui valor.\n");
    return 0.0f; // Retorna um valor padrão em caso de erro
}



void ASTNodeRpnWalk(TreeNode n) {
    if (n == NULL) {
        return;  // Se o nó for nulo, não faz nada
    }

    // Processa os filhos da árvore (visita a esquerda e direita)
    TreeNode leftChild = n->left;
    TreeNode rightChild = n->right;

    // Armazena o tipo do nó atual
    int nodeType = n->type;
    float nodeValue = n->value;
    char *nodeName = n->name;

    // Processamento recursivo dos filhos
    ASTNodeRpnWalk(leftChild);
    ASTNodeRpnWalk(rightChild);

    // Agora, com o tipo do nó, decidimos o que imprimir
    switch (nodeType) {
        case PI:
            printf("%f ", nodeValue);  // Se for o tipo PI, imprime o valor numérico
            break;
        case MOD:
            printf("%% ");
            break;
        case ABS:
            printf("ABS ");
            break;
        case TAN:
            printf("TAN ");
            break;
        case COS:
            printf("COS ");
            break;
        case SEN:
            printf("SEN ");
            break;
        case PLUS:
            printf("+ ");
            break;
        case MINUS:
            printf("- ");
            break;
        case TIMES:
            printf("* ");
            break;
        case DIVIDE:
            printf("/ ");
            break;
        case POWER:
            printf("^ ");
            break;
        case X:
            printf("x ");  // Se for o tipo X, imprime "x"
            break;
        case IDENTIFIER:
            printf("%s ", nodeName);  // Se for um identificador, imprime o nome
            break;
        case INT:
        case REAL:
            printf("%f ", nodeValue);  // Se for tipo INT ou REAL, imprime o valor numérico
            break;
    }
}

float ASTNodeSolveForX(TreeNode n, float x, int *error){
    if (n == NULL) { 
        *error = -1;  // Marca erro
        return 0.0;   // Retorna valor padrão
    }

    // Em vez de acessar diretamente os campos do nó, usa um ponteiro local para simplificar
    struct ASTNode *node = (struct ASTNode *) n;
    *error = 0;  // Inicializa erro como 0

    // Realiza a resolução usando a função RpnSolve
    return RpnSolve(node, x);  
}

float RpnSolve(TreeNode node, float x){
    if (node == NULL) {
        return 0.0;  // Caso o nó seja nulo, retorna 0.0
    }

    float result = 0.0f;
    
    // Para os nós esquerdo e direito, se existirem, as operações são feitas recursivamente
    if (node->left != NULL) {
        result = RpnSolve(node->left, x);  // Resultado do nó esquerdo
    }

    if (node->right != NULL) {
        result = RpnSolve(node->right, x);  // Resultado do nó direito
    }

    // Processa o tipo do nó com base no tipo de operação
    switch (node->type) {
        case PLUS:
            return result + RpnSolve(node->right, x); // Soma
        case MINUS:
            return result - RpnSolve(node->right, x); // Subtração
        case TIMES:
            return result * RpnSolve(node->right, x); // Multiplicação
        case DIVIDE:
            return result / RpnSolve(node->right, x); // Divisão
        case POWER:
            return powf(result, RpnSolve(node->right, x)); // Exponenciação
        case MOD:
            return (int)result % (int)RpnSolve(node->right, x); // Módulo
        case SEN:
            return sinf(result); // Seno
        case COS:
            return cosf(result); // Cosseno
        case TAN:
            return tanf(result); // Tangente
        case ABS:
            return fabsf(result); // Valor absoluto
        case INT:
        case REAL:
            return node->value; // Valores inteiros ou reais diretamente do nó
        case X:
            return x; // Retorna o valor de x
        case IDENTIFIER:
            return x; // Retorna o valor de x para identificadores
        default:
            return 0.0; // Caso não seja reconhecido, retorna 0
    }
}




void ASTNodeDelete(TreeNode n){
    if (n == NULL) {
        return;  // Se o nó for nulo, apenas retorna
    }

    // Realiza a exclusão recursiva com o nó e seus filhos
    struct ASTNode *node = (struct ASTNode *) n;
    
    // Deleta a subárvore à esquerda e à direita recursivamente
    if (node->left != NULL) {
        ASTNodeDelete(node->left);  // Deleta o nó esquerdo
    }
    if (node->right != NULL) {
        ASTNodeDelete(node->right);  // Deleta o nó direito
    }

    // Libera a memória do nó atual
    free(node);  
}