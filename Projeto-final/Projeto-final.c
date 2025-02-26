#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "inc/ssd1306.h"
#include <stdio.h>

#define GPIO_EMISSAO 0  // Pino de emissão do sinal
#define GPIO_RECEPCAO 3 // Pino de recepção do sinal
#define GPIO_BOTAO_A 5  // Pino do botão A
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define OLED_ADDRESS 0x3C
#define DURACAO_MEDICAO 60000 // 1 minuto em milissegundos

ssd1306_t display;

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

void iniciar_medicao() {
    printf("Iniciando medicao...\n");
    sleep_ms(100);
    atualizar_display("Medicao iniciada");
    iniciar_sinal_pwm(GPIO_EMISSAO, 1000, 6250);
    gpio_init(GPIO_RECEPCAO);
    gpio_set_dir(GPIO_RECEPCAO, GPIO_IN);

    uint32_t tempo_inicio = to_ms_since_boot(get_absolute_time());
    uint32_t tempo_atual;
    char buffer[32];

    while (true) {
        tempo_atual = to_ms_since_boot(get_absolute_time());
        if (tempo_atual - tempo_inicio >= DURACAO_MEDICAO) {
            printf("Tempo de medicao concluido.\n");
            atualizar_display("Medicao finalizada.");
            pwm_set_enabled(pwm_gpio_to_slice_num(GPIO_EMISSAO), false);
            break;
        }

        int estado_recebido = gpio_get(GPIO_RECEPCAO);
        snprintf(buffer, sizeof(buffer), "Freq: 1000Hz\nRecebido: %d", estado_recebido);
        printf("%s\n", buffer);
        atualizar_display(buffer);
        sleep_ms(1000);
    }
}

int main() {
    stdio_init_all();
    printf("Inicializando sistema...\n");
    sleep_ms(500);
    
    printf("Inicializando I2C...\n");
    i2c_init(I2C_PORT, 400 * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    printf("Inicializando display...\n");
    ssd1306_init(&display, 128, 64, false, OLED_ADDRESS, I2C_PORT);
    ssd1306_config(&display);
    sleep_ms(100);
    ssd1306_fill(&display, 0);
    ssd1306_send_data(&display);
    
    printf("Configurando botao A...\n");
    gpio_init(GPIO_BOTAO_A);
    gpio_set_dir(GPIO_BOTAO_A, GPIO_IN);
    gpio_pull_up(GPIO_BOTAO_A);

    printf("Aguardando botao A para iniciar...\n");
    atualizar_display("Pressione A para iniciar");
    sleep_ms(100);

    while (gpio_get(GPIO_BOTAO_A) == 1) {
        sleep_ms(100);
    }

    printf("Botao A pressionado! Iniciando medicao...\n");
    iniciar_medicao();
    return 0;
}