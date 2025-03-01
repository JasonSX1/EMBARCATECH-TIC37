#include "inc/joystick.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

void inicializar_joystick() {
    adc_init();
    adc_gpio_init(GPIO_JOYSTICK_X);
    adc_gpio_init(GPIO_JOYSTICK_Y);
    
    gpio_init(GPIO_JOYSTICK_BTN);
    gpio_set_dir(GPIO_JOYSTICK_BTN, GPIO_IN);
    gpio_pull_up(GPIO_JOYSTICK_BTN);
}

int ler_movimento_vertical() {
    adc_select_input(1);  // Lê o eixo Y
    uint16_t valor = adc_read();
    
    if (valor < 1000) return -1; // Movimento para cima
    if (valor > 3000) return 1;  // Movimento para baixo
    return 0; // Sem movimento
}

bool botao_pressionado() {
    return gpio_get(GPIO_JOYSTICK_BTN) == 0;
}