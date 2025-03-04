#include "inc/joystick.h"
#include "hardware/adc.h"
#include <stdlib.h>
#include "math.h"

// Definição dos canais ADC do joystick
#define JOY_X 1  // Canal ADC para eixo X
#define JOY_Y 0  // Canal ADC para eixo Y

// Variáveis para controle da aceleração ao segurar o analógico
static uint32_t last_x_change_time = 0;
static uint32_t last_y_change_time = 0;
static uint32_t acceleration_delay = 300;  // Tempo inicial entre mudanças (ms)
static uint32_t holding_time = 0;  // Tempo segurando o analógico

void init_joystick() {
    adc_init();
    adc_gpio_init(26); // Entrada do joystick no ADC Y
    adc_gpio_init(27); // Entrada do joystick no ADC X
}

void read_joystick(int *menu_index, int menu_size, bool *update_display) {
    static uint16_t last_y_value = CENTER;
    static uint32_t last_move_time = 0;  // Debounce de tempo

    adc_select_input(0);
    uint16_t y_value = adc_read();
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time - last_move_time < 100) return; // Debounce de 100ms

    if (abs(y_value - last_y_value) > DEADZONE) {
        if (y_value < CENTER - DEADZONE) {
            *menu_index = (*menu_index + 1) % menu_size;
            *update_display = true;
        } else if (y_value > CENTER + DEADZONE) {
            *menu_index = (*menu_index - 1 + menu_size) % menu_size;
            *update_display = true;
        }
        last_y_value = y_value;
        last_move_time = current_time;
    }
}

// Função para navegação no submenu e edição de valores
void read_joystick_submenu(int *campo_atual, int total_campos, bool *update_display, bool editando, int *idade, int *altura, float *peso, int *sexo) {
    static uint16_t last_y_value = CENTER;
    static uint16_t last_x_value = CENTER;

    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    adc_select_input(JOY_Y);
    uint16_t y_value = adc_read();

    adc_select_input(JOY_X);
    uint16_t x_value = adc_read();

    if (!editando && abs(y_value - last_y_value) > DEADZONE) {  
        if (current_time - last_y_change_time > 250) {
            if (y_value < CENTER - DEADZONE) {
                *campo_atual = (*campo_atual + 1) % total_campos;
            } else if (y_value > CENTER + DEADZONE) {
                *campo_atual = (*campo_atual - 1 + total_campos) % total_campos;
            }
            *update_display = true;
            last_y_change_time = current_time;
        }
    }
    last_y_value = y_value;

    if (editando) {
        int *valor_modificado = NULL;
        switch (*campo_atual) {
            case 0: valor_modificado = idade; break;
            case 1: valor_modificado = altura; break;
            case 2: valor_modificado = (int*)peso; break;
            case 3: valor_modificado = sexo; break;
        }

        bool analogico_movido = abs(x_value - last_x_value) > DEADZONE;
        if (analogico_movido) {
            holding_time = current_time;
            last_x_value = x_value;
        }

        if (valor_modificado && analogico_movido && (current_time - last_x_change_time > 150)) {
            if (x_value < CENTER - DEADZONE) {
                if (*campo_atual == 2) { 
                    if (*(float*)valor_modificado > 0.0f) {
                        *(float*)valor_modificado -= 0.1f;
                    }
                } else if (*campo_atual == 3) { // Limitar sexo entre 0 (Masculino) e 1 (Feminino)
                    if (*valor_modificado > 0) {
                        (*valor_modificado) -= 1;
                    }
                } else {
                    if (*valor_modificado > 0) {
                        (*valor_modificado) -= 1;
                    }
                }
            } else if (x_value > CENTER + DEADZONE) {
                if (*campo_atual == 2) { 
                    *(float*)valor_modificado += 0.1f;
                } else if (*campo_atual == 3) { // Limitar sexo entre 0 e 1
                    if (*valor_modificado < 1) {
                        (*valor_modificado) += 1;
                    }
                } else {
                    (*valor_modificado) += 1;
                }
            }
            *update_display = true;
            last_x_change_time = current_time;
        }

        if (valor_modificado && (current_time - holding_time > acceleration_delay)) {
            if (x_value < CENTER - DEADZONE) {
                if (*campo_atual == 2) {
                    if (*(float*)valor_modificado > 0.0f) {
                        *(float*)valor_modificado = floor(*(float*)valor_modificado) - 1;
                    }
                } else if (*campo_atual == 3) { // Evita sair do intervalo 0-1
                    if (*valor_modificado > 0) {
                        (*valor_modificado) -= 1;
                    }
                } else {
                    if (*valor_modificado > 0) {
                        (*valor_modificado) -= 2;
                    }
                }
            } else if (x_value > CENTER + DEADZONE) {
                if (*campo_atual == 2) {
                    *(float*)valor_modificado = ceil(*(float*)valor_modificado) + 1;
                } else if (*campo_atual == 3) { // Evita sair do intervalo 0-1
                    if (*valor_modificado < 1) {
                        (*valor_modificado) += 1;
                    }
                } else {
                    (*valor_modificado) += 2;
                }
            }
            *update_display = true;
            holding_time = current_time;

            if (acceleration_delay > 30) acceleration_delay -= 20;
        }

        if (abs(x_value - CENTER) < DEADZONE) {
            acceleration_delay = 300;
            holding_time = 0;
        }
    }
}