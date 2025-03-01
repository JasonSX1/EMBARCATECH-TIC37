#include "inc/buzzer.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h" // Biblioteca necessária para clock_get_hz()

void iniciar_buzzer() {
    gpio_set_function(GPIO_BUZZER, GPIO_FUNC_PWM);  // Configura o buzzer como saída PWM
    uint slice_num = pwm_gpio_to_slice_num(GPIO_BUZZER);
    pwm_set_wrap(slice_num, 12500);  // Define a resolução do PWM
    pwm_set_enabled(slice_num, false);  // Inicia desativado
}

void tocar_buzzer(uint frequency, uint duration) {
    uint slice_num = pwm_gpio_to_slice_num(GPIO_BUZZER);
    
    if (frequency > 0) {
        uint32_t clock_freq = clock_get_hz(clk_sys);  // Obtém a frequência do clock do sistema
        float divider = (float) clock_freq / (frequency * 12500);
        pwm_set_clkdiv(slice_num, divider);
        pwm_set_chan_level(slice_num, PWM_CHAN_A, 6250);  // Define o duty cycle em 50%
        pwm_set_enabled(slice_num, true);

        sleep_ms(duration);  // Tempo de duração do som
        parar_buzzer();
    }
}

void parar_buzzer() {
    uint slice_num = pwm_gpio_to_slice_num(GPIO_BUZZER);
    pwm_set_enabled(slice_num, false);  // Desliga o buzzer
}
