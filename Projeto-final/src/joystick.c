#include "inc/joystick.h"
#include "hardware/adc.h"
#include <stdlib.h>

void init_joystick() {
    adc_init();
    adc_gpio_init(26);  // Entrada do joystick no ADC (confirme qual GPIO correto)
}

void read_joystick(int *menu_index, int menu_size, bool *update_display) {
    static uint16_t last_y_value = CENTER;
    static uint32_t last_move_time = 0;  // Debounce de tempo

    adc_select_input(0);
    uint16_t y_value = adc_read();
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (current_time - last_move_time < 200) return; // Debounce de 200ms

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
