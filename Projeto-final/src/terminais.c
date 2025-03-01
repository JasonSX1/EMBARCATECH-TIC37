#include "inc/terminais.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include <stdio.h>

#define MEDIA_MOVEL 5

static uint32_t tempo_ultimo_pulso = 0;
static float frequencia_instantanea = 0;
static float historico_frequencias[MEDIA_MOVEL] = {0};
static int indice_frequencia = 0;

void iniciar_terminais() {
    gpio_init(GPIO_RECEPCAO);
    gpio_set_dir(GPIO_RECEPCAO, GPIO_IN);
}

float medir_frequencia() {
    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    static uint32_t tempo_anterior = 0;

    if (gpio_get(GPIO_RECEPCAO) == 1) {
        if (tempo_anterior != 0) {
            uint32_t periodo = tempo_atual - tempo_anterior;
            if (periodo > 0) {
                float freq_atual = 1000000.0 / periodo;
                historico_frequencias[indice_frequencia] = freq_atual;
                indice_frequencia = (indice_frequencia + 1) % MEDIA_MOVEL;

                float soma = 0;
                for (int i = 0; i < MEDIA_MOVEL; i++) {
                    soma += historico_frequencias[i];
                }
                frequencia_instantanea = soma / MEDIA_MOVEL;
            }
        }
        tempo_anterior = tempo_atual;
        tempo_ultimo_pulso = to_ms_since_boot(get_absolute_time());
    }

    return frequencia_instantanea;
}