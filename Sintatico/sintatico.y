%{

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define size 10000
extern char* yytext;
extern char currentLineBuffer[size];
extern char bufferAccumulator[size];
extern char tokenAccumulator[size];
extern char tokenAccumulator2[size];
extern int suporteSintatico;
extern int lexicalAid;
extern int columnPosition;
extern int currentLine;
int yylex(void);

int dimensao;
int numeroLinha = 0;
char bufferAuxiliar[size];

void yyerror(char *s) {
    // Verifica se a suporteSintatico ainda não foi ativada
    if (suporteSintatico != 0) {
        return;  // Se suporteSintatico for diferente de 0, a função não faz nada
    }

    // Atualiza dimensao e numeroLinha
    dimensao = columnPosition - (int)(strlen(currentLineBuffer) - 1);
    numeroLinha = currentLine;
    
    // Copia o conteúdo de currentLineBuffer para bufferAuxiliar
    strncpy(bufferAuxiliar, currentLineBuffer, sizeof(bufferAuxiliar) - 1);
    bufferAuxiliar[sizeof(bufferAuxiliar) - 1] = '\0';  

    suporteSintatico = 1;
}


%}


%token VOID
%token INT
%token CHAR
%token RETURN
%token BREAK
%token SWITCH
%token CASE
%token DEFAULT
%token DO
%token WHILE
%token FOR
%token IF
%token ELSE
%token TYPEDEF
%token STRUCT
%token PLUS
%token MINUS
%token MULTIPLY
%token DIV
%token REMAINDER
%token INC
%token DEC
%token BITWISE_AND
%token BITWISE_OR
%token BITWISE_NOT
%token BITWISE_XOR
%token NOT
%token LOGICAL_AND
%token LOGICAL_OR
%token EQUAL
%token NOT_EQUAL
%token LESS_THAN
%token GREATER_THAN
%token LESS_EQUAL
%token GREATER_EQUAL
%token R_SHIFT
%token L_SHIFT
%token ASSIGN
%token ADD_ASSIGN
%token MINUS_ASSIGN
%token SEMICOLON
%token COMMA
%token COLON
%token L_PAREN
%token R_PAREN
%token L_CURLY_BRACKET
%token R_CURLY_BRACKET
%token L_SQUARE_BRACKET
%token R_SQUARE_BRACKET
%token TERNARY_CONDITIONAL
%token NUMBER_SIGN
%token POINTER
%token PRINTF
%token SCANF
%token DEFINE
%token EXIT
%token NUM_OCTAL
%token NUM_INTEGER
%token NUM_HEXA
%token CHARACTER
%token STRING
%token IDENTIFIER

%start start

%%

/*Exemplo de teste:

mod (int a, int b)

{
  return (a % b);
}

int
main ()
{
  mod (1, 2);
  exit (0);
}
*/

start:
      input { printf("SUCCESSFUL COMPILATION."); }
;


input:
      blocoDeDeclaracoes corpoPrograma
    | declaracaoFuncao corpoPrograma
;

// Corpo do programa
corpoPrograma:
      input
    | 
;

// Bloco de declarações no início do arquivo
blocoDeDeclaracoes:
      NUMBER_SIGN DEFINE IDENTIFIER expressaoPrincipal
    | declaracaoVariaveis
    | declaracaoPrototipos
;

// Declaração de função
declaracaoFuncao:
      tipoDeRetorno corpoFuncao
;

// Corpo de uma função, que pode ser uma lista de parâmetros ou um bloco de código
corpoFuncao:
      MULTIPLY corpoFuncao
    | IDENTIFIER listaParametros L_CURLY_BRACKET corpoFuncoes
;

// Funções no corpo da função, incluindo variáveis e comandos
corpoFuncoes:
      declaracaoVariaveis corpoFuncoes
    | listaComandos R_CURLY_BRACKET
;

// Declaração de variáveis
declaracaoVariaveis:
      tipoDeRetorno variaveisList
;

// Lista de variáveis, que pode incluir múltiplas variáveis ou ponteiros
variaveisList:
      IDENTIFIER variaveisDeclaracao
    | MULTIPLY variaveisList
;

