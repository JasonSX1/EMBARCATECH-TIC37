#define BUZZER_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "inc/menu.h"

extern const uint16_t A_BUZZER;
extern const uint16_t B_BUZZER;
extern const float PWM_CLOCK_DIV;