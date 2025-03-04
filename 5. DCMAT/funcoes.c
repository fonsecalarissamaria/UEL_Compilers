#include<stdio.h>
#include<math.h>
#include"funcoes.h"
#include "symbol_table.h"
#include <stdlib.h>
#define MAX_FUNC_LENGTH 256
#define WIDTH 80   // Largura do gráfico (número de colunas)
#define HEIGHT 25  // Altura do gráfico (número de linhas)
TreeNode globalProcessor = NULL;
// Variáveis globais de configuração
float h_view_lo = -6.5f;
float h_view_hi = 6.5f;
float v_view_lo = -3.5f;
float v_view_hi = 3.5f; 
int integral_steps = 1000;
int float_precision = 6;
int draw_axis = 1;  // 1 = ON, 0 = OFF
int erase_plot = 1; // 1 = ON, 0 = OFF
int connect_dots = 0; // 0 = OFF, 1 = ON

// Estrutura para representar os pontos na tela
typedef struct {
    float x;
    float y;
    char pixel;
} ScreenPoint;
ScreenPoint screen[WIDTH][HEIGHT];

// Funções principais
void show_settings();
void reset_settings();
void set_h_view(float* low, float* high);
void set_v_view(float* low, float* high);
void set_axis(int status);
void integral_steps_set(int steps);
void plot(TreeNode function);
void integrate(TreeNode* function, float inf_limit, float sup_limit);
void function_global_delete();

// Funções auxiliares
static void update_screen_with_function(float* y_values);
static void initialize_screen(float* x_step, float* y_step);
static void render_axis_on_screen();
static void calculate_steps(float* x_step, float* y_step);
static void print_screen();
float riemannSum(TreeNode* function, float lo_bound, float x_step);

// Função que exibe o conteúdo das variáveis internas
void show_settings() {
    printf("\nh_view_lo: %.6f\n", h_view_lo);
    printf("h_view_hi: %.6f\n", h_view_hi);
    printf("v_view_lo: %.6f\n", v_view_lo);
    printf("v_view_hi: %.6f\n", v_view_hi);
    printf("float precision: %d\n", float_precision);
    printf("integral_steps: %d\n", integral_steps);
    printf("\nDraw Axis: %s\n", draw_axis ? "ON" : "OFF");
    printf("Erase Plot: %s\n", erase_plot ? "ON" : "OFF");
    printf("Connect Dots: %s\n", connect_dots ? "ON" : "OFF");
    printf("\n");
}

// Função que reseta as variáveis para os valores padrões
void reset_settings() {
    h_view_lo = -6.5f;
    h_view_hi = 6.5f;
    v_view_lo = -3.5f;
    v_view_hi = 3.5f;
    integral_steps = 1000;
    float_precision = 6;
    draw_axis = 1;  // 1 = ON
    erase_plot = 1; // 1 = ON
    connect_dots = 0; // 0 = OFF
    
    printf("Settings have been reset to default values.\n\n");
}

// Função que configura a visualização horizontal
void set_h_view(float* low, float* high) {
    if (*low >= *high) {
        printf("\nERROR: h_view_lo must be smaller than h_view_hi\n\n");
        return;
    }
    h_view_lo = *low;
    h_view_hi = *high;
    printf("\nHorizontal view set to: %.6f to %.6f\n\n", h_view_lo, h_view_hi);
}

// Função que configura a visualização vertical
void set_v_view(float* low, float* high) {
    if (*low >= *high) {
        printf("\nERROR: v_view_lo must be smaller than v_view_hi\n\n");
        return;
    }
    v_view_lo = *low;
    v_view_hi = *high;
    printf("\nVertical view set to: %.6f to %.6f\n\n", v_view_lo, v_view_hi);
}


// Função para ajustar a precisão do ponto flutuante
void setFloatPrecision(int precision) {
    if (precision >= 0 && precision <= 8) {
        float_precision = precision;
        //printf("Float precision set to %d\n", float_precision);
    } else {
        printf("\nERROR: float precision must be from 0 to 8\n\n");
    }
}

// Função para retornar o valor atual da precisão de ponto flutuante
int getFloatPrecision() {
    return float_precision;
}


void print_cat() {
    printf("+-----------------------------------------------------------+\n");
    printf("|                                                           |\n");
    printf("|      202200560118 - Larissa Maria dos Santos Fonseca      |\n");
    printf("|                                                           |\n");
    printf("|                         /\\_/\\                             |\n");
    printf("|                        ( o.o )                            |\n");
    printf("|                         > ^ <                             |\n");
    printf("|                                                           |\n");
    printf("+-----------------------------------------------------------+\n");
}

// Função que ativa ou desativa os eixos no gráfico
void set_axis(int status) {
    draw_axis = status;
    if (draw_axis) {
        printf("Eixos ativados para o gráfico.\n");
    } else {
        printf("Eixos desativados para o gráfico.\n");
    }
}


// Calcula os passos de escala para os eixos
static void calculate_steps(float* x_step, float* y_step) {
    *x_step = (h_view_hi - h_view_lo) / WIDTH;
    *y_step = (v_view_hi - v_view_lo) / HEIGHT;
    if (erase_plot)
        initialize_screen(x_step, y_step);
    render_axis_on_screen();
}

// Exibe a tela no console
static void print_screen() {
    for (int y = HEIGHT - 1; y >= 0; y--) {
        for (int x = 0; x < WIDTH; x++) {
            printf("%c", screen[x][y].pixel);
        }
        printf("\n");
    }
}

