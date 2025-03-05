#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "pico/stdlib.h"

#define CENTER 2048  // Valor médio esperado do joystick
#define DEADZONE 800  // Margem para detectar movimento

void init_joystick();  // Inicializa o joystick
void read_joystick(int *menu_index, int menu_size, bool *update_display);
void read_joystick_submenu(int *campo_atual, int total_campos, bool *update_display, bool editando, int *idade, int *altura, float *peso, int *sexo);

#endif
