#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linear_scan_alloc.h"
#include <unistd.h>


int main() {
    LINEAR_SCAN_ALLOCATOR *linear_scan_alloc = create_linear_scan_allocation();

    // Ler K da entrada
    fscanf(stdin, "K=%d\n", &linear_scan_alloc->registradores_fisicos);
    
    // Ler os intervalos de vida das variáveis
    while (fscanf(stdin, "%s --> %d %d\n", 
                  linear_scan_alloc->intervals[linear_scan_alloc->num_intervals].name, 
                  &linear_scan_alloc->intervals[linear_scan_alloc->num_intervals].start, 
                  &linear_scan_alloc->intervals[linear_scan_alloc->num_intervals].end) == 3) {
        linear_scan_alloc->intervals[linear_scan_alloc->num_intervals].reg = -1; // Inicializa como não alocado
        linear_scan_alloc->num_intervals++;
    }

    // Testar diferentes valores de K (decrescente)
    int min_K = 2;
    int max_K = linear_scan_alloc->registradores_fisicos;
    int spill_iterations[100]; // Matriz para armazenar SPILLs por K
    int spill_count[100] = {0}; // Conta o número de SPILLs para cada K
        
    // Inicializa os contadores de spill
    memset(spill_count, 0, sizeof(spill_count));

    // Alocar spill_array fora do loop, com tamanho suficiente para todas as iterações de K
    int **spill_array = malloc(MAX_VARIABLES * sizeof(int *));
    for (int i = 0; i < MAX_VARIABLES; i++) {
        spill_array[i] = malloc(2 * sizeof(int)); 
    }
 
    for (int k = max_K; k >= min_K; k--) {
        linear_scan_alloc->current_K = k;
        printf("K = %d\n\n", k);

        // Processa a alocação de registradores
        linear_scan_register_allocation(linear_scan_alloc, spill_array);

        for (int i = 0; i < linear_scan_alloc->num_intervals; i++) {
            if (linear_scan_alloc->intervals[i].spill == 1) {
                spill_iterations[spill_count[k]++] = i + 1; 
            }
        }

        printf("----------------------------------------\n");

    }

    // Exibe as iterações com SPILL para cada K
    printf("----------------------------------------\n");


    for (int k = max_K; k >= min_K; k--) {
        int index = k - min_K;

        int i = 0;
        int j = 0; 

        if (spill_count[k] > 0) 
        {
            printf("K = %d: SPILL on interation(s): ", k);
            for (int i = 0; i < spill_count[k]; i++)
            {
                printf("%d", spill_array[k][i]);
                if (i < spill_count[k] - 1) printf(", ");
            }
        } else 
        {
            printf("K = %d: Successful Allocation", k);
        }
        i++;
        

        if (k > min_K) 
        {
            printf("\n");
        }
    }

    
    free(spill_array);

    deallocate_registers(linear_scan_alloc);
    return 0;
}

   