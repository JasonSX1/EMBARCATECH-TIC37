#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "inc/ssd1306.h"
#include "inc/font.h"
#include "inc/menu.h"
#include "inc/terminais.h"
#include "inc/buzzer.h"
#include "inc/medicao.h"
#include "inc/joystick.h"
#include "inc/entrada_usuario.h"

// Definição dos pinos
#define BUTTON_JOY 22
#define BUTTON_A 5
#define BUTTON_B 6
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_ADDR 0x3C

// Estrutura do display
ssd1306_t ssd;

// Variáveis globais
static volatile uint32_t last_press_time = 0;
bool aguardando_confirmacao = false;
extern const int MENU_SIZE;
bool medicao_realizada = false;  // Flag para garantir que a medição ocorreu pelo menos uma vez
static bool resultados_exibidos = false;

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

// Função de interrupção para seleção do menu
void button_isr(uint gpio, uint32_t events) {
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - last_press_time < 200000) return;  // Debounce de 200ms
    last_press_time = current_time;

    if (gpio == BUTTON_JOY) {
        if (menu_state == MENU_CONFIRMAR_MEDICAO) {
            // Agora inicia a medição corretamente ao confirmar
            printf("Iniciando medição...\n");
            menu_state = MENU_MEDIR;
            iniciar_medicao(&ssd);
            update_display = true;
        } else if (menu_state == MENU_EDITAR_DADO) {
            // Se estiver editando um dado, retorna à seleção de opções
            printf("Voltando para MENU_DADOS_USUARIO...\n");
            menu_state = MENU_DADOS_USUARIO;
            update_display = true;
        } else if (menu_state == MENU_DADOS_USUARIO) {
            printf("Entrando no modo de edição de dados...\n");
            menu_state = MENU_EDITAR_DADO;
            update_display = true;
        } else if (menu_state == MENU_PRINCIPAL) {
            switch (menu_index) {
                case 0:
                    menu_state = MENU_CONFIRMAR_MEDICAO;
                    exibir_confirmacao(&ssd);
                    update_display = true;
                    break;
                case 1:
                    menu_state = MENU_DADOS_USUARIO;
                    submenu_index = 0;
                    update_display = true;
                    break;
            }
        }
    }

    if (gpio == BUTTON_A) {
        if (menu_state == MENU_CONFIRMAR_MEDICAO) {
            // Se pressionar "A", volta ao menu principal sem iniciar a medição
            printf("Cancelando medição, voltando ao MENU_PRINCIPAL...\n");
            menu_state = MENU_PRINCIPAL;
            update_display = true;
        } else if (menu_state == MENU_DADOS_USUARIO) {
            menu_state = MENU_PRINCIPAL;
        } else {
            menu_state = MENU_PRINCIPAL;
        }
        update_display = true;
    }

    if (gpio == BUTTON_B) {
        reset_usb_boot(0, 0);
    }
}

int main() {
    stdio_init_all();
    adc_init();
    init_buzzer();
    init_joystick();
    iniciar_dados_usuario(); // Inicializa os dados do usuário como vazios

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
        bool editando = (menu_state == MENU_EDITAR_DADO);

        if (menu_state == MENU_DADOS_USUARIO || menu_state == MENU_EDITAR_DADO) {
            read_joystick_submenu(&submenu_index, 4, &update_display, editando, &usuario.idade, &usuario.altura, &usuario.peso, &usuario.sexo);
        } else {
            read_joystick(&menu_index, MENU_SIZE, &update_display);
        }
        
        if (update_display) {
            update_menu_display(&ssd);
            update_display = false;
        }

        if (medicao_ativa) {
            atualizar_medicao(&ssd);
            medicao_realizada = true;
        } 
        else if (medicao_realizada) {
            static bool resultados_exibidos = false;

            if (!resultados_exibidos) {
                float media_frequencia = calcular_media_frequencia();
                exibir_resultados_no_display(&ssd, media_frequencia);
                ssd1306_send_data(&ssd);
                resultados_exibidos = true;
            }

            if (gpio_get(BUTTON_A) == 0) {  // Verifica se o botão foi pressionado
                menu_state = MENU_PRINCIPAL;
                update_display = true;
                medicao_realizada = false;
                resultados_exibidos = false;
            }
        }

        atualizar_buzzer();
        sleep_ms(150);
    }
}