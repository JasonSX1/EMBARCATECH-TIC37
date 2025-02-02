# Sistema de Controle de Semáforo com Temporizador no Raspberry Pi Pico

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/fa0bd604-8666-42d5-b1bf-a8696ccf7b4d" alt="Descrição da imagem">
</div>

---

## Descrição do Projeto

Este projeto implementa um **sistema de semáforo utilizando o Raspberry Pi Pico**, onde os LEDs representam as três cores do semáforo e alternam automaticamente a cada **3 segundos**. A estrutura foi desenvolvida seguindo os requisitos da atividade e empregando a função **`add_repeating_timer_ms()`** do **Pico SDK** para o controle do temporizador.

### **Principais funcionalidades implementadas:**

1. O **semáforo inicia na cor vermelha**, conforme especificado no descritivo.
2. A troca de estados ocorre a cada **3 segundos**, utilizando um **callback de temporizador** (`repeating_timer_callback`).
3. A função **`initAll()`** foi criada para organizar a inicialização dos componentes.
4. O sistema imprime mensagens via **comunicação serial** a cada 1 segundo, indicando seu funcionamento.

---

## Componentes Utilizados

- **Microcontrolador:** Raspberry Pi Pico.
- **LEDs:** Vermelho (GPIO 11), Amarelo (GPIO 12), Verde (GPIO 13).
- **Resistores:** 330 Ω conectados em série com cada LED.

---

## Funcionamento do Código

### **Fluxo do Semáforo:**
1. **Vermelho aceso** por 3 segundos.
2. **Amarelo aceso** por 3 segundos.
3. **Verde aceso** por 3 segundos.
4. Retorna ao vermelho e reinicia o ciclo.

### **Estrutura do Código:**

1. **Função `initAll()`**:
   - Inicializa a comunicação serial via **USB/UART**.
   - Configura os **GPIOs** dos LEDs como saída.
   - Define o estado inicial do semáforo (**vermelho ligado**).

2. **Função `repeating_timer_callback()`**:
   - Alterna os LEDs seguindo a sequência correta.
   - Mantém o temporizador ativo retornando `true`.
   - Exibe mensagens no terminal serial para indicar a troca de estados.

3. **Loop Principal (`main()`)**:
   - Chama `initAll()` para configurar o sistema.
   - Inicializa o temporizador com `add_repeating_timer_ms(3000, repeating_timer_callback, NULL, &timer);`
   - Exibe uma mensagem a cada **1 segundo** no terminal para indicar que o sistema está operando corretamente.

---

## Pré-requisitos

- **Ambiente de Desenvolvimento:** VS Code configurado com o **Pico SDK**.
- **Linguagem:** C.
- **Placa de Desenvolvimento:** Raspberry Pi Pico.
- **Monitor Serial:** Putty, minicom ou Monitor Serial do Arduino IDE.

---

## Instruções de Uso

1. **Clone este repositório:**
   ```bash
   git clone https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T5-ClockTemp-Ativ1
   ```

2. **Compile e execute o código usando a extensão do Raspberry Pi Pico.**

3. **Envie o arquivo `.uf2` para o Raspberry Pi Pico, ou para sua placa de desenvolvimento BitDogLab.**

4. **Abra um terminal serial** configurado para `115200 baud` e observe as mensagens de funcionamento do semáforo.

---

## Observações

- O **delay inicial de 2000ms** (`sleep_ms(2000);`) evita problemas na comunicação serial.
- A troca de LEDs é gerenciada **exclusivamente pelo temporizador**.
- A comunicação serial imprime mensagens a cada **1 segundo** para facilitar a depuração.

Este projeto segue os requisitos estabelecidos e pode ser utilizado para estudos e aplicações práticas em sistemas embarcados. 🚦

