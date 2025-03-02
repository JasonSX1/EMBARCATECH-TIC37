#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc/menu.h"

// Definição dos pinos
#define BUTTON_JOY 22
#define BUTTON_A 5
#define BUTTON_B 6
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x3C

#define CENTER 2048  // Valor médio esperado do joystick
#define DEADZONE 200  // Margem para detectar movimento

// Estrutura do display
ssd1306_t ssd;

// Variáveis globais
static volatile uint32_t last_press_time = 0;
static int menu_index = 0;
extern const char *menu_options[];
extern const int MENU_SIZE;

// Configuração de pinos
void setup_gpio(uint pin, bool is_output) {
    gpio_init(pin);
    gpio_set_dir(pin, is_output ? GPIO_OUT : GPIO_IN);
    if (!is_output) gpio_pull_up(pin);
}

// Inicialização do display
bool setup_display() {
    ssd1306_init(&ssd, 128, 64, false, I2C_ADDR, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
    return true;
}

// Atualiza o menu na tela
void draw_menu() {
    ssd1306_fill(&ssd, false);
    ssd1306_draw_string(&ssd, "MENU:", 10, 0);
    for (int i = 0; i < MENU_SIZE; i++) {
        if (i == menu_index) {
            ssd1306_draw_string(&ssd, ">", 5, 15 + (i * 12));  // Indicador de seleção
        }
        ssd1306_draw_string(&ssd, menu_options[i], 20, 15 + (i * 12));
    }
    ssd1306_send_data(&ssd);
}

// Leitura do joystick para navegação no menu
void read_joystick() {
    static uint16_t last_y_value = CENTER;
    adc_select_input(0);
    uint16_t y_value = adc_read();

    if (abs(y_value - last_y_value) > DEADZONE) {
        if (y_value > CENTER + DEADZONE) {
            menu_index = (menu_index + 1) % MENU_SIZE;
            update_display = true;
        } else if (y_value < CENTER - DEADZONE) {
            menu_index = (menu_index - 1 + MENU_SIZE) % MENU_SIZE;
            update_display = true;
        }
        last_y_value = y_value;
    }
}

// Função de interrupção para seleção do menu
void button_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_press_time < 300000) return;
    last_press_time = current_time;

    if (gpio == BUTTON_JOY) {
        switch (menu_index) {
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
        update_display = true;
    }
    if (gpio == BUTTON_A) {
        menu_state = MENU_PRINCIPAL; // Retorna ao menu principal
        menu_index = 0; // Reseta a seleção para o primeiro item
        update_display = true;
        draw_menu();
    }
    if (gpio == BUTTON_B) {
        reset_usb_boot(0, 0);
    }
}

int main() {
    stdio_init_all();
    adc_init();
    
    // Inicialização do I2C
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    // Configuração dos botões
    setup_gpio(BUTTON_JOY, false);
    setup_gpio(BUTTON_A, false);
    setup_gpio(BUTTON_B, false);
    
    // Inicialização do display
    if (!setup_display()) {
        printf("Falha ao inicializar o display. Encerrando...\n");
        return -1;
    }
    
    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_JOY, GPIO_IRQ_EDGE_FALL, true, button_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, button_isr);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, button_isr);
    
    while (true) {
        read_joystick(); // Leitura do joystick
        if (update_display) {
            update_menu_display(&ssd);
            update_display = false;
        }
        sleep_ms(150);
    }
}
