#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"

// Definição de entradas e saídas conectadas às GPIOs
#define RED_LED 13
#define GREEN_LED 11
#define BLUE_LED 12
#define BUTTON_A 5
#define BUTTON_B 6

// Definição do número de frames da "animação" que exibe os números
#define ANIMATION_FRAMES 10

// Definição do número de LEDs e do pino
#define LEDS_COUNT 25
#define MATRIX_PIN 7

// Definição da luminosidade dos LEDs (0 a 1)
#define BRIGHTNESS 0.01

// Variáveis para uso da máquina PIO
PIO np_pio;
uint sm;

// Definição do pixel GRB (ordem de cores do WS2812B)
struct pixel_t {
    uint8_t g, r, b;
};

typedef struct pixel_t pixel_t; // Definição do tipo pixel_t

// Declaração do buffer de pixels que formam a matriz
pixel_t leds[LEDS_COUNT];

// Definição de tempo de debounce em ms
#define DEBOUNCE_TIME_MS 600

// Variáveis globais para controle do número exibido e estado dos botões
volatile uint8_t number = 0;
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;
volatile uint32_t last_button_a_time = 0;
volatile uint32_t last_button_b_time = 0;

//Função para calcular o índice do LED na matriz 5x5.
int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return 24 - (y * 5 + x);
    } else {
        return 24 - (y * 5 + (4 - x));
    }
}

//Inicialização da máquina de estados para controle dos LEDs
void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }

    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    for (uint i = 0; i < LEDS_COUNT; ++i) {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
    }
}

//Callback para interrupção dos botões
void gpio_callback(uint gpio, uint32_t events) {
    uint32_t current_time = to_ms_since_boot(get_absolute_time());

    if (gpio == BUTTON_A) {
        if (current_time - last_button_a_time > DEBOUNCE_TIME_MS) {
            button_a_pressed = true;
            last_button_a_time = current_time;
        }
    } else if (gpio == BUTTON_B) {
        if (current_time - last_button_b_time > DEBOUNCE_TIME_MS) {
            button_b_pressed = true;
            last_button_b_time = current_time;
        }
    }
}

//Função para exibir um número na matriz de LEDs.
void display_number(uint8_t number) {
    // Garante que o número está dentro dos limites válidos
    if (number >= ANIMATION_FRAMES) {
        return;
    }

    // Desenho dos números na matriz de LEDs (0 a 9)
    const int numbers_animation[10][5][5][3] = {
{
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}},     
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 0, 0}},
    {{255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}, {255, 0, 0}}      
},

