# Comunicação Serial e Controle de Hardware com Raspberry Pi Pico W

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/3c453f0d-73c3-408d-8452-5f7021a11f67" alt="Esquema do circuito">
</div>

---

## Descrição do Projeto

Este projeto explora a **comunicação serial (UART e I2C)** e o **controle de hardware** com o **Raspberry Pi Pico W**, integrando **entrada de dados via PC, exibição em display OLED SSD1306 e controle de LEDs endereçáveis WS2812 e LEDs RGB**. O objetivo é demonstrar a aplicação prática dos conceitos de **interrupções, debounce de botões e comunicação serial**.

### **Principais funcionalidades implementadas:**

1. **Entrada de caracteres via UART:**
   - Caracteres digitados no **Serial Monitor** são exibidos no **display SSD1306**.
   - Quando um número entre **0 e 9** for digitado, ele é exibido na **matriz de LEDs WS2812**.

2. **Controle de LEDs via botões:**
   - **Botão A (GPIO 5)** alterna o **LED RGB Verde (GPIO 11)**.
   - **Botão B (GPIO 6)** alterna o **LED RGB Azul (GPIO 12)**.
   - O estado dos LEDs é exibido no **display OLED** e no **monitor serial**.

3. **Matriz de LEDs WS2812 (5x5) para números:**
   - Exibição dos números de **0 a 9** conforme entrada via **UART**.
   - Controle via **PIO (Programmable I/O)** para eficiência.
   - Correção da disposição dos pixels para exibição correta.

4. **Fontes personalizadas:**
   - **Caracteres maiúsculos e minúsculos adicionados** à biblioteca `font.h`.
   - Suporte a fontes **12x12 pixels** para melhor legibilidade no **SSD1306**.

5. **Debounce e interrupções:**
   - Uso de **interrupções (IRQ)** para botões, evitando pooling ineficiente.
   - Implementação de **debounce por software** para evitar múltiplos acionamentos acidentais.

---

## Componentes Utilizados

- **Microcontrolador:** Raspberry Pi Pico W.
- **Matriz de LEDs WS2812 (5x5)** conectada ao **GPIO 7**.
- **LED RGB:**
  - Verde (GPIO 11), Azul (GPIO 12), Vermelho (GPIO 13).
- **Botões de entrada:**
  - **Botão A** (GPIO 5) - Alterna LED Verde.
  - **Botão B** (GPIO 6) - Alterna LED Azul.
- **Display SSD1306** via I2C (SDA: GPIO 14, SCL: GPIO 15).

---

## Funcionamento do Código

### **Fluxo de Operação:**
1. O **usuário digita um caractere** no **Serial Monitor**.
2. Se for **uma letra**, ela aparece **apenas no display SSD1306**.
3. Se for **um número (0-9)**, ele aparece **no display SSD1306 e na matriz de LEDs WS2812**.
4. **Pressionar Botão A** alterna o **LED Verde** e exibe o estado no **serial monitor e display**.
5. **Pressionar Botão B** alterna o **LED Azul** e exibe o estado no **serial monitor e display**.

### **Estrutura do Código:**

1. **Função `initAll()`**:
   - Configura **UART**, **I2C**, **GPIOs**, **display OLED**, **matriz WS2812** e **interrupções**.

2. **Função `display_number()`**:
   - Atualiza **matriz de LEDs WS2812** com o número digitado.
   - Ajusta **cores e disposição de pixels** corretamente.

3. **Função `ssd1306_draw_char()`**:
   - Exibe caracteres no **display SSD1306** usando a nova **biblioteca de fontes (font.h)**.

4. **Função `gpio_callback()`**:
   - Trata **interrupções dos botões A e B**.
   - Alterna LEDs e exibe informações no **serial monitor e display OLED**.

5. **Loop principal (`main()`)**:
   - Monitora **entrada via UART** e exibe caracteres e números conforme especificado.

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
   git clone https://github.com/SEU_GITHUB/bitdoglab_serial_ws2812
   ```

2. **Compile e carregue o código:**
   - Use o **Raspberry Pi Pico SDK** para compilar.
   - Copie o **arquivo `.uf2`** para a **placa Raspberry Pi Pico W**.

3. **Abra um terminal serial:**
   - Configuração recomendada: `115200 baud`.
   - Use **Putty**, **minicom**, ou **Arduino IDE**.

4. **Interaja com o sistema:**
   - Digite **números (0-9)** para exibir na **matriz WS2812** e no **display OLED**.
   - Digite **letras** para exibir no **display OLED**.
   - Pressione **Botão A** para alternar o **LED Verde**.
   - Pressione **Botão B** para alternar o **LED Azul**.

---

## Observações

- **Correção das cores da matriz WS2812** para exibição precisa.
- **Correção do posicionamento dos pixels** (espelhamento horizontal corrigido).
- **Debounce de botões implementado** via software.
- **Fontes aprimoradas** para melhor visualização no **display OLED**.
- **Código modular e organizado** seguindo boas práticas de programação embarcada.

---

### 🎥 **Demonstração do Projeto**

Para ver o funcionamento do projeto, assista ao vídeo de demonstração disponível em:

🔗 **[Link do vídeo no YouTube ou Google Drive](INSERIR_LINK_AQUI)**

