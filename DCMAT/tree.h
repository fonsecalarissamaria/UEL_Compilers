#ifndef TREE_H
#define TREE_H
#include <string.h>
typedef struct ASTNode* TreeNode;
TreeNode createNode(int type, float value, TreeNode leftChild, TreeNode rightChild, const char *name);
extern int error_flag; 

struct ASTNode {
    int type;              
    float value;            
    char *name;             
    struct ASTNode *left;   
    struct ASTNode *right;  
};

float evaluateExpression(struct ASTNode *node);
int operacoes_matriz(struct ASTNode *node);
struct ASTNode* ASTNodeGetRight(struct ASTNode *node);
float ASTNodeGetValue(struct ASTNode *node);
void ASTNodeDelete(struct ASTNode *node);
int verifica_is_matriz(struct ASTNode *node);
void ASTNodeRpnWalk(struct ASTNode *node);
float ASTNodeSolveForX(struct ASTNode *node, float x, int *error);
struct ASTNode* ASTNodeGetLeft(struct ASTNode *node);
void printIdentifier(TreeNode node);
float evaluate_expression(TreeNode expression, int value);


#endif