#ifndef _SYNTAX_TREE_H_
#define _SYNTAX_TREE_H_

    typedef struct quantumNode QUANTUM_NODE;
    typedef struct cosmicData COSMIC_DATA;
    typedef struct valueSlot VALUE_SLOT;
    typedef struct actionNode ACTION_NODE;
    typedef struct actionSet ACTION_SET;
    typedef struct procedureDetails PROCEDURE_DETAILS;
    typedef struct argument ARGUMENT;

    typedef struct arbiter_node 
    {   
    int kind;                   
    int integer_data;           
    float decimal_data;         
    char *text_data;            
    struct arbiter_node *left_child_node;  
    struct arbiter_node *right_child_node; 
    } ARBITER_NODE;  

    ACTION_SET *createNewExit(int exitCode);
    ARGUMENT *createNewArgument(ARBITER_NODE *argExpr, ARGUMENT *nextArg);
    ACTION_SET *createNewForLoop(ARBITER_NODE *initNode, ARBITER_NODE *condNode, ARBITER_NODE *incNode, ACTION_SET *loopBody, ACTION_SET *nextCmd);
    ACTION_SET *createNewCommandIfReturn(int cmdType, ARBITER_NODE *expNode, ARBITER_NODE *cmd1, ARBITER_NODE *cmd2, ARBITER_NODE *expNode2);
    PROCEDURE_DETAILS *startOperation(ARBITER_NODE *arbiterNodeName, int returnType, ARBITER_NODE *returnExpression, QUANTUM_NODE *paramList, COSMIC_DATA *varList, ACTION_SET *cmdList, PROCEDURE_DETAILS *nextFunc);
    ARBITER_NODE *createExpressionTree(int nodeType, int nodeValue, char *nodeStr, ARBITER_NODE *leftNode, ARBITER_NODE *rightNode, float nodeValueFloat);
    QUANTUM_NODE *addNewParameter(ARBITER_NODE *paramId, int paramType, QUANTUM_NODE *nextParam);
    void displayAST(PROCEDURE_DETAILS *astTree);
    ARBITER_NODE *createNewConstant(char *constName, int constValue);
    COSMIC_DATA *createNewGlobalVariable(ARBITER_NODE *varId, int varType, COSMIC_DATA *nextVar);
    ACTION_SET *createNewAssignment(ARBITER_NODE *targetNode, ARBITER_NODE *valueNode);
    void setArrayProperties(COSMIC_DATA *arrayVar, int arraySize);
    ACTION_SET *createNewWhile(ARBITER_NODE *conditionNode, ACTION_SET *cmdList);
    ACTION_SET *createNewDoWhile(ARBITER_NODE *conditionNode, ACTION_SET *cmdList);
    COSMIC_DATA *insertVariable(ARBITER_NODE *varId, int varType, COSMIC_DATA *nextVar);
    ACTION_SET *generateCommand(int cmdType, ARBITER_NODE *expNode, ACTION_SET *cmd1, ACTION_SET *cmd2, ARBITER_NODE *expNode2);
    ACTION_SET *linkCommands(ACTION_SET *currentCmd, ACTION_SET *nextCmd);
 
#endif