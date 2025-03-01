#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "inc/menu.h"
#include "inc/ssd1306.h"

// Definição dos pinos dos botões
#define BUTTON_UP 5
#define BUTTON_DOWN 6
#define BUTTON_SELECT 22

// Estado atual do menu
MenuState menu_state = MENU_PRINCIPAL;
int selected_option = 0;
bool update_display = true;

// Opções do menu
const char *menu_options[] = {
    "Medir Bioimpedancia",
    "Configuracoes",
    "Sobre"
};

const int MENU_SIZE = sizeof(menu_options) / sizeof(menu_options[0]);

// Inicializa os botões e suas interrupções
void init_buttons() {
    gpio_init(BUTTON_UP);
    gpio_set_dir(BUTTON_UP, GPIO_IN);
    gpio_pull_up(BUTTON_UP);
    gpio_set_irq_enabled_with_callback(BUTTON_UP, GPIO_IRQ_EDGE_FALL, true, &on_button_up);

    gpio_init(BUTTON_DOWN);
    gpio_set_dir(BUTTON_DOWN, GPIO_IN);
    gpio_pull_up(BUTTON_DOWN);
    gpio_set_irq_enabled_with_callback(BUTTON_DOWN, GPIO_IRQ_EDGE_FALL, true, &on_button_down);

    gpio_init(BUTTON_SELECT);
    gpio_set_dir(BUTTON_SELECT, GPIO_IN);
    gpio_pull_up(BUTTON_SELECT);
    gpio_set_irq_enabled_with_callback(BUTTON_SELECT, GPIO_IRQ_EDGE_FALL, true, &on_button_select);
}

void on_button_up() {
    if (menu_state == MENU_PRINCIPAL) {
        selected_option = (selected_option - 1 + MENU_SIZE) % MENU_SIZE;
        update_display = true;
    }
}

void on_button_down() {
    if (menu_state == MENU_PRINCIPAL) {
        selected_option = (selected_option + 1) % MENU_SIZE;
        update_display = true;
    }
}

void on_button_select() {
    switch (menu_state) {
        case MENU_PRINCIPAL:
            switch (selected_option) {
                case 0:
                    menu_state = MENU_MEDIR;
                    break;
                case 1:
                    menu_state = MENU_CONFIG;
                    break;
                case 2:
                    menu_state = MENU_SOBRE;
                    break;
            }
            break;
        default:
            menu_state = MENU_PRINCIPAL;
            break;
    }
    update_display = true;
}

void update_menu_display(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);

    switch (menu_state) {
        case MENU_PRINCIPAL:
            ssd1306_draw_string(ssd, "MENU:", 10, 0);
            for (int i = 0; i < MENU_SIZE; i++) {
                if (i == selected_option) {
                    ssd1306_draw_string(ssd, "> ", 5, 15 + (i * 12));
                }
                ssd1306_draw_string(ssd, menu_options[i], 20, 15 + (i * 12));
            }
            break;
        case MENU_MEDIR:
            ssd1306_draw_string(ssd, "Modo: Medicao", 10, 20);
            break;
        case MENU_CONFIG:
            ssd1306_draw_string(ssd, "Configuracoes", 10, 20);
            break;
        case MENU_SOBRE:
            ssd1306_draw_string(ssd, "Sobre este projeto", 10, 10);
            ssd1306_draw_string(ssd, "Bioimpedancia V1.0", 10, 20);
            break;
    }
    ssd1306_send_data(ssd);
}
