#include <stdio.h>
#include <stdlib.h>
#include "linear_scan_alloc.h"
#include "intervals.h"
#include <limits.h>

// Declarações do linear_scan_alloc
LINEAR_SCAN_ALLOCATOR* create_linear_scan_allocation();
int linear_scan(Interval *intervals, int num_intervals, int K, int *spill_vars, int *spill_iterations, int *spill_count, int **spill_array, int index_novo);
void linear_scan_register_allocation(LINEAR_SCAN_ALLOCATOR *reg_alloc, int **spill_array);
void deallocate_registers(LINEAR_SCAN_ALLOCATOR* reg_alloc);


LINEAR_SCAN_ALLOCATOR* create_linear_scan_allocation() {
    LINEAR_SCAN_ALLOCATOR* reg_alloc = malloc(sizeof(LINEAR_SCAN_ALLOCATOR));
    reg_alloc->num_intervals = 0;
    reg_alloc->registradores_fisicos = 0;
    reg_alloc->spill_aux = 0;
    reg_alloc->current_K = 0;
    return reg_alloc;
}


int linear_scan(Interval *intervals, int num_intervals, int K, int *spill_vars, int *spill_iterations, int *spill_count, int **spill_array, int index_novo) {
    int *active = malloc(K * sizeof(int));     // Índices dos intervalos ativos
    int *active_end = malloc(K * sizeof(int)); // Fim dos intervalos ativos
    int spill_occurred = 0;

    // Inicializa os vetores
    for (int i = 0; i < K; i++) {
        active[i] = -1;
        active_end[i] = -1;
    }
    for (int i = 0; i < num_intervals; i++) {
        intervals[i].reg = -1;  // Inicializa a variável registradora como -1 (não alocada)
        spill_vars[i] = 0;      // Inicializa spill_vars como 0 (não spill)
    }

    for (int i = 0; i < num_intervals; i++) {

        // Remover registradores expirados
        for (int j = 0; j < K; j++) {
            if (active[j] != -1 && active_end[j] <= intervals[i].start) {
                active[j] = -1;
                active_end[j] = INT_MAX;  // Para garantir que não seja escolhido acidentalmente
            }
        }

        // Tentar alocar um registrador disponível
        int allocated = -1;
        for (int j = 0; j < K; j++) {
            if (active[j] == -1) {
                active[j] = i;
                active_end[j] = intervals[i].end;
                allocated = j;
                break;
            }
        }

        // Se não conseguiu alocar, escolhe quem será "spilled"
        if (allocated == -1) {
            // Selecionar o intervalo ativo que tem o maior "end", mas também começou há mais tempo
            int latest_end = -1;
            int spill_index = -1;

            // Procurando o intervalo com o maior número de linha na última utilização
            for (int j = 0; j < K; j++) {
                // Verifica se o intervalo j deve ser spillado
                if (active_end[j] > latest_end || 
                    (active_end[j] == latest_end && 
                    (intervals[active[j]].end - intervals[active[j]].start) < (intervals[active[spill_index]].end - intervals[active[spill_index]].start)) ||
                    (active_end[j] == latest_end && 
                    (intervals[active[j]].end - intervals[active[j]].start) == (intervals[active[spill_index]].end - intervals[active[spill_index]].start) &&
                    active[j] > active[spill_index])) {
                    latest_end = active_end[j];
                    spill_index = j;
                }
            }


            // Se encontrou um para "spill", faz a troca
            if (spill_index != -1 && latest_end > intervals[i].end) {
                spill_vars[active[spill_index]] = 1; // Marca como SPILL

                // Registrar a iteração do spill (baseada no índice i)
                if (*spill_count == 0 || spill_iterations[*spill_count - 1] != i) {
                    
                    spill_iterations[(*spill_count)++] = i; // Armazena o momento do spill
                    //printf("______________________________________________Antes de inserir - K: %d index_novo: %d\n\n", K, index_novo);
                    spill_array[K][index_novo] = i;
                    //printf("-----2------Valor armazenado spill_array[%d][%d]: %d\n", K, index_novo, spill_array[K][index_novo]);
                    index_novo++;
                }
                active[spill_index] = i;
                active_end[spill_index] = intervals[i].end;
                //printf("Caiu aqui-----spill_index: %d---\n", spill_index);
                allocated = spill_index;
               
            } else {
                // Se não há quem substituir, o novo intervalo sofre SPILL
                spill_vars[i] = 1;

                // Registrar a iteração do spill (baseada no índice i)
                if (*spill_count == 0 || spill_iterations[*spill_count - 1] != i) {
                    spill_iterations[(*spill_count)++] = i; // Armazena o momento do spill
                    //printf("____________________2__________________________Antes de inserir - K: %d index_novo: %d\n\n", K, index_novo);
                    spill_array[K][index_novo] = i;
                    //printf("-----3------Valor armazenado spill_array[%d][%d]: %d\n", K, index_novo, spill_array[K][index_novo]);
                    index_novo++;
                }
                
                spill_occurred = 1;
            }
        }

        intervals[i].reg = allocated;
    }

    free(active);
    free(active_end);

    return spill_occurred;
}



void linear_scan_register_allocation(LINEAR_SCAN_ALLOCATOR *reg_alloc, int **spill_array) {
    qsort(reg_alloc->intervals, reg_alloc->num_intervals, sizeof(Interval), compare_intervals);

    // Aloca arrays auxiliares
    int *spill_vars = malloc(reg_alloc->num_intervals * sizeof(int));
    int *spill_iterations = malloc(reg_alloc->num_intervals * sizeof(int));
    int spill_count = 0;

    int index_novo = 0;

    linear_scan(reg_alloc->intervals, reg_alloc->num_intervals, reg_alloc->current_K, spill_vars, spill_iterations, &spill_count, spill_array, index_novo);
    
    // Exibir alocação por variável
    for (int i = 0; i < reg_alloc->num_intervals; i++) {
        if (spill_vars[i]) {
            reg_alloc->intervals[i].spill = 1;
            reg_alloc->spill_aux = 1;
            printf("%s: SPILL\n", reg_alloc->intervals[i].name);
        } else {
            reg_alloc->intervals[i].spill = 0;
            reg_alloc->spill_aux = 0;
            printf("%s: %d\n", reg_alloc->intervals[i].name, reg_alloc->intervals[i].reg);
        }
    }

    free(spill_vars);
    free(spill_iterations);
}

void deallocate_registers(LINEAR_SCAN_ALLOCATOR* reg_alloc) {
    free(reg_alloc);
}