#ifndef _GERENCIADOR_GRAFO_H_
#define _GERENCIADOR_GRAFO_H_


typedef struct networkMap NETMAP;
typedef struct connection CONEXAO;
typedef struct node NO;

NETMAP* initialize_map(

);

NO* add_new_node(
    int node_id
);

void insert_node_in_map(
    NETMAP* map, 
    NO* node
);

void connect_node_to_target(
    NO* source, 
    int target_node_id
);

void connect_nodes_in_map(
    NETMAP* map, 
    int sourceNodeId, 
    int target_node_id
);

struct node {  
    NO* anterior; 
    NO* proximo;  
    CONEXAO* inicio;       
    CONEXAO* fim;          
    int identificador;   
    int grau;              
    int cor;              
};

void disconnect_edge_from_node(
    NO* node, 
    int target_node_id
);

struct networkMap {
    NO* entryPoint;
    NO* exitPoint;
    int totalNodes;
};

void remove_node_from_map(
    NETMAP* map, 
    NO* node
);

struct connection {  
    CONEXAO* no_anterior;  
    CONEXAO* proximo_no;   
    int indice_no;               
};

#endif