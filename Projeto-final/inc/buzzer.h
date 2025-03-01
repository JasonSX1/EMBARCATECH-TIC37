#ifndef BUZZER_H
#define BUZZER_H

#include "pico/stdlib.h"

#define GPIO_BUZZER 10  // Definição do pino do buzzer

void iniciar_buzzer();
void tocar_buzzer(uint frequency, uint duration);
void parar_buzzer();

#endif // BUZZER_H
