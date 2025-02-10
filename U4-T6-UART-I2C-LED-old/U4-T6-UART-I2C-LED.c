#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/bootrom.h"
#include "inc/ssd1306.h"
#include "inc/ssd1306.c"
#include "inc/font.h"
#include "ws2812b.pio.h"
#include "hardware/i2c.h"
#include "hardware/clocks.h"
#include "hardware/pio.h"

int main()
{
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
