#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "inc/buzzer.h"

#define BUZZER_PIN 10  // Defina o pino correto para seu buzzer

// Estado do buzzer
typedef enum {
    BUZZER_IDLE,       // Inativo
    BUZZER_NOTIFICACAO // Emitindo beep curto
} BuzzerState;

static volatile BuzzerState buzzer_state = BUZZER_IDLE;
static volatile uint32_t tempo_inicio_som = 0;
static uint8_t beep_etapa = 0;

// Inicializa o pino do buzzer como saída
void init_buzzer() {
    gpio_init(BUZZER_PIN);
    gpio_set_dir(BUZZER_PIN, GPIO_OUT);
}

// Gera um tom específico no buzzer por um tempo determinado (não bloqueante)
void iniciar_buzzer(uint32_t freq_hz) {
    uint32_t period_us = 1000000 / freq_hz;
    gpio_put(BUZZER_PIN, true);
    sleep_us(period_us / 2);
    gpio_put(BUZZER_PIN, false);
    sleep_us(period_us / 2);
}

// Inicia o som de notificação (assíncrono)
void tocar_notificacao() {
    buzzer_state = BUZZER_NOTIFICACAO;
    tempo_inicio_som = to_ms_since_boot(get_absolute_time());
    beep_etapa = 0;
}

// Atualiza o som do buzzer (chamar no loop principal)
void atualizar_buzzer() {
    if (buzzer_state == BUZZER_IDLE) return; // Nada para fazer

    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());

    switch (buzzer_state) {
        case BUZZER_NOTIFICACAO:
            if (beep_etapa == 0 && tempo_atual - tempo_inicio_som >= 0) {
                iniciar_buzzer(5000);  // Primeira nota (1000Hz)
                beep_etapa = 1;
                tempo_inicio_som = tempo_atual;
            } else if (beep_etapa == 1 && tempo_atual - tempo_inicio_som >= 50) {
                iniciar_buzzer(8000);  // Segunda nota (1500Hz)
                beep_etapa = 2;
                tempo_inicio_som = tempo_atual;
            } else if (beep_etapa == 2 && tempo_atual - tempo_inicio_som >= 50) {
                gpio_put(BUZZER_PIN, false); // Desliga o buzzer
                buzzer_state = BUZZER_IDLE;
            }
            break;

        default:
            buzzer_state = BUZZER_IDLE;
            break;
    }
}
