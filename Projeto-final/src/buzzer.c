#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "inc/buzzer.h"

#define BUZZER_PIN 10  // Defina o pino correto para seu buzzer

// Estado do buzzer
typedef enum {
    BUZZER_IDLE,       
    BUZZER_NOTIFICACAO
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
void iniciar_buzzer(uint32_t freq_hz, uint32_t duration_ms) {
    uint32_t period_us = 1000000 / freq_hz;
    uint32_t half_period = period_us / 2;
    uint32_t end_time = time_us_32() + (duration_ms * 1000); 

    while (time_us_32() < end_time) {
        gpio_put(BUZZER_PIN, true);
        sleep_us(half_period);
        gpio_put(BUZZER_PIN, false);
        sleep_us(half_period);
    }
}

// Inicia o som de notificação (sweep eletrônico)
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
                iniciar_buzzer(1200, 30);  // Primeira nota (1200Hz, 30ms)
                beep_etapa = 1;
                tempo_inicio_som = tempo_atual;
            }
            break;

        default:
            buzzer_state = BUZZER_IDLE;
            break;
    }
}
