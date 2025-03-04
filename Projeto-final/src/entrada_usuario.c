#include "inc/entrada_usuario.h"
#include <stdio.h>

// Variável global para armazenar os dados do usuário
DadosUsuario usuario = {0, 0, 0.0, 0, false};  // Inicia sem valores válidos

// Inicializa os dados do usuário como vazios/nulos
void iniciar_dados_usuario() {
    usuario.idade = 0;
    usuario.altura = 0;
    usuario.peso = 0.0;
    usuario.sexo = 0;
    usuario.dados_cadastrados = false;
}

// Exibe e permite editar os dados do usuário
void configurar_dados_usuario(ssd1306_t *ssd) {
    int index = 0;  // Índice do campo que está sendo editado

    while (!usuario.dados_cadastrados) {
        ssd1306_fill(ssd, false);
        char buffer[32];

        snprintf(buffer, sizeof(buffer), "Idade: %d anos", usuario.idade);
        ssd1306_draw_string(ssd, buffer, 10, 4);

        snprintf(buffer, sizeof(buffer), "Altura: %d cm", usuario.altura);
        ssd1306_draw_string(ssd, buffer, 10, 20);

        snprintf(buffer, sizeof(buffer), "Peso: %.1f kg", usuario.peso);
        ssd1306_draw_string(ssd, buffer, 10, 36);

        snprintf(buffer, sizeof(buffer), "Sexo: %s", usuario.sexo ? "Masculino" : "Feminino");
        ssd1306_draw_string(ssd, buffer, 10, 52);

        ssd1306_draw_string(ssd, "Pressione OK p/ salvar", 10, 60);
        ssd1306_send_data(ssd);

        // Aqui você pode implementar a lógica para capturar inputs do usuário
        // Exemplo:
        // Se UP -> aumentar o valor do campo atual
        // Se DOWN -> diminuir o valor do campo atual
        // Se SELECT -> confirmar e passar para o próximo campo

        // Simulação de confirmação (substitua com lógica real de entrada do usuário)
        usuario.dados_cadastrados = true;
    }
}
