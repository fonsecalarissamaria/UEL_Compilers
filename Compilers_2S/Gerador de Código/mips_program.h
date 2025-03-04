#ifndef _MIPS_PROGRAM_
#define _MIPS_PROGRAM_

typedef struct mips_node {
    char *assembly_code;   
    struct mips_node *next_instruction;  
} MIPS_NODE;
void insertEqualityCheckCommand(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index
);

void insertExitCode(
    MIPS_NODE *list, 
    float exit_value
);

void insertHaltSignal(
    MIPS_NODE *operation_chain
);

void insertCommand(
    MIPS_NODE **list, 
    const char *format, 
    ...
);

void insertMultiplicationOperation(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index
);

void insertSubtractionOperation(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index
);

void insertFunctionInvocationMarker(
    MIPS_NODE *operation_chain, 
    char *marker_name, 
    int marker_id
);

void insertComparisonLessThanCommand(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index
);

void insertReturnOp(
    MIPS_NODE *list
);

void insertMoveOp(
    MIPS_NODE *list, 
    char source_register, 
    int source_number, 
    char dest_register, 
    int dest_number
);

void insertHaltSignalCode(
    MIPS_NODE *list, 
    float exit_value
);

void insertLogicalAndCommand(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index
);

typedef struct mips_architecture {
        MIPS_NODE *static_memory;
        MIPS_NODE *instruction_flow;
    } MIPS_ARCHITECTURE;
    MIPS_ARCHITECTURE* beginProgram();

void formatPrintfCommand(
    MIPS_ARCHITECTURE *mips_code_structure, 
    char *marker_name, 
    char *text_output, 
    int marker_id
);

void insertLogicalOrCommand(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index
);

void defineNewFunction(
    MIPS_NODE *operation_chain, 
    char *function_identifier
);

void insertMarker(
    MIPS_NODE *operation_chain, 
    char *marker_name, 
    int marker_id
);

void constructMipsBlueprint(
    MIPS_ARCHITECTURE *mips_code_structure
);

void insertDivisionOperationWithRemainder(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index, 
    int remainder_flag
);

void insertConditionalOperation(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    int marker_id
);

void insertAssignOp(
    MIPS_NODE *list, 
    char target_register, 
    int target_number, 
    char value_register, 
    int value_number
);

void insertAdditiveCommand(
    MIPS_NODE *operation_chain, 
    char reg_one, 
    int num_one, 
    char reg_two, 
    int num_two, 
    char result_register, 
    int result_index
);

void insertScanfInputCommand(
    MIPS_NODE *operation_chain, 
    int input_register_index
);
    
#endif