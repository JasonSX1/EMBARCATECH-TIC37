#ifndef MEDICAO_H
#define MEDICAO_H

#include "inc/ssd1306.h"

// Função para calcular a composição corporal
float calcular_gordura_corporal(float frequencia);

// Função para exibir os resultados diretamente no display OLED
void exibir_resultados_no_display(ssd1306_t *display, float frequencia);

#endif // MEDICAO_H
