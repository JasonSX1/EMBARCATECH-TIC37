#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22     // Pino do servomotor
#define BLUE_LED 12      // Pino do LED RGB (canal azul)
#define PWM_FREQUENCY 50 // Frequência do PWM
#define PWM_WRAP_VALUE 20000  // 20ms período

// Função para definir o pulso PWM do servomotor
void set_servo_pulse(uint slice, uint32_t pulse_width) {
    pwm_set_gpio_level(SERVO_PIN, pulse_width);
    printf("Servo PWM: %u us\n", pulse_width); // Debug no Wokwi
}

// Função para definir o brilho do LED
void set_led_brightness(uint slice, uint32_t brightness) {
    pwm_set_gpio_level(BLUE_LED, brightness);
    printf("LED PWM: %u\n", brightness); // Debug no Wokwi
}

int main() {
    stdio_init_all();
    sleep_ms(2000);  // Espera inicial para o Wokwi inicializar
    printf("Iniciando...\n");

    // Configuração do GPIO como saída PWM para o servo
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    uint slice_servo = pwm_gpio_to_slice_num(SERVO_PIN);

    pwm_config config_servo = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_servo, 125.0f);  // Ajuste do clock
    pwm_config_set_wrap(&config_servo, PWM_WRAP_VALUE);
    pwm_init(slice_servo, &config_servo, true);

    // Configuração do GPIO como saída PWM para o LED azul
    gpio_set_function(BLUE_LED, GPIO_FUNC_PWM);
    uint slice_led = pwm_gpio_to_slice_num(BLUE_LED);

    pwm_config config_led = pwm_get_default_config();
    pwm_config_set_clkdiv(&config_led, 125.0f);  // Mesmo clock do servo
    pwm_config_set_wrap(&config_led, PWM_WRAP_VALUE);
    pwm_init(slice_led, &config_led, true);

    while (true) {
        // Define o servo para 180 graus (2.400µs) e LED com brilho alto
        set_servo_pulse(slice_servo, 2400);
        set_led_brightness(slice_led, 1800);
        sleep_ms(5000);

        // Define o servo para 90 graus (1.470µs) e LED com brilho médio
        set_servo_pulse(slice_servo, 1470);
        set_led_brightness(slice_led, 1000);
        sleep_ms(5000);

        // Define o servo para 0 graus (500µs) e LED com brilho baixo
        set_servo_pulse(slice_servo, 500);
        set_led_brightness(slice_led, 500);
        sleep_ms(5000);

        // Movimentação suave entre 0 e 180 graus e ajuste de brilho do LED
        for (uint32_t pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_pulse(slice_servo, pulse);
            set_led_brightness(slice_led, pulse / 2);  // Brilho proporcional
            sleep_ms(10);
        }
        for (uint32_t pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_pulse(slice_servo, pulse);
            set_led_brightness(slice_led, pulse / 2);  // Brilho proporcional
            sleep_ms(10);
        }
    }
}
