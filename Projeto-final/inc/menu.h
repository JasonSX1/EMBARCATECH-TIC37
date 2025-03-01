#ifndef MENU_H
#define MENU_H

#include "pico/stdlib.h"
#include "inc/ssd1306.h"

// Definição dos estados do menu
typedef enum {
    MENU_PRINCIPAL,
    MENU_MEDIR,
    MENU_CONFIG,
    MENU_SOBRE
} MenuState;

// Declaração de variáveis globais
extern MenuState menu_state;
extern int selected_option;
extern bool update_display;
extern const char *menu_options[];
extern const int MENU_SIZE;

// Declaração das funções do menu
void init_buttons();
void on_button_up();
void on_button_down();
void on_button_select();
void update_menu_display(ssd1306_t *ssd);

#endif // MENU_H