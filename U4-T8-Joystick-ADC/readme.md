# Controle de Joystick com Display OLED - Raspberry Pi Pico

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/839bc273-88d5-4fc7-90c1-d93ed350f66d" alt="Esquema do circuito">
</div>

---

## 🎥 **Demonstração do Projeto**

Para ver o funcionamento do projeto, assista ao vídeo de demonstração disponível em:

🔗 **[Link do vídeo no YouTube](https://youtu.be/jrPrKpviOzg)**

---

## Descrição do Projeto

Este projeto implementa o **controle de um joystick** utilizando um **display OLED SSD1306** com o **Raspberry Pi Pico**. O código permite movimentar um quadrado na tela do display conforme os comandos recebidos do joystick, garantindo que o quadrado não ultrapasse as bordas da tela.

### **Principais funcionalidades implementadas:**

1. **Leitura do Joystick:**
   - Captura os valores dos eixos X e Y.
   - Mapeamento da posição do joystick para coordenadas no display.

2. **Exibição Gráfica no OLED:**
   - Renderiza um quadrado na posição correspondente ao movimento do joystick.
   - Atualiza a tela dinamicamente.

3. **Limitação das Bordas:**
   - Impede que o quadrado ultrapasse os limites do display (128x64 pixels).
   - Considera o tamanho do quadrado (8x8 pixels) ao definir os limites.

---

## 🛠 **Componentes Utilizados**

- **Microcontrolador:** Raspberry Pi Pico.
- **Joystick Analógico.**
- **Display OLED SSD1306.**
- **Resistores e jumpers para conexões.**

### **Conexões do Circuito:**

| Componente          | GPIO do Raspberry Pi Pico |
|---------------------|--------------------------|
| **Eixo X do Joystick** | ADC0 (GPIO 26) |
| **Eixo Y do Joystick** | ADC1 (GPIO 27) |
| **Botão do Joystick** | GPIO 22 |
| **SDA do Display**  | GPIO 4 |
| **SCL do Display**  | GPIO 5 |

---

## 🔍 **Funcionamento do Código**

### **Fluxo de Operação:**

1. **Leitura dos valores analógicos do joystick:**
   - Conversão do valor lido do ADC para uma posição na tela do display.

2. **Atualização do Display OLED:**
   - Limpa a tela e desenha o quadrado na nova posição.

3. **Verificação de Limites:**
   - Garante que o quadrado não ultrapasse as bordas do display (128x64 pixels).

### **Estrutura do Código:**

1. **Função `init_display()`**:
   - Configura a comunicação I2C com o display OLED.
   - Inicializa a tela para exibição gráfica.

2. **Função `read_joystick()`**:
   - Lê os valores do ADC correspondentes ao eixo X e Y.
   - Converte os valores para coordenadas no display.

3. **Função `update_display()`**:
   - Atualiza o display com a nova posição do quadrado.

4. **Loop principal (`main()`)**:
   - Lê os valores do joystick e atualiza a tela continuamente.

---

## ✅ **Pré-requisitos**

- **Ambiente de Desenvolvimento:** VS Code com extensão do Raspberry Pi Pico (com RP2040).
- **Linguagem:** C.
- **Placa de Desenvolvimento:** Raspberry Pi Pico.
- **Bibliotecas:** SSD1306 para comunicação com o display OLED.

---

## 🚀 **Instruções de Uso**

1. **Clone este repositório:**
   ```bash
   git clone https://github.com/SeuUsuario/SeuRepositorio
   ```

2. **Carregue o código na Raspberry Pi Pico:**
   - Pelo VS Code, compile e gere um arquivo `.uf2`, depois copie-o para a Pico conectada em modo `BOOTSEL`.

3. **Conecte os componentes e ligue a placa.**

4. **Movimente o joystick e veja a interação no display OLED!**

---

