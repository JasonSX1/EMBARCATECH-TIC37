#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
// Definição dos pinos
#define LED_G 11
#define LED_B 12
#define LED_R 13
#define BUTTON_JOY 22
#define BUTTON_A 5
#define VX_JOY 26
#define VY_JOY 27
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x3C

// Configurações PWM
#define DIVIDER_PWM 100
#define WRAP_PERIOD 20000

// Estrutura do display
ssd1306_t ssd;

// Variáveis globais
static volatile uint32_t last_press_time = 0;
static volatile bool leds_enabled = true;
static volatile char border_type = 'A';

// Configuração de pinos
void setup_gpio(uint pin, bool is_output) {
    gpio_init(pin);
    gpio_set_dir(pin, is_output ? GPIO_OUT : GPIO_IN);
    if (!is_output) gpio_pull_up(pin);
}

void setup_pwm(int pin) {
    uint slice = pwm_gpio_to_slice_num(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_set_clkdiv(slice, DIVIDER_PWM);
    pwm_set_wrap(slice, WRAP_PERIOD);
    pwm_set_gpio_level(pin, 0);
    pwm_set_enabled(slice, true);
}

// Inicialização do display
bool setup_display() {
    ssd1306_init(&ssd, 128, 64, false, I2C_ADDR, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    return true;
}

// Função de interrupção para os botões
void button_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_press_time < 300000) return;
    last_press_time = current_time;
    
    if (gpio == BUTTON_A) {
        leds_enabled = !leds_enabled;
        pwm_set_gpio_level(LED_B, 0);
        pwm_set_gpio_level(LED_R, 0);
    } else if (gpio == BUTTON_JOY) {
        gpio_put(LED_G, !gpio_get(LED_G));
        border_type = border_type == 'B' ? 'A' : border_type + 1;
    }
}

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(VX_JOY);
    adc_gpio_init(VY_JOY);

    // Inicialização do I2C com resistores pull-up
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Configuração de LEDs e botões
    setup_gpio(LED_G, true);
    setup_gpio(LED_B, true);
    setup_gpio(LED_R, true);
    setup_pwm(LED_B);
    setup_pwm(LED_R);
    setup_gpio(BUTTON_JOY, false);
    setup_gpio(BUTTON_A, false);

    // Configuração do display
    if (!setup_display()) {
        printf("Falha ao inicializar o display. Encerrando...\n");
        return -1;
    }

    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, button_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_JOY, GPIO_IRQ_EDGE_FALL, true, button_isr);

    while (true) {
        // Leitura do Joystick
        adc_select_input(1);
        uint16_t x_value = adc_read();
        adc_select_input(0);
        uint16_t y_value = adc_read();

        // Normaliza os valores para que direita seja direita e esquerda seja esquerda
        uint x_pos = (x_value * 120) / 4095;  // Mantém a inversão corrigida
        uint y_pos = 56 - ((y_value * 56) / 4095);  // Já estava certo        

        // Atualiza display
        ssd1306_fill(&ssd, false);
        
        // Desenho das bordas
        switch (border_type) {
            case 'A': 
                ssd1306_hline(&ssd, 6, 122, 2, true); 
                ssd1306_hline(&ssd, 6, 122, 61, true);
                ssd1306_vline(&ssd, 2, 4, 60, true);
                ssd1306_vline(&ssd, 125, 4, 60, true);
                ssd1306_pixel(&ssd, 5, 3, true);
                ssd1306_pixel(&ssd, 122, 3, true);
                ssd1306_pixel(&ssd, 5, 60, true);
                ssd1306_pixel(&ssd, 122, 60, true);
                break;
            case 'B': 
                ssd1306_rect(&ssd, 1, 1, 126, 62, true, false); 
                break;
        }
        
        // Desenha quadrado que representa o joystick
        ssd1306_rect(&ssd, y_pos, x_pos, 8, 8, true, true);
        ssd1306_send_data(&ssd);

        // Atualiza LEDs RGB conforme posição do joystick
        if (leds_enabled) {
            uint16_t levelX = (x_value * WRAP_PERIOD) / 4095;
            uint16_t levelY = (y_value * WRAP_PERIOD) / 4095;            
            // Garante que os valores estejam no intervalo correto
            if (levelX < 0) levelX = 0;
            if (levelY < 0) levelY = 0;        
            pwm_set_gpio_level(LED_B, levelY);
            pwm_set_gpio_level(LED_R, levelX);
        }

        sleep_ms(10);
    }
}