// Declaração de variáveis, com atribuições e listas
variaveisDeclaracao:
      ASSIGN expressaoAtribuicao listaVariaveis
    | L_SQUARE_BRACKET expressaoPrincipal R_SQUARE_BRACKET variaveisDeclaracao
    | listaVariaveis
;

// Lista de variáveis pode ser apenas um ponto e vírgula ou uma lista separada por vírgulas
listaVariaveis:
      SEMICOLON
    | COMMA variaveisList
;

// Declaração de protótipos de função
declaracaoPrototipos:
      tipoDeRetorno prototipoFuncao
;

// Protótipo de função, que pode incluir múltiplos protótipos
prototipoFuncao:
      IDENTIFIER listaParametros SEMICOLON 
    | MULTIPLY prototipoFuncao
;

// Lista de parâmetros
listaParametros:
      L_PAREN parametrosFechamento
;

// Fechamento dos parâmetros, pode ser uma lista de parâmetros ou apenas um parêntese fechado
parametrosFechamento:
      parametroLista
    | R_PAREN
;

// Lista de parâmetros, que pode incluir tipos de parâmetros e parâmetros opcionais
parametroLista:
      tipoDeRetorno tipoParametro
;

// Tipo de parâmetro, que pode ser um identificador ou um ponteiro
tipoParametro:
      IDENTIFIER parametroOpcional
    | MULTIPLY tipoParametro 
;

// Parâmetro opcional, que pode incluir uma lista de parâmetros ou um parêntese fechado
parametroOpcional:
      COMMA parametroLista
    | R_PAREN
    | L_SQUARE_BRACKET expressaoPrincipal R_SQUARE_BRACKET parametroOpcional
;

// Tipos de retorno possíveis para funções
tipoDeRetorno:
      CHAR
    | VOID
    | INT
;

// Lista de comandos dentro de um bloco de código
listaComandos:
      comandosPrincipais comandosRecursivos
;

// Comandos recursivos podem incluir outros comandos ou ser vazio
comandosRecursivos:
      listaComandos
    | /* Vazio */
;

// Bloco de código delimitado por chaves
blocoDeCodigo:
      L_CURLY_BRACKET listaComandos R_CURLY_BRACKET
;

// Comandos principais, incluindo estruturas de controle e chamadas de função
comandosPrincipais:
      IF L_PAREN expressaoPrincipal R_PAREN blocoDeCodigo alternativaElse
    | SEMICOLON
    | FOR L_PAREN instrucoesFor
    | PRINTF L_PAREN STRING parametrosPrintf
    | SCANF L_PAREN STRING COMMA BITWISE_AND IDENTIFIER R_PAREN SEMICOLON
    | EXIT L_PAREN expressaoPrincipal R_PAREN SEMICOLON
    | RETURN retornoFuncao
    | expressaoPrincipal SEMICOLON
    | WHILE L_PAREN expressaoPrincipal R_PAREN blocoDeCodigo
    | DO blocoDeCodigo WHILE L_PAREN expressaoPrincipal R_PAREN SEMICOLON
    | blocoDeCodigo
;

// Alternativa else para estruturas condicionais
alternativaElse:
      ELSE blocoDeCodigo
    | /* Vazio */
;

	
// Instruções de controle dentro de um loop for
instrucoesFor:
      SEMICOLON instrucoesWhile
    | expressaoPrincipal SEMICOLON instrucoesWhile
;

// Instruções de controle dentro de um loop while
instrucoesWhile:
      SEMICOLON instrucoesIf
    | expressaoPrincipal SEMICOLON instrucoesIf
;

// Instruções de controle dentro de uma estrutura condicional if
instrucoesIf:
      fechamentoParenteseBloco
    | expressaoPrincipal fechamentoParenteseBloco
;

// Fechamento de parênteses seguido por um bloco de código
fechamentoParenteseBloco:
      R_PAREN blocoDeCodigo
;

// Parâmetros para a função printf, com ou sem expressões
parametrosPrintf:
      R_PAREN SEMICOLON
    | COMMA expressaoPrincipal R_PAREN SEMICOLON
;

