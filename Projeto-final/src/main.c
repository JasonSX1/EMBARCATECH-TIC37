#include "inc/menu.h"
#include "inc/medicao.h"
#include "inc/display.h"
#include "inc/entrada_usuario.h"

int main() {
    inicializar_hardware(); // Configura os GPIOs, I²C, PWM e PIO
    exibir_boas_vindas(); // Exibe mensagem inicial no Display OLED

    while (1) {
        exibir_menu(); // Chamando o menu principal para interação do usuário
    }
}
