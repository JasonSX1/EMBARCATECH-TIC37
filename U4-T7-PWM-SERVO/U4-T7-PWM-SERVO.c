#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

#define SERVO_PIN 22
#define PWM_FREQUENCY 50
#define PWM_WRAP_VALUE 20000  // 20ms período

void set_servo_pulse(uint slice, uint32_t pulse_width) {
    pwm_set_gpio_level(SERVO_PIN, pulse_width);
}

int main() {
    stdio_init_all();
    
    // Configuração do GPIO como saída PWM
    gpio_set_function(SERVO_PIN, GPIO_FUNC_PWM);
    
    // Obtém o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(SERVO_PIN);
    
    // Configuração do PWM
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 125.0f);  // Define um divisor de clock para ajustar a frequência
    pwm_config_set_wrap(&config, PWM_WRAP_VALUE);
    pwm_init(slice_num, &config, true);
    
    while (true) {
        // Define o servo para 180 graus (2.400µs)
        set_servo_pulse(slice_num, 2400);
        sleep_ms(5000);

        // Define o servo para 90 graus (1.470µs)
        set_servo_pulse(slice_num, 1470);
        sleep_ms(5000);

        // Define o servo para 0 graus (500µs)
        set_servo_pulse(slice_num, 500);
        sleep_ms(5000);

        // Movimentação suave entre 0 e 180 graus
        for (uint32_t pulse = 500; pulse <= 2400; pulse += 5) {
            set_servo_pulse(slice_num, pulse);
            sleep_ms(10);
        }
        for (uint32_t pulse = 2400; pulse >= 500; pulse -= 5) {
            set_servo_pulse(slice_num, pulse);
            sleep_ms(10);
        }
    }
}
