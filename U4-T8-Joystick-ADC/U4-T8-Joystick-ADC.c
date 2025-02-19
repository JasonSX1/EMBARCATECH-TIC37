#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"
#include "inc/font.h"

// Definição dos pinos
#define JOYSTICK_X 27
#define JOYSTICK_Y 26
#define JOYSTICK_BTN 22
#define BTN_A 5
#define BTN_B 6
#define LED_GREEN 11
#define LED_BLUE 12
#define LED_RED 13
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x3C

// Configurações do PWM
#define DIVISOR_PWM 100
#define WRAP_PERIOD 24999

// Variáveis globais
ssd1306_t display;
static volatile uint32_t last_press_time = 0;
static volatile bool leds_enabled = true;
static volatile char border_type = 'A';

// Inicialização do ADC (Joystick)
void setup_adc() {
    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);
}

// Leitura do joystick com correção de eixo
void leitura_joystick(uint16_t *x, uint16_t *y) {
    adc_select_input(1);
    *x = abs(adc_read() - 4096); // Corrige inversão do eixo X

    adc_select_input(0);
    *y = adc_read(); // Corrige inversão do eixo Y
}

// Configuração do display via I2C
bool setup_display() {
    ssd1306_init(&display, 128, 64, false, I2C_ADDR, I2C_PORT);
    ssd1306_config(&display);
    ssd1306_fill(&display, false);
    ssd1306_send_data(&display);
    return true;
}

// Atualiza o display com a posição do joystick
void atualiza_display(uint16_t x, uint16_t y) {
    ssd1306_fill(&display, false);

    // Mapeia os valores do joystick corretamente para a tela (128x64)
    uint x_pos = (x * 120) / 4095;  // Agora vai de 0 a 120 corretamente
    uint y_pos = (y * 56) / 4095;   // Agora vai de 0 a 56 corretamente

    // Inverte os eixos para corrigir a orientação do display
    uint temp = x_pos;
    x_pos = y_pos;
    y_pos = temp;

    // Evita ultrapassar a borda
    if (x_pos > 120) x_pos = 120;  // Agora chega até o final da tela
    if (y_pos > 56) y_pos = 56;    // Mantém dentro dos limites verticais

    // Desenha as bordas da tela
    switch (border_type) {
        case 'A': 
            ssd1306_hline(&display, 4, 124, 1, true);
            ssd1306_hline(&display, 4, 124, 63, true);
            break;
        case 'B': 
            ssd1306_vline(&display, 1, 0, 63, true);
            ssd1306_vline(&display, 126, 0, 63, true);
            break;
        case 'C': 
            ssd1306_hline(&display, 4, 124, 1, true);
            ssd1306_vline(&display, 126, 0, 63, true);
            break;
        case 'D': 
            ssd1306_hline(&display, 4, 124, 63, true);
            ssd1306_vline(&display, 1, 0, 63, true);
            break;
        case 'E': 
            ssd1306_rect(&display, 1, 1, 126, 62, true, false);
            break;
    }

    // Desenha o quadrado corretamente posicionado
    ssd1306_rect(&display, x_pos, y_pos, 8, 8, true, true);
    ssd1306_send_data(&display);
}


// Configuração dos botões e LEDs
void setup_gpio(uint pin, bool is_output) {
    gpio_init(pin);
    gpio_set_dir(pin, is_output ? GPIO_OUT : GPIO_IN);
    if (!is_output) gpio_pull_up(pin);
}

// Configuração do PWM para LEDs RGB
void setup_pwm(int pin) {
    uint slice = pwm_gpio_to_slice_num(pin);
    gpio_set_function(pin, GPIO_FUNC_PWM);
    pwm_set_clkdiv(slice, DIVISOR_PWM);
    pwm_set_wrap(slice, WRAP_PERIOD);
    pwm_set_gpio_level(pin, 0);
    pwm_set_enabled(slice, true);
}

// Interrupção dos botões
void button_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());

    if (current_time - last_press_time < 200000) return; // Reduzi o debounce para 200ms
    last_press_time = current_time;

    if (gpio == BTN_A) {
        leds_enabled = !leds_enabled;
        pwm_set_gpio_level(LED_BLUE, 0);
        pwm_set_gpio_level(LED_RED, 0);
    } else if (gpio == BTN_B) {
        printf("Entrando no modo BOOTSEL...\n");
        sleep_ms(100); // Pequeno delay para evitar falha
        reset_usb_boot(0, 0); // Reinicia no modo BOOTSEL
    } else if (gpio == JOYSTICK_BTN) {
        gpio_put(LED_GREEN, !gpio_get(LED_GREEN));
        border_type = border_type == 'E' ? 'A' : border_type + 1;
    }
}

// Configuração da interrupção corretamente
void setup_buttons() {
    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, button_isr);
    gpio_set_irq_enabled_with_callback(BTN_B, GPIO_IRQ_EDGE_FALL, true, button_isr);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BTN, GPIO_IRQ_EDGE_FALL, true, button_isr);
}

// Configuração geral do sistema
void setup() {
    stdio_init_all();
    setup_adc();

    // Inicializa I2C com resistores pull-up
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // Configuração dos LEDs e botões
    setup_gpio(LED_GREEN, true);
    setup_gpio(LED_BLUE, true);
    setup_gpio(LED_RED, true);
    setup_pwm(LED_BLUE);
    setup_pwm(LED_RED);
    setup_gpio(JOYSTICK_BTN, false);
    setup_gpio(BTN_A, false);

    // Configuração do display
    if (!setup_display()) {
        printf("Falha ao inicializar o display. Encerrando...\n");
        return;
    }

    gpio_set_irq_enabled_with_callback(BTN_A, GPIO_IRQ_EDGE_FALL, true, button_isr);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BTN, GPIO_IRQ_EDGE_FALL, true, button_isr);
}

// Loop principal
int main() {
    setup();
    uint16_t x, y;

    while (true) {
        leitura_joystick(&x, &y);
        atualiza_display(x, y);
        printf("Valor X: %d | Valor Y: %d\n", x, y);

        if (leds_enabled) {
            uint16_t levelX = (x == 0 || x == 2048) ? 0 : ((abs(x - 2048) * WRAP_PERIOD) / 2048);
            uint16_t levelY = (y == 0 || y == 2048) ? 0 : ((abs(y - 2048) * WRAP_PERIOD) / 2048);
            pwm_set_gpio_level(LED_BLUE, levelY);
            pwm_set_gpio_level(LED_RED, levelX);
        }

        sleep_ms(10);
    }
}
