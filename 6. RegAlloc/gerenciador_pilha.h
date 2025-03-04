#ifndef _GERENCIADOR_PILHA_H_
#define _GERENCIADOR_PILHA_H_


typedef struct elemento ELEMENTO;
typedef struct pilha PILHA;
typedef void* DADO;
PILHA* iniciar_pilha();

void adicionar_na_pilha(
    PILHA* pilha, 
    DADO item
);

struct elemento {
    ELEMENTO* anterior;
    DADO dado;
};

DADO remover_da_pilha(
    PILHA* pilha
);

struct pilha {
    ELEMENTO* topo;
    int quantidade;
};

void destruir_pilha(
    PILHA* pilha
);


#endif