{
    {{0, 0, 0}, {0, 0, 0}, {230, 42, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {230, 42, 0}, {230, 42, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {230, 42, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {0, 0, 0}, {230, 42, 0}, {0, 0, 0}, {0, 0, 0}},
    {{0, 0, 0}, {230, 42, 0}, {230, 42, 0}, {230, 42, 0}, {0, 0, 0}}       
},

{
    {{204, 85, 0}, {204, 85, 0}, {204, 85, 0}, {204, 85, 0}, {204, 85, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {204, 85, 0}},
    {{204, 85, 0}, {204, 85, 0}, {204, 85, 0}, {204, 85, 0}, {204, 85, 0}},
    {{204, 85, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{204, 85, 0}, {204, 85, 0}, {204, 85, 0}, {204, 85, 0}, {204, 85, 0}}
},

{
    {{255, 128, 0}, {255, 128, 0}, {255, 128, 0}, {255, 128, 0}, {255, 128, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 128, 0}},
    {{0, 0, 0}, {255, 128, 0}, {255, 128, 0}, {255, 128, 0}, {255, 128, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 128, 0}},
    {{255, 128, 0}, {255, 128, 0}, {255, 128, 0}, {255, 128, 0}, {255, 128, 0}}
},

{
    {{255, 170, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 170, 0}},
    {{255, 170, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 170, 0}},
    {{255, 170, 0}, {255, 170, 0}, {255, 170, 0}, {255, 170, 0}, {255, 170, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 170, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 170, 0}}
},

{
    {{255, 212, 0}, {255, 212, 0}, {255, 212, 0}, {255, 212, 0}, {255, 212, 0}},
    {{255, 212, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{255, 212, 0}, {255, 212, 0}, {255, 212, 0}, {255, 212, 0}, {255, 212, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {255, 212, 0}},
    {{255, 212, 0}, {255, 212, 0}, {255, 212, 0}, {255, 212, 0}, {255, 212, 0}}
},

{
    {{170, 255, 0}, {170, 255, 0}, {170, 255, 0}, {170, 255, 0}, {170, 255, 0}},
    {{170, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
    {{170, 255, 0}, {170, 255, 0}, {170, 255, 0}, {170, 255, 0}, {170, 255, 0}},
    {{170, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {170, 255, 0}},
    {{170, 255, 0}, {170, 255, 0}, {170, 255, 0}, {170, 255, 0}, {170, 255, 0}}
},

{
    {{85, 255, 0}, {85, 255, 0}, {85, 255, 0}, {85, 255, 0}, {85, 255, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {85, 255, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {85, 255, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {85, 255, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {85, 255, 0}}
},

{
    {{42, 255, 0}, {42, 255, 0}, {42, 255, 0}, {42, 255, 0}, {42, 255, 0}},
    {{42, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {42, 255, 0}},
    {{42, 255, 0}, {42, 255, 0}, {42, 255, 0}, {42, 255, 0}, {42, 255, 0}},
    {{42, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {42, 255, 0}},
    {{42, 255, 0}, {42, 255, 0}, {42, 255, 0}, {42, 255, 0}, {42, 255, 0}}
},

{
    {{0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}},
    {{0, 255, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 0}},
    {{0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}},
    {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 255, 0}},
    {{0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}, {0, 255, 0}}
}
    };

    // Percorre a matriz 5x5 para definir os LEDs
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            int posicao = getIndex(x, y);

            // Aplica redução da luminosidade multiplicando por 0.6
            leds[posicao].r = (uint8_t)(numbers_animation[number][y][x][0] * BRIGHTNESS);
            leds[posicao].g = (uint8_t)(numbers_animation[number][y][x][1] * BRIGHTNESS);
            leds[posicao].b = (uint8_t)(numbers_animation[number][y][x][2] * BRIGHTNESS);
        }
    }

    // Envia os dados para os LEDs
    for (uint i = 0; i < LEDS_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].g);
        pio_sm_put_blocking(np_pio, sm, leds[i].r);
        pio_sm_put_blocking(np_pio, sm, leds[i].b);
    }
    sleep_us(100);
}

// Função para inicializar tudo
void initAll() {
    stdio_init_all();

    // Inicializa matriz de LEDs NeoPixel
    npInit(MATRIX_PIN);

    // Inicializa os pinos do LED RGB
    gpio_init(RED_LED);
    gpio_init(GREEN_LED);
    gpio_init(BLUE_LED);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_set_dir(BLUE_LED, GPIO_OUT);

    // Inicializa os botões com resistores de pull-up internos
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);

    // Configura interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
}

int main() {
    // Inicializa todos os periféricos (LEDs, botões, matriz)
    initAll();

    // Exibe o primeiro número (0) na matriz
    number = 0;
    display_number(number);

    while (true) {
        // Pisca o LED vermelho do LED RGB 5 vezes por segundo
        gpio_put(RED_LED, 1);
        sleep_ms(100);
        gpio_put(RED_LED, 0);
        sleep_ms(100);

        // Verifica se o botão A foi pressionado para incrementar o número
        if (button_a_pressed) {
            button_a_pressed = false;
            if (number < 9) {
                number++;
            } else {
                number = 0; // Volta para 0 após 9
            }
            display_number(number);
        }

        // Verifica se o botão B foi pressionado para decrementar o número
        if (button_b_pressed) {
            button_b_pressed = false;
            if (number > 0) {
                number--;
            } else {
                number = 9; // Volta para 9 após 0
            }
            display_number(number);
        }
    }
}