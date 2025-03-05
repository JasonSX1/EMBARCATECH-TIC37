#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "inc/menu.h"
#include "inc/ssd1306.h"
#include "inc/terminais.h"
#include "inc/buzzer.h"
#include "inc/entrada_usuario.h"
#include "string.h"

// Definição dos pinos dos botões
#define BUTTON_UP 5
#define BUTTON_DOWN 6
#define BUTTON_SELECT 22

// Estado atual do menu
MenuState menu_state = MENU_PRINCIPAL;
bool update_display = true;
const int MENU_SIZE = 2; // Agora temos "Med Bioimped" e "Inserir Dados"
int submenu_index = 0; // Índice para navegar no menu de inserção de dados
int menu_index = 0;  // Definição da variável global

// Inicializa os botões e suas interrupções
void init_buttons() {
    gpio_init(BUTTON_UP);
    gpio_set_dir(BUTTON_UP, GPIO_IN);
    gpio_pull_up(BUTTON_UP);
    gpio_set_irq_enabled_with_callback(BUTTON_UP, GPIO_IRQ_EDGE_FALL, true, &on_button_up);

    gpio_init(BUTTON_DOWN);
    gpio_set_dir(BUTTON_DOWN, GPIO_IN);
    gpio_pull_up(BUTTON_DOWN);
    gpio_set_irq_enabled_with_callback(BUTTON_DOWN, GPIO_IRQ_EDGE_FALL, true, &on_button_down);

    gpio_init(BUTTON_SELECT);
    gpio_set_dir(BUTTON_SELECT, GPIO_IN);
    gpio_pull_up(BUTTON_SELECT);
    gpio_set_irq_enabled_with_callback(BUTTON_SELECT, GPIO_IRQ_EDGE_FALL, true, &on_button_select);
}

void on_button_up() {
    if (menu_state == MENU_PRINCIPAL) {
        menu_index = (menu_index - 1 + MENU_SIZE) % MENU_SIZE;
    } else if (menu_state == MENU_DADOS_USUARIO) {
        submenu_index = (submenu_index - 1 + 4) % 4;
    }
    update_display = true;
}

void on_button_down() {
    if (menu_state == MENU_PRINCIPAL) {
        menu_index = (menu_index + 1) % MENU_SIZE;
    } else if (menu_state == MENU_DADOS_USUARIO) {
        submenu_index = (submenu_index + 1) % 4;
    }
    update_display = true;
}


// Função que inicia a medição de bioimpedância
void menu_medir(ssd1306_t *ssd) {
    if (!usuario.dados_cadastrados) {
        printf("Por favor, insira os seus dados antes da medição!\n");
        menu_state = MENU_DADOS_USUARIO;
        return;
    }
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
    iniciar_medicao(ssd);
}

// Ação ao pressionar o botão SELECT
void on_button_select(ssd1306_t *ssd) {
    printf("Botão SELECT pressionado! Estado atual: %d\n", menu_state);
    tocar_notificacao(); // Som ao mudar de menu

    switch (menu_state) {
        case MENU_PRINCIPAL:
            if (menu_index == 0) {  // Medição
                if (!usuario.dados_cadastrados) {
                    printf("Redirecionando para MENU_DADOS_USUARIO...\n");
                    menu_state = MENU_DADOS_USUARIO;
                } else {
                    printf("Redirecionando para MENU_CONFIRMAR_MEDICAO...\n");
                    menu_state = MENU_CONFIRMAR_MEDICAO;
                    exibir_confirmacao(ssd);
                }
            } else if (menu_index == 1) {  // Inserir Dados
                printf("Entrando no submenu de edição de dados...\n");
                menu_state = MENU_DADOS_USUARIO;
                submenu_index = 0;
            }
            break;

        case MENU_CONFIRMAR_MEDICAO:
            printf("Confirmando medição...\n");
            menu_state = MENU_MEDIR;
            iniciar_medicao(ssd);
            break;

        case MENU_DADOS_USUARIO:
            printf("Entrando no modo de edição de dados...\n");
            menu_state = MENU_EDITAR_DADO;
            update_display = true;
            break;

        case MENU_EDITAR_DADO:
            printf("Confirmando edição, retornando ao MENU_DADOS_USUARIO...\n");
            menu_state = MENU_DADOS_USUARIO;
            update_display = true;
            break;

        default:
            printf("Voltando ao MENU_PRINCIPAL...\n");
            menu_state = MENU_PRINCIPAL;
            break;
    }
    printf("Novo estado do menu: %d\n", menu_state);
    update_display = true;
}


// Tela de confirmação antes da medição
void exibir_confirmacao(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);
    ssd1306_draw_string(ssd, "Iniciar Medicao", 6, 10);
    ssd1306_draw_string(ssd, "Pressione OK", 8, 25);
    ssd1306_draw_string(ssd, "Para continuar", 8, 40);
    tocar_notificacao();
    ssd1306_send_data(ssd);
}

