#ifndef ENTRADA_USUARIO_H
#define ENTRADA_USUARIO_H

#include "pico/stdlib.h"
#include "inc/ssd1306.h"

// Estrutura para armazenar os dados do usuário
typedef struct {
    int idade;
    int altura;  // Em cm
    float peso;  // Em kg, com 1 casa decimal
    int sexo;    // 0 = Feminino, 1 = Masculino
    bool dados_cadastrados;  // Indica se os dados foram cadastrados
} DadosUsuario;

extern DadosUsuario usuario; // Variável global para armazenar os dados do usuário

// Funções para entrada e manipulação dos dados do usuário
void iniciar_dados_usuario();  // Inicializa os dados como vazios/nulos
void configurar_dados_usuario(ssd1306_t *ssd);  // Permite que o usuário insira os dados

#endif
