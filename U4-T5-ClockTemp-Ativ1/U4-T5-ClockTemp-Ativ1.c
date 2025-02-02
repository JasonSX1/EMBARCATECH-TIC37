#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define RED_LED 11    // GPIO para o LED vermelho
#define YELLOW_LED 12 // GPIO para o LED amarelo
#define GREEN_LED 13  // GPIO para o LED verde

volatile int state = 0; // Faz com que o semáforo comece no vermelho

// Função para inicializar os LEDs
void initAll() {
    stdio_init_all(); // Inicializa a comunicação serial via UART/USB
    sleep_ms(2000);   // Aguarda inicialização da serial - tempo padrão para evitar problemas entre diferentes sistemas

    // Configuração dos pinos como saída
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_init(YELLOW_LED);
    gpio_set_dir(YELLOW_LED, GPIO_OUT);
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);

    // Inicializa o semáforo no vermelho
    gpio_put(RED_LED, 1);
    gpio_put(YELLOW_LED, 0);
    gpio_put(GREEN_LED, 0);
}

// Função de callback para o timer
bool repeating_timer_callback(struct repeating_timer *t)
{
    printf("Passaram-se 3 segundos...\n");
    switch (state) {
    case 0:
        gpio_put(RED_LED, 1);
        gpio_put(YELLOW_LED, 0);
        gpio_put(GREEN_LED, 0);
        state = 1;
        break;
    case 1:
        gpio_put(RED_LED, 0);
        gpio_put(YELLOW_LED, 1);
        gpio_put(GREEN_LED, 0);
        state = 2;
        break;
    case 2:
        gpio_put(RED_LED, 0);
        gpio_put(YELLOW_LED, 0);
        gpio_put(GREEN_LED, 1);
        state = 0;
        break;
    }
    return true;
}

int main() {
    initAll(); // Chama a função de inicialização dos componentes

    struct repeating_timer timer;
    add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer); // Chama a cada 3s

    while (1) {
        printf("Semaforo trabalhando...\n");
        sleep_ms(1000); // Mensagem a cada 1s
    }
}