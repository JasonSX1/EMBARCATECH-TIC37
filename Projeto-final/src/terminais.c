#include "inc/terminais.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "pico/time.h"
#include <stdio.h>

#define MEDIA_MOVEL 5
#define GPIO_EMISSAO 0  // Pino de emissão do sinal
#define GPIO_RECEPCAO 3 // Pino de recepção do sinal
#define GPIO_ANALOGICO 7 // Pino do clique do analógico

static uint32_t tempo_ultimo_pulso = 0;
static float frequencia_instantanea = 0;
static float historico_frequencias[MEDIA_MOVEL] = {0};
static int indice_frequencia = 0;
bool medicao_ativa = false;

void iniciar_terminais() {
    gpio_init(GPIO_EMISSAO);
    gpio_set_dir(GPIO_EMISSAO, GPIO_OUT);
    gpio_put(GPIO_EMISSAO, 0);

    gpio_init(GPIO_RECEPCAO);
    gpio_set_dir(GPIO_RECEPCAO, GPIO_IN);
    
    gpio_init(GPIO_ANALOGICO);
    gpio_set_dir(GPIO_ANALOGICO, GPIO_IN);
    gpio_pull_up(GPIO_ANALOGICO);
}

void iniciar_medicao() {
    if (gpio_get(GPIO_ANALOGICO) == 0) {
        medicao_ativa = !medicao_ativa;
    }

    if (medicao_ativa) {
        printf("Medicao ativa\n");
        
        float freq_envio = 50.0;
        float freq_recebida = medir_frequencia();

        printf("F. Envio: %.2f Hz\n", freq_envio);
        printf("F. Recebida: %.2f Hz\n", freq_recebida);
    }
}

bool verificar_contato_usuario() {
    return medicao_ativa && gpio_get(GPIO_RECEPCAO) == 1;
}

float medir_frequencia() {
    if (!medicao_ativa) return 0;

    uint32_t tempo_atual = to_us_since_boot(get_absolute_time());
    static uint32_t tempo_anterior = 0;

    if (verificar_contato_usuario()) {
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
                return soma / MEDIA_MOVEL;
            }
        }
        tempo_anterior = tempo_atual;
    }
    return 0;
}