// Retorno de função, podendo ser vazio ou uma expressão
retornoFuncao:
      SEMICOLON
    | expressaoPrincipal SEMICOLON
;

// Expressão principal, que pode incluir atribuição e adicionais
expressaoPrincipal:
      expressaoAtribuicao expressaoAdicional
;

// Adicionais para a expressão principal, incluindo múltiplas expressões
expressaoAdicional:
      COMMA expressaoAtribuicao expressaoAdicional
    | /* Regra base: quando não há mais expressões adicionais */
;

// Atribuição de expressão, que pode incluir uma atribuição ou uma expressão condicional
expressaoAtribuicao:
      expressaoUnaria operadorAtribuicao
    | expressaoCondicional
;

// Operadores de atribuição, que podem ser adição, subtração ou atribuição simples
operadorAtribuicao:
      ADD_ASSIGN expressaoAtribuicao
    | MINUS_ASSIGN expressaoAtribuicao
    | ASSIGN expressaoAtribuicao
;



// Expressão condicional, incluindo uma expressão lógica OR e condicional ternária
expressaoCondicional:
      expressaoOrLogico condicionalTernario
;

// Condicional ternário, composta por um operador ternário seguido de duas expressões
condicionalTernario:
      TERNARY_CONDITIONAL expressaoPrincipal COLON expressaoCondicional
    | /* Regra base: quando não há um condicional ternário */
;

// Expressão lógica OR, composta por AND lógico e adicionais de OR lógico
expressaoOrLogico:
      expressaoAndLogico orLogicoAdicional
;

// Adicionais de OR lógico, que podem incluir operadores de OR lógico
orLogicoAdicional:
      LOGICAL_OR expressaoAndLogico orLogicoAdicional
    | /* Regra base: quando não há mais operadores de OR lógico */
;

// Expressão lógica AND, composta por OR lógico e adicionais de AND lógico
expressaoAndLogico:
      expressaoOr andLogicoAdicional
;

// Adicionais de AND lógico, que podem incluir operadores de AND lógico
andLogicoAdicional:
      LOGICAL_AND expressaoOr andLogicoAdicional
    | /* Regra base: quando não há mais operadores de AND lógico */
;

// Expressão OR bitwise, composta por XOR e adicionais de OR bitwise
expressaoOr:
      expressaoXor orAdicional
;

// Adicionais de OR bitwise, que podem incluir operadores de OR bitwise
orAdicional:
      BITWISE_OR expressaoXor orAdicional
    | /* Regra base: quando não há mais operadores de OR bitwise */
;

// Expressão XOR bitwise, composta por AND bitwise e adicionais de XOR bitwise
expressaoXor:
      expressaoAnd xorAdicional
;

// Adicionais de XOR bitwise, que podem incluir operadores de XOR bitwise
xorAdicional:
      BITWISE_XOR expressaoAnd xorAdicional
    | /* Regra base: quando não há mais operadores de XOR bitwise */
;



// Expressão lógica com operador AND
expressaoAnd:
      expressaoIgualdade andAdicional
;

// Adicionais para operadores AND bitwise
andAdicional:
      BITWISE_AND expressaoIgualdade andAdicional
    | /* Regra base: quando não há mais operadores AND */
;

// Expressão de igualdade, composta por relacional e adicionais de igualdade
expressaoIgualdade:
      expressaoRelacional igualdadeAdicional
;

// Adicionais de igualdade, incluindo desigualdade e igualdade
igualdadeAdicional:
      NOT_EQUAL expressaoRelacional igualdadeAdicional
    | EQUAL expressaoRelacional igualdadeAdicional
    | /* Regra base: quando não há mais operadores de igualdade */
;

// Expressão relacional, composta por shift e adicionais relacionais
expressaoRelacional:
      expressaoShift relacionalAdicional
;

// Adicionais relacionais, incluindo comparação maior/menor
relacionalAdicional:
      GREATER_EQUAL expressaoShift relacionalAdicional
    | GREATER_THAN expressaoShift relacionalAdicional
    | LESS_EQUAL expressaoShift relacionalAdicional
    | LESS_THAN expressaoShift relacionalAdicional
    | /* Regra base: quando não há mais operadores relacionais */
