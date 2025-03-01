#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"

#define GPIO_JOYSTICK_X 26  // Pino do eixo X
#define GPIO_JOYSTICK_Y 27  // Pino do eixo Y
#define GPIO_JOYSTICK_BTN 28 // Pino do botão

void inicializar_joystick();
int ler_movimento_vertical();
bool botao_pressionado();

#endif // JOYSTICK_H