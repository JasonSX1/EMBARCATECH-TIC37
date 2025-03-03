#ifndef TERMINAIS_H
#define TERMINAIS_H

#include <stdbool.h>
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"

void iniciar_medicao(ssd1306_t *display);
void atualizar_medicao(ssd1306_t *display);
void medir_frequencia_instantanea();

#endif // TERMINAIS_H