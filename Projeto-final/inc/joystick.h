#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"

#define CENTER 2048  // Valor médio esperado do joystick
#define DEADZONE 800  // Margem para detectar movimento

void init_joystick();  // Inicializa o joystick
void read_joystick(int *menu_index, int menu_size, bool *update_display);

#endif
