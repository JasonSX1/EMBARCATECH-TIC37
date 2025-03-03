#ifndef TERMINAIS_H
#define TERMINAIS_H

#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"

// Variável global para indicar se a medição está ativa
extern bool medicao_ativa;
#define GPIO_EMISSAO 0  // Pino de emissão do sinal
#define GPIO_RECEPCAO 3 // Pino de recepção do sinal

void iniciar_medicao(ssd1306_t *display);
void atualizar_medicao(ssd1306_t *display);
void medir_frequencia_instantanea();

#endif
