#include "inc/medicao.h"
#include <stdio.h>
#include <math.h>

// Simulação de dados fixos
#define IDADE 25
#define ALTURA 1.75  // metros
#define PESO 70.0    // kg
#define SEXO 1       // 1 = masculino, 0 = feminino

// Fórmula de cálculo baseada na bioimpedância
float calcular_gordura_corporal(float frequencia) {
    float impedancia = 500.0 / frequencia;  // Simulando resistência
    return (1.2 * (PESO / (ALTURA * ALTURA))) + (0.23 * IDADE) - (10.8 * SEXO) - 5.4;
}

void exibir_resultados_no_display(ssd1306_t *display, float frequencia) {
    float bf = calcular_gordura_corporal(frequencia);
    
    char buffer1[32];
    char buffer2[32];
    
    snprintf(buffer1, sizeof(buffer1), "Freq: %.2f Hz", frequencia);
    snprintf(buffer2, sizeof(buffer2), "BF: %.2f%%", bf);
    
    ssd1306_fill(display, false);
    ssd1306_draw_string(display, "Resultados:", 10, 5);
    ssd1306_draw_string(display, buffer1, 10, 20);
    ssd1306_draw_string(display, buffer2, 10, 35);
    ssd1306_send_data(display);
}
