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
#define TEMPO_LIMITE_MEDICAO 5000  // Tempo máximo sem contato (5 segundos)

// Estrutura do display
ssd1306_t ssd;

// Variáveis globais
extern const int MENU_SIZE;
static volatile uint32_t last_press_time = 0;
static bool resultados_exibidos = false;
static uint32_t tempo_inicio_medicao = 0;  // Armazena o tempo de início da medição
bool medicao_realizada = false;  // Flag para garantir que a medição ocorreu pelo menos uma vez
bool aguardando_confirmacao = false;

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
            // Verificar se os dados estão completos antes de iniciar a medição
            if (usuario.idade <= 0 || usuario.altura <= 0 || usuario.peso <= 0.0 || (usuario.sexo != 0 && usuario.sexo != 1)) {
                printf("Erro: Dados do usuário incompletos!\n");

                // Emite som de erro
                tocar_som_erro();

                // Exibe a mensagem no display
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, "Erro", 50, 10);
                ssd1306_draw_string(&ssd, "Preencha os", 20, 25);
                ssd1306_draw_string(&ssd, "dados antes", 20, 40);
                ssd1306_draw_string(&ssd, "de medir", 30, 55);
                ssd1306_send_data(&ssd);

                // Aguarda o usuário pressionar o botão SELECT para continuar
                for (int i = 0; i < 20 && gpio_get(BUTTON_JOY); i++) {
                    sleep_ms(100);  // Aguarda no máximo 2 segundos
                }

                // Redireciona o usuário ao menu de edição de dados
                menu_state = MENU_DADOS_USUARIO;
                return;
            }

            // Iniciar a medição somente se os dados estiverem completos
            menu_state = MENU_MEDIR;
            iniciar_medicao(&ssd);
            update_display = true;
        } else if (menu_state == MENU_EDITAR_DADO) {
            menu_state = MENU_DADOS_USUARIO;
            update_display = true;
        } else if (menu_state == MENU_DADOS_USUARIO) {
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
        if (menu_state == MENU_MEDIR) {
            // Interrompe a medição imediatamente
            medicao_ativa = false;
            parar_som_preenchimento();
            menu_state = MENU_PRINCIPAL;
            update_display = true;
        } else {
            menu_state = MENU_PRINCIPAL;
            update_display = true;
        }
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
            uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
        
            // Garante que a medição não comece com erro imediato
            if (tempo_inicio_medicao == 0) {
                tempo_inicio_medicao = tempo_atual;
            }
        
            // Se a frequência for maior que 0, significa que há contato → Reinicia o contador
            if (frequencia_instantanea > 0) {  
                tempo_inicio_medicao = tempo_atual;
                printf("Contato detectado. Reiniciando contador de inatividade.\n");
            }
        
            atualizar_medicao(&ssd);
            medicao_realizada = true;
        
            // Se a frequência ficar em zero por 5 segundos, cancela a medição
            if (tempo_atual - tempo_inicio_medicao >= TEMPO_LIMITE_MEDICAO) {
                printf("Medição cancelada por inatividade!\n");
        
                // Toca som de erro antes de cancelar
                tocar_som_erro();
        
                // Exibir mensagem de erro no display
                ssd1306_fill(&ssd, false);
                ssd1306_draw_string(&ssd, "Erro", 50, 10);
                ssd1306_draw_string(&ssd, "Sem contato", 20, 25);
                ssd1306_draw_string(&ssd, "por 5s", 30, 40);
                ssd1306_draw_string(&ssd, "Cancelando...", 15, 55);
                ssd1306_send_data(&ssd);
                sleep_ms(2000);
        
                // Cancela a medição e retorna ao menu principal
                medicao_ativa = false;
                medicao_realizada = false;
                tempo_inicio_medicao = 0;
                menu_state = MENU_PRINCIPAL;
                update_display = true;
            }
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
