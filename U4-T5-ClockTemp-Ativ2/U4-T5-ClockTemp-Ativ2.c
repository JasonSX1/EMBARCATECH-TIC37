#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"

// Definição dos pinos dos LEDs e botão
#define BLUE_LED 12  // LED Azul
#define RED_LED 13   // LED Vermelho
#define GREEN_LED 11 // LED Verde
#define BUTTON_A 5   // Botão A

// Estados dos LEDs
typedef enum
{
    STATE_ALL_ON,    // Todos os LEDs ligados
    STATE_GREEN_OFF, // Verde desligado
    STATE_BLUE_OFF,  // Azul desligado
    STATE_RED_OFF    // Vermelho desligado
} led_state_t;

volatile led_state_t current_state = STATE_ALL_ON;
volatile bool button_pressed = false;
volatile bool timer_running = false;

// Função para inicializar os componentes
void initAll()
{
    stdio_init_all(); // Inicializa a comunicação serial
    sleep_ms(2000);   // Aguarda inicialização da serial

    printf("Inicializando o sistema...\n");

    // Configuração dos pinos como saída
    gpio_init(BLUE_LED);
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_init(RED_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_init(GREEN_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    // Inicializa os LEDs desligados
    gpio_put(BLUE_LED, 0);
    gpio_put(RED_LED, 0);
    gpio_put(GREEN_LED, 0);
}

// Callback do temporizador
int64_t timer_callback(alarm_id_t id, void *user_data)
{
    printf("Executando callback do temporizador. Estado atual: %d\n", current_state);

    switch (current_state)
    {
    case STATE_ALL_ON:
        current_state = STATE_GREEN_OFF;
        gpio_put(GREEN_LED, 0); // Desliga o LED Verde
        break;
    case STATE_GREEN_OFF:
        current_state = STATE_BLUE_OFF;
        gpio_put(BLUE_LED, 0); // Desliga o LED Azul
        break;
    case STATE_BLUE_OFF:
        current_state = STATE_RED_OFF;
        gpio_put(RED_LED, 0);  // Desliga o LED Vermelho
        timer_running = false; // Permite novo acionamento pelo botão
        printf("Sequência concluída. Aguardando novo acionamento...\n");
        return 0; // Finaliza o temporizador
    default:
        break;
    }
    return 3000000; // Repete o alarme após 3 segundos (3.000.000 µs)
}

// Callback do botão com debounce
void button_callback(uint gpio, uint32_t events)
{
    if (gpio == BUTTON_A && !timer_running)
    {
        button_pressed = true;
    }
}

int main()
{
    initAll(); // Inicializa os componentes

    // Configura a interrupção do botão
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &button_callback);

    while (1)
    {
        if (button_pressed && !timer_running)
        {
            printf("Botão pressionado! Iniciando sequência...\n");

            button_pressed = false;
            timer_running = true;

            // Inicia a sequência de temporização
            current_state = STATE_ALL_ON;
            gpio_put(BLUE_LED, 1);
            gpio_put(RED_LED, 1);
            gpio_put(GREEN_LED, 1);
            printf("Todos os LEDs ligados. Iniciando temporizador...\n");

            add_alarm_in_ms(3000, timer_callback, NULL, false); // Primeiro atraso de 3 segundos
        }
        sleep_ms(100); // Evita uso excessivo da CPU
    }
    return 0;
}
