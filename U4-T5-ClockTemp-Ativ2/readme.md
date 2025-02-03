# Temporizador de Um Disparo (One-Shot) com Raspberry Pi Pico W

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/f6c87836-4074-43b0-aa58-f57f9102fee0" alt="Esquema do circuito">
</div>

---

## Descrição do Projeto

Este projeto implementa um **sistema de temporização acionado por botão (pushbutton)** utilizando o **Raspberry Pi Pico W**. O sistema atende os requisitos da **Atividade 2: Temporizador de um disparo (One-Shot)**, onde um botão aciona a sequência de desligamento dos LEDs em intervalos de **3 segundos**.

### **Principais funcionalidades implementadas:**

1. **Ação por botão**: Os LEDs só podem ser acionados novamente após a conclusão do ciclo de desligamento, conforme exigido.
2. **Uso do `add_alarm_in_ms()`**: Implementado corretamente para realizar a temporização dos LEDs em um esquema de **um único disparo (one-shot).**
3. **Mudança de estados programada**: A sequência de desligamento ocorre na seguinte ordem:
   - **Pressionar o botão liga todos os LEDs**.
   - **Após 3 segundos, o LED verde desliga**.
   - **Após mais 3 segundos, o LED azul desliga**.
   - **Após mais 3 segundos, o LED vermelho desliga**.
   - **Sistema fica pronto para nova ativação pelo botão**.
4. **Implementação de debounce no botão**: O sistema inclui um **filtro por software** para evitar múltiplos acionamentos acidentais do botão.
5. **Estrutura otimizada**: Código organizado com **função de inicialização `initAll()`** e uma **tabela de estados dos LEDs**, tornando o código modular e fácil de modificar.

---

## Componentes Utilizados

- **Microcontrolador:** Raspberry Pi Pico W.
- **LEDs:** Verde (GPIO 11), Azul (GPIO 12), Vermelho (GPIO 13).
- **Resistores:** 330 Ω conectados em série com cada LED.
- **Botão Pushbutton:** Conectado ao GPIO 5 com resistor de pull-up interno ativado.

---

## Funcionamento do Código

### **Fluxo de Operação:**
1. O usuário **pressiona o botão** (GPIO 5).
2. Todos os **LEDs acendem imediatamente** (**cor branca**).
3. O temporizador inicia e desliga os LEDs na seguinte sequência:
   - **3s:** LED verde desliga (**cor roxa**).
   - **6s:** LED azul desliga (**cor vermelha**).
   - **9s:** LED vermelho desliga (**desligado completamente**).
4. O sistema aguarda um **novo acionamento do botão** para repetir o ciclo.

### **Estrutura do Código:**

1. **Função `initAll()`**:
   - Configura **todos os GPIOs** (LEDs e botão).
   - Habilita **pull-up** para o botão.
   - Inicializa os LEDs desligados.

2. **Função `timer_callback()`**:
   - Controla **o ciclo de desligamento dos LEDs**, garantindo intervalos de **3 segundos** entre cada transição.
   - **Finaliza o temporizador** após o último LED ser desligado.

3. **Loop principal (`main()`)**:
   - Monitora **o estado do botão** e inicia a sequência **apenas quando o último LED estiver desligado**.
   - Implementa um **filtro de debounce** para evitar acionamentos involuntários.

---

## Pré-requisitos

- **Ambiente de Desenvolvimento:** VS Code configurado com o **Pico SDK**.
- **Linguagem:** C.
- **Placa de Desenvolvimento:** Raspberry Pi Pico W.
- **Monitor Serial:** Putty, minicom ou Monitor Serial do Arduino IDE.

---

## Instruções de Uso

1. **Clone este repositório:**
   ```bash
   git clone https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T5-ClockTemp-Ativ2
   ```

2. **Compile e execute o código usando a extensão do Raspberry Pi Pico.**

3. **Envie o arquivo `.uf2` para o Raspberry Pi Pico W, ou para sua placa de desenvolvimento BitDogLab.**

4. **Abra um terminal serial** configurado para `115200 baud` e observe as mensagens de funcionamento do sistema.

---

## Observações

- O **debounce por software** evita múltiplos acionamentos acidentais do botão.
- O **temporizador one-shot** garante que apenas **uma sequência ocorra por vez**, seguindo os requisitos.
- O código segue **boas práticas**, com modularização e facilidade de manutenção.
- **Testado com sucesso na plataforma BitDogLab**, com a seguinte transição de cores: **branco → roxo → vermelho → desligado**.