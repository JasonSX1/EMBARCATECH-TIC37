#ifndef MENU_H
#define MENU_H

#include "pico/stdlib.h"
#include "inc/ssd1306.h"

// Estado do menu
typedef enum {
    MENU_PRINCIPAL,
    MENU_MEDIR,
    MENU_CONFIRMAR_MEDICAO,
    MENU_DADOS_USUARIO,
    MENU_EDITAR_DADO
} MenuState;

// Declaração das variáveis globais
extern MenuState menu_state;
extern bool update_display;
extern int menu_index;
extern int submenu_index;

// Funções do menu
void init_buttons();
void on_button_up();
void on_button_down();
void on_button_select();
void update_menu_display(ssd1306_t *ssd);
void exibir_confirmacao(ssd1306_t *ssd);
void menu_medir(ssd1306_t *ssd);
void menu_dados_usuario(ssd1306_t *ssd);
void menu_editar_dado(ssd1306_t *ssd);

#endif