;

// Expressão de shift, composta por aditiva e adicionais de shift
expressaoShift:
      expressaoAditiva shiftAdicional
;

// Adicionais de shift, incluindo shift à esquerda e à direita
shiftAdicional:
      R_SHIFT expressaoAditiva shiftAdicional
    | L_SHIFT expressaoAditiva shiftAdicional
    | /* Regra base: quando não há mais operadores de shift */
;



// Expressão aditiva, composta por multiplicativa e adicionais aditivos
expressaoAditiva:
      expressaoMultiplicativa aditivaAdicional
;

// Adicionais aditivos, que podem ser operações de soma ou subtração
aditivaAdicional:
      PLUS expressaoMultiplicativa aditivaAdicional
    | MINUS expressaoMultiplicativa aditivaAdicional
    | /* Regra base: quando não há mais aditivos */
;

// Expressão multiplicativa, composta por casting e adicionais multiplicativos
expressaoMultiplicativa:
      expressaoCasting multiplicativaAdicional
;

// Adicionais multiplicativos, que podem ser operações de módulo, divisão ou multiplicação
multiplicativaAdicional:
      REMAINDER expressaoCasting multiplicativaAdicional
    | DIV expressaoCasting multiplicativaAdicional
    | MULTIPLY expressaoCasting multiplicativaAdicional
    | /* Regra base: quando não há mais multiplicativos */
;

// Expressão de casting, que pode ser uma expressão unária ou uma expressão em parênteses com tipo de retorno
expressaoCasting:
      expressaoUnaria
    | L_PAREN tipoDeRetorno R_PAREN expressaoCasting
;

// Expressão unária, que pode incluir operações bitwise, sinais, ou pré e pós incrementos/decrementos
expressaoUnaria:
      BITWISE_NOT expressaoCasting
    | MINUS expressaoCasting
    | PLUS expressaoCasting
    | MULTIPLY expressaoCasting
    | BITWISE_AND expressaoCasting
    | DEC expressaoUnaria
    | INC expressaoUnaria
    | expressaoPosFixa
    | NOT expressaoCasting
;



// Regra principal para expressões pós-fixa
expressaoPosFixa:
      expressaoPrimaria
    | expressaoPosFixa posFixaAdicional
;

// Adicionais possíveis para uma expressão pós-fixa
posFixaAdicional:
      L_PAREN fechamentoParentese
    | DEC
    | INC
    | L_SQUARE_BRACKET expressaoPrincipal R_SQUARE_BRACKET
;

// Fechamento de parênteses
fechamentoParentese:
      R_PAREN
    | expressaoAtribuicaoAdicional
;

// Atribuição adicional para expressão
expressaoAtribuicaoAdicional:
      expressaoAtribuicao parenteseFechamento
;

// Fechamento de parênteses com uma expressão adicional opcional
parenteseFechamento:
      COMMA expressaoAtribuicaoAdicional
    | R_PAREN
;

// Definição de expressões primárias
expressaoPrimaria:
      L_PAREN expressaoPrincipal R_PAREN
    | STRING
    | CHARACTER
    | IDENTIFIER
    | numeroLiteral
;

// Literais numéricos
numeroLiteral:
      NUM_OCTAL
    | NUM_HEXA
    | NUM_INTEGER
;



%%


int main(int argc, char **argv) {
    // Chamada de funções externas
    yyparse();
    while (yylex()); 

    // Inicializando variáveis
    char tempString[size] = {0};  // Inicializando com zero
    int currentIndex = 0;

    // Copiando caracteres até encontrar '\n' ou o final da string
    for (; tokenAccumulator2[currentIndex] && tokenAccumulator2[currentIndex] != '\n'; ++currentIndex) {
        tempString[currentIndex] = tokenAccumulator2[currentIndex];
    }
    tempString[currentIndex] = '\0';  // Assegurando que tempString seja uma string válida

    // Verificação e exibição da mensagem de erro
    if (suporteSintatico == 1) {
        printf("error:syntax:%d:%d: %s\n%s\n%*s", numeroLinha, dimensao, bufferAuxiliar, tempString, dimensao, "^");
    }

    return 0;
}
