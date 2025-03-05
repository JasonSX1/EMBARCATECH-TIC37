#include "inc/terminais.h"
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"
#include "inc/buzzer.h"
#include "inc/medicao.h"
#include <stdio.h>

#define GPIO_EMISSAO 0  // Pino de emissão do sinal
#define GPIO_RECEPCAO 3 // Pino de recepção do sinal
#define GPIO_BOTAO_A 5  // Pino do botão A
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDRESS 0x3C
#define DURACAO_MEDICAO 30000 // 30 segundos em milissegundos
#define POLLING_RATE 1 // Polling mais rápido para capturar mudanças
#define TIMEOUT_FREQUENCIA 500 // Tempo limite para resetar a frequência
#define MEDIA_MOVEL 5 // Número de amostras para suavização

ssd1306_t display;
static uint32_t tempo_ultimo_pulso = 0;
static float frequencia_instantanea = 0;
static float historico_frequencias[MEDIA_MOVEL] = {0};
static int indice_frequencia = 0;
bool medicao_ativa = false;
uint32_t tempo_inicio_medicao = 0;

void iniciar_sinal_pwm(uint gpio, uint freq, uint duty_cycle) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_set_wrap(slice_num, 12500);
    pwm_set_chan_level(slice_num, PWM_CHAN_A, duty_cycle);
    pwm_set_enabled(slice_num, true);
}

void atualizar_display(const char *mensagem) {
    printf("Atualizando display: %s\n", mensagem);
    ssd1306_fill(&display, 0);
    ssd1306_draw_string(&display, mensagem, 0, 0);
    ssd1306_send_data(&display);
}

void medir_frequencia_instantanea() {
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    static uint32_t tempo_anterior = 0;

    if (gpio_get(GPIO_RECEPCAO) == 1) {
        if (tempo_anterior != 0) {
            uint32_t periodo = tempo_atual - tempo_anterior;
            if (periodo > 0) {
                float freq_atual = 1000000.0 / periodo; // Convertendo período para frequência em Hz
                historico_frequencias[indice_frequencia] = freq_atual;
                indice_frequencia = (indice_frequencia + 1) % MEDIA_MOVEL;

                float soma = 0;
                for (int i = 0; i < MEDIA_MOVEL; i++) {
                    soma += historico_frequencias[i];
                }
                frequencia_instantanea = soma / MEDIA_MOVEL; // Média móvel para suavizar valores
            }
        }
        tempo_anterior = tempo_atual;
        tempo_ultimo_pulso = to_ms_since_boot(get_absolute_time()); // Atualiza tempo da última detecção
    }
}

float calcular_media_frequencia() {
    float soma = 0;
    int contagem = 0;
    
    for (int i = 0; i < MEDIA_MOVEL; i++) {
        if (historico_frequencias[i] > 0) {  // Ignora valores inválidos
            soma += historico_frequencias[i];
            contagem++;
        }
    }

    return (contagem > 0) ? (soma / contagem) : 0;  // Retorna a média ou 0 se não houver dados suficientes
}

void iniciar_medicao(ssd1306_t *display) {
    tocar_som_preenchimento();
    if (!medicao_ativa) {
        medicao_ativa = true;
        tempo_inicio_medicao = to_ms_since_boot(get_absolute_time());
        iniciar_sinal_pwm(GPIO_EMISSAO, 1000, 6250);
        gpio_init(GPIO_RECEPCAO);
        gpio_set_dir(GPIO_RECEPCAO, GPIO_IN);
    }
}

void atualizar_medicao(ssd1306_t *display) {
    if (!medicao_ativa) return; // Se a medição foi interrompida, sai imediatamente

    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - tempo_inicio_medicao >= DURACAO_MEDICAO) {
        medicao_ativa = false;
        pwm_set_enabled(pwm_gpio_to_slice_num(GPIO_EMISSAO), false);
        parar_som_preenchimento();
    
        // Exibir resultados corretamente antes de retornar ao menu
        float media_frequencia = calcular_media_frequencia();
        exibir_resultados_no_display(display, media_frequencia);
    }
    

    medir_frequencia_instantanea();
    
    if (tempo_atual - tempo_ultimo_pulso > TIMEOUT_FREQUENCIA) {
        frequencia_instantanea = 0;
    }

    char buffer[64];
    snprintf(buffer, sizeof(buffer), "Freq: %.2f Hz", frequencia_instantanea);
    printf("%s\n", buffer);
    ssd1306_fill(display, false);
    ssd1306_draw_string(display, buffer, 10, 20);
    ssd1306_send_data(display);
}
