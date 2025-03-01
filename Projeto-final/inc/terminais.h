#ifndef TERMINAIS_H
#define TERMINAIS_H

#include "pico/stdlib.h"

#define GPIO_EMISSAO 0  // Pino de emissão do sinal
#define GPIO_RECEPCAO 3 // Pino de recepção do sinal

void iniciar_terminais();
float medir_frequencia();

#endif // TERMINAIS_H