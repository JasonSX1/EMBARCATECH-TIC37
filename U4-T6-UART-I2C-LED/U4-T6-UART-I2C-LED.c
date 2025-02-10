// Inclusão de bibliotecas
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812b.pio.h"
#include "inc/ssd1306.h"

// Definições de hardware
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C

#define LEDS_COUNT 25
#define LEDS_PIN 7
#define BRIGHTNESS 0.2

// Definições de botões e LEDs
#define RED_LED 13
#define GREEN_LED 11
#define BLUE_LED 12
#define BUTTON_A 5
#define BUTTON_B 6

// Definição da estrutura para os LEDs (ordem GRB)
typedef struct {
    uint8_t g, r, b;
} pixel_t;

pixel_t leds[LEDS_COUNT]; // Matriz de LEDs

// Buffer para armazenar a entrada do usuário
char input_buffer[32];
int input_index = 0;

// Variáveis globais para PIO e state machine
static PIO np_pio;
static uint sm;
ssd1306_t ssd; // Instância do display
bool cor = true; // Cor de fundo do display

// Variáveis globais para controle dos botões
volatile uint32_t tempo_anterior = 0;

// Inicializa a máquina de estado PIO
void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2812b_program);
    np_pio = pio0;

    sm = pio_claim_unused_sm(np_pio, true);
    ws2812b_program_init(np_pio, sm, offset, pin, 800000.f);

    pio_sm_set_enabled(np_pio, sm, true);

    // Desliga todos os LEDs ao iniciar
    for (uint i = 0; i < LEDS_COUNT; ++i) {
        leds[i].r = 0;
        leds[i].g = 0;
        leds[i].b = 0;
    }
}

// Função para enviar dados para os LEDs
void send_to_leds() {
    for (uint i = 0; i < LEDS_COUNT; ++i) {
        uint32_t pixel_color = ((uint32_t)leds[i].g << 16) | ((uint32_t)leds[i].r << 8) | ((uint32_t)leds[i].b);
        pio_sm_put_blocking(np_pio, sm, pixel_color);
    }
}

// Converte índices da matriz 5x5 para LEDs WS2812B (invertendo verticalmente)
int getIndex(int x, int y) {
    if (y % 2 == 0) {
        return (4 - y) * 5 + (4 - x);  // Espelhamos a linha par
    } else {
        return (4 - y) * 5 + x;  // Mantemos a linha ímpar normal
    }
}


// Exibe um número na matriz de LEDs
void display_number(uint8_t number) {
    // Matriz de números (0-9)
    const int numbers_animation[11][5][5][3] = {
        // 0
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
        },
        {
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}},
            {{0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}, {0, 0, 0}}
        }
            };

    // Atualiza os LEDs
    for (int y = 0; y < 5; y++) {
        for (int x = 0; x < 5; x++) {
            int posicao = getIndex(x, y);
            leds[posicao].r = (uint8_t)(numbers_animation[number][y][x][0] * BRIGHTNESS);
            leds[posicao].g = (uint8_t)(numbers_animation[number][y][x][1] * BRIGHTNESS);
            leds[posicao].b = (uint8_t)(numbers_animation[number][y][x][2] * BRIGHTNESS);
        }
    }
    send_to_leds();
}

// Callback para os botões
void gpio_callback(uint gpio, uint32_t events) {
    uint32_t tempo_agora = to_ms_since_boot(get_absolute_time());
    if (tempo_agora - tempo_anterior > 250) {
        tempo_anterior = tempo_agora;
        ssd1306_fill(&ssd, !cor); // Limpa o display

        if (gpio == BUTTON_A) {
            bool estado = !gpio_get(GREEN_LED);
            gpio_put(GREEN_LED, estado);
            printf("LED Verde %s\n", estado ? "LIGADO" : "DESLIGADO");

            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
            ssd1306_draw_string(&ssd, "GEISON L. F.", 15, 48); // Desenha uma string   
            ssd1306_draw_string(&ssd, estado ? "VERDE ON" : "VERDE OFF", 30, 30);
        } else if (gpio == BUTTON_B) {
            bool estado = !gpio_get(BLUE_LED);
            gpio_put(BLUE_LED, estado);
            printf("LED Azul %s\n", estado ? "LIGADO" : "DESLIGADO");

            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
            ssd1306_draw_string(&ssd, "GEISON L. F.", 15, 48); // Desenha uma string   
            ssd1306_draw_string(&ssd, estado ? "AZUL ON" : "AZUL OFF", 30, 30);
        }

        ssd1306_send_data(&ssd); // Atualiza o display
    }
}


// Inicializa os periféricos
void initAll() {
    stdio_init_all();
    npInit(LEDS_PIN);

    // Inicializa os LEDs
    gpio_init(GREEN_LED);
    gpio_init(RED_LED);
    gpio_init(BLUE_LED);
    gpio_set_dir(GREEN_LED, GPIO_OUT);
    gpio_set_dir(RED_LED, GPIO_OUT);
    gpio_set_dir(BLUE_LED, GPIO_OUT);
    gpio_put(GREEN_LED, 0);
    gpio_put(RED_LED, 0);
    gpio_put(BLUE_LED, 0);

    // Inicializa botões
    gpio_init(BUTTON_A);
    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
    
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);

    // Inicializa display OLED
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    ssd1306_init(&ssd, 128, 64, false, endereco, I2C_PORT);
    ssd1306_config(&ssd);
    ssd1306_send_data(&ssd);
    ssd1306_fill(&ssd, false);
    ssd1306_send_data(&ssd);
}

// Loop principal
int main() {
    initAll();
    display_number(10);  // Limpa a matriz de LEDs

    while (true) {
        printf("Digite um número ou letra (0-9)(A-Z, a-z): \n");
        fflush(stdout); // Garante que a mensagem seja exibida imediatamente 

        char c = getchar();
        if (c >= '0' && c <= '9') {
            printf("Número recebido: %c\n", c); // Imprime no monitor serial
            display_number(c - '0'); // Atualiza matriz de LEDs
        
            // Atualiza a tela OLED com o número
            ssd1306_fill(&ssd, !cor); 
            char str[2] = {c, '\0'};
            ssd1306_draw_string(&ssd, str, 60, 30);
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
            ssd1306_draw_string(&ssd, "GEISON L. F.", 15, 48); // Desenha uma string     
            ssd1306_send_data(&ssd); // Atualiza o display            
        } else {
            printf("Número recebido: %c\n", c); // Imprime no monitor serial
            display_number(10); // Limpa a matriz de LEDs
            ssd1306_fill(&ssd, !cor);
            char str[2] = {c, '\0'};
            ssd1306_draw_string(&ssd, str, 60, 30);
            ssd1306_rect(&ssd, 3, 3, 122, 58, cor, !cor); // Desenha um retângulo
            ssd1306_draw_string(&ssd, "CEPEDI   TIC37", 8, 10); // Desenha uma string
            ssd1306_draw_string(&ssd, "GEISON L. F.", 15, 48); // Desenha uma string     
            ssd1306_send_data(&ssd); // Atualiza o display
        }
        
    }
}