// Função que configura o número de passos da integral
void integral_steps_set(int steps) {
    if (steps <= 0) {
        printf("\nERROR: integral_steps must be a positive non-zero integer\n\n");
        return;
    }
    integral_steps = steps;
    printf("Integral steps set to: %d\n", integral_steps);
}


void integrate(TreeNode* function, float inferior, float superior){
    if (!function) {
        fprintf(stderr, "\nError: No function passed.\n\n");
        return;
    }

    if (inferior >= superior) {
        if (inferior == superior) {
            printf("\nIntegral result: %f\n\n", 0.0);
        } else {
            printf("\nERROR: Lower limit must be smaller than upper limit.\n\n");
        }
        return;
    }

    // Calcular o intervalo de x de forma diferente
    float interval_width = (superior - inferior);
    float x_step = interval_width / integral_steps;
    
    // Chamar a função de soma de Riemann para calcular a integral
    float integral_result = riemannSum(function, inferior, x_step);
    
    // Exibir o resultado da integral de forma compacta
    printf("\nIntegral result: %f\n\n", integral_result);
}

/* Função auxiliar para realizar a soma de Riemann */
float riemannSum(TreeNode* function, float lo_bound, float x_step){
    if (!function) {
        fprintf(stderr, "\nError: Invalid function pointer\n");
        return 0.0;
    }

    float x = lo_bound, sum = 0;
    int error_flag = 0;

    for (int i = 0; i < integral_steps; ++i) {
        // Cálculo da altura no ponto médio do intervalo
        float mid_point = x + (x_step / 2);
        float height = ASTNodeSolveForX(function, mid_point, &error_flag);

        if (error_flag) {
            printf("\nError in solving for x at step %d\n", i);
            return 0.0;
        }

        // Acumulando a altura na soma
        sum += height;

        // Avançando o valor de x
        x += x_step;
    }

    // Multiplica pela largura do intervalo (x_step) para retornar o valor da integral
    return sum * x_step;
}



void summation(TreeNode* function, int start, int end) {
    if (!function) {
        printf("\nError: no function passed\n\n");
        return;
    }

    if (start > end) {
        printf("\nERROR: start index must be less than or equal to end index\n\n");
        return;
    }

    float result = 0.0;
    int error = 0;

    // Itera sobre o intervalo e calcula o somatório
    for (int i = start; i <= end; i++) {
        // Verifica se a função foi avaliada corretamente
        float value = ASTNodeSolveForX(function, i, &error);
        if (error) {
            printf("\nError evaluating the function at i = %d\n\n", i);
            return;
        }
        result += value;
    }

    // Exibe o resultado
    printf("\n%f\n\n", result);
}



void bresenham_line(int x0, int y0, int x1, int y1, char screen[HEIGHT][WIDTH]) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x0 >= 0 && x0 < WIDTH && y0 >= 0 && y0 < HEIGHT) {
            screen[y0][x0] = '*'; // Marca o ponto no "screen"
        }
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// Função de plotagem do gráfico
void plot(TreeNode function) {
    if (!function) {
        if (!globalProcessor) {
            printf("\nNo Function defined!\n\n");
            return;
        }
        function = globalProcessor;
    } else 
        globalProcessor = function;
    

    float y_values[WIDTH];
    int dont = 0;
    float x_step;
    float y_step;
    
    calculate_steps(&x_step, &y_step);

    for (int i = 0; i < WIDTH; i++) {
        y_values[i] = ASTNodeSolveForX(function, (h_view_lo + x_step * i), &dont);
        if (dont) {
            return;
        }
    }
    update_screen_with_function(y_values);
    print_screen();
}


// Funções auxiliares

// Atualiza a tela com os valores da função
static void update_screen_with_function(float* y_values) {
    int j;
    for (int i = 0; i < WIDTH; i++) {
        if (y_values[i] >= v_view_lo && y_values[i] <= v_view_hi) {
            for (j = HEIGHT - 1; j >= 0; j--) {
                if (screen[i][j].y <= y_values[i]) {
                    screen[i][j].pixel = '*';
                    break;
                }
            }
        }
    }
}

// Inicializa a tela com os valores de coordenadas
static void initialize_screen(float* x_step, float* y_step) {
    for (int a = 0; a < WIDTH; a++) {
        for (int b = 0; b < HEIGHT; b++) {
            screen[a][b].x = h_view_lo + (*x_step * b);
            screen[a][b].y = v_view_lo + (*y_step * b);
            screen[a][b].pixel = ' ';
        }
    }
}

// Renderiza os eixos na tela
static void render_axis_on_screen() {
    if (draw_axis) {
        float x_steps_to_zero = -1;
        float y_steps_to_zero = -1;

        if (v_view_lo * v_view_hi <= 0) {
            y_steps_to_zero = v_view_lo / (v_view_hi - v_view_lo);
            y_steps_to_zero = roundf(fabsf(y_steps_to_zero * (HEIGHT - 1)));
            for (int i = 0; i < WIDTH; i++) {
                screen[i][(int)y_steps_to_zero].pixel = '-';
            }
        }

        if (h_view_lo * h_view_hi <= 0) {
            x_steps_to_zero = h_view_lo / (h_view_hi - h_view_lo);
            x_steps_to_zero = roundf(fabsf(x_steps_to_zero * (WIDTH - 1)));
            for (int i = 0; i < HEIGHT; i++) {
                screen[(int)x_steps_to_zero][i].pixel = '|';
            }

            if (y_steps_to_zero != -1) {
                screen[(int)x_steps_to_zero][(int)y_steps_to_zero].pixel = '+';
            }
        }
    }
}

// Função para deletar a função global
void function_global_delete() {
    if (globalProcessor) {
        ASTNodeDelete(globalProcessor);
    }
}


