#ifndef MENU_H
#define MENU_H

#include "pico/stdlib.h"
#include "inc/ssd1306.h"

// Estado do menu
typedef enum {
    MENU_PRINCIPAL,
    MENU_MEDIR,
    MENU_CONFIG,
    MENU_SOBRE
} MenuState;

// Declaração das variáveis globais (apenas como referência, sem definir!)
extern MenuState menu_state;
extern bool update_display;
extern int menu_index;

// Funções do menu
void init_buttons();
void on_button_up();
void on_button_down();
void on_button_select();
void update_menu_display(ssd1306_t *ssd);

#endif