// Atualiza a exibição do menu principal
void update_menu_display(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false); // Limpa o display

    const char *menu_options[] = {"Med Bioimped", "Inserir Dados"};
    int altura_opcao = 16;

    switch (menu_state) {
        case MENU_PRINCIPAL:
            for (int i = 0; i < MENU_SIZE; i++) {
                ssd1306_draw_string(ssd, menu_options[i], 20, i * altura_opcao + 4);
            }
            int selecao_y_posicao = menu_index * altura_opcao;
            ssd1306_rect(ssd, selecao_y_posicao, 0, 128, altura_opcao, true, false);
            break;

        case MENU_CONFIRMAR_MEDICAO:
            exibir_confirmacao(ssd);
            break;

        case MENU_MEDIR:
            ssd1306_fill(ssd, false);
            ssd1306_draw_string(ssd, "Modo: Medicao", 10, 10);
            ssd1306_draw_string(ssd, "Pressione L3 p/ iniciar", 10, 20);
            ssd1306_send_data(ssd);
            iniciar_medicao(ssd);
            break;

        case MENU_DADOS_USUARIO:
            menu_dados_usuario(ssd);
            break;

        case MENU_EDITAR_DADO:
            menu_editar_dado(ssd);
            break;
    }
    ssd1306_send_data(ssd);
}


// Novo submenu: Exibe os dados do usuário
void menu_dados_usuario(ssd1306_t *ssd) {
    tocar_notificacao();
    ssd1306_fill(ssd, false);
    
    const char *dados_opcoes[] = {"Idade", "Altura", "Peso", "Sexo"};
    char buffer[32];

    for (int i = 0; i < 4; i++) {
        snprintf(buffer, sizeof(buffer), "%s: ", dados_opcoes[i]);
        
        switch (i) {
            case 0:
                snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%d anos", usuario.idade);
                break;
            case 1:
                snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%d cm", usuario.altura);
                break;
            case 2:
                snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%.1f kg", usuario.peso);
                break;
            case 3:
                snprintf(buffer + strlen(buffer), sizeof(buffer) - strlen(buffer), "%s", usuario.sexo ? "Masc" : "Fem");
                break;
        }

        ssd1306_draw_string(ssd, buffer, 10, i * 16 + 4);
    }

    // Exibe a caixa de seleção ao redor do item selecionado
    int selecao_y_posicao = submenu_index * 16;
    ssd1306_rect(ssd, selecao_y_posicao, 0, 128, 16, true, false);

    ssd1306_send_data(ssd);
}

void menu_editar_dado(ssd1306_t *ssd) {
    ssd1306_fill(ssd, false);

    char buffer[16];

    // Exibe as setas e o valor atual (exceto para o sexo)
    if (submenu_index < 3) {
        ssd1306_draw_string(ssd, "<", 10, 28); // Seta esquerda
        ssd1306_draw_string(ssd, ">", 110, 28); // Seta direita
    }

    switch (submenu_index) {
        case 0: // Idade
            snprintf(buffer, sizeof(buffer), "%d anos", usuario.idade);
            break;
        case 1: // Altura
            snprintf(buffer, sizeof(buffer), "%d cm", usuario.altura);
            break;
        case 2: // Peso
            snprintf(buffer, sizeof(buffer), "%.1f kg", usuario.peso);
            break;
        case 3: // Sexo
            {
                // Definição das posições base para alinhamento
                int x_texto = 40;  // Posição X do texto alinhado
                int y_masculino = 20;  // Posição Y do texto "Masculino"
                int y_feminino = 40;  // Posição Y do texto "Feminino"

                // Desenha as opções de sexo
                ssd1306_draw_string(ssd, "Masculino", x_texto, y_masculino);
                ssd1306_draw_string(ssd, "Feminino", x_texto, y_feminino);

                // Ajuste do deslocamento da caixa de seleção
                int deslocamento_x_caixa = -2;  // Move 2px para a esquerda
                int deslocamento_y_caixa = -3;  // Move 3px para cima

                // Definição da posição da caixa de seleção
                int x_caixa, y_caixa;
                if (usuario.sexo == 0) {
                    // Selecionado: Feminino
                    x_caixa = x_texto + deslocamento_x_caixa;
                    y_caixa = y_feminino + deslocamento_y_caixa;
                } else {
                    // Selecionado: Masculino
                    x_caixa = x_texto + deslocamento_x_caixa;
                    y_caixa = y_masculino + deslocamento_y_caixa;
                }

                // Desenha a caixa de seleção ao redor da opção escolhida
                ssd1306_rect(ssd, y_caixa, x_caixa, 80, 14, true, false);

                ssd1306_send_data(ssd);
                return;
            }
    }

    // Centraliza o valor na tela apenas para idade, altura e peso
    if (submenu_index < 3) {
        int text_width = strlen(buffer) * 6; // Largura aproximada do texto
        ssd1306_draw_string(ssd, buffer, (128 - text_width) / 2, 28);
    }

    ssd1306_send_data(ssd);
}
