#include <stdio.h>
#include <math.h>
#include "inc/medicao.h"
#include "inc/entrada_usuario.h"  // Para acessar os dados do usuário

extern DadosUsuario usuario;  // Declaração externa da estrutura

// Fórmula de cálculo baseada na bioimpedância
float calcular_gordura_corporal(float frequencia) {
    float impedancia = 500.0 / frequencia;  // Simulando resistência
    return (1.2 * (usuario.peso / ((usuario.altura / 100.0) * (usuario.altura / 100.0)))) +
           (0.23 * usuario.idade) - (10.8 * usuario.sexo) - 5.4;
}
void exibir_resultados_no_display(ssd1306_t *display, float frequencia) {
    float bf = calcular_gordura_corporal(frequencia);
    
    char buffer1[32];
    char buffer2[32];
    
    snprintf(buffer2, sizeof(buffer2), "BF aprox %.2f%%", bf);
    
    ssd1306_fill(display, false);
    ssd1306_draw_string(display, "Resultados", 10, 5);
    ssd1306_draw_string(display, buffer2, 10, 35);
    ssd1306_send_data(display);
}
