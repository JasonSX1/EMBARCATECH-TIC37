# Controle de Servo Motor com PWM - Raspberry Pi Pico

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/ac66053d-4e88-4897-8d91-d6297116ef52" alt="Esquema do circuito">
</div>

---

## 🎥 **Demonstração do Projeto**

Para ver o funcionamento do projeto, assista ao vídeo de demonstração disponível em:

🔗 **[Link do vídeo no YouTube](https://youtu.be/5ZP-V13TzgA)**

---

## Comportamento na placa BitDogLab

- **Ao substituir o pino 22 pelo pino 12 (Correspondente ao led Azul fora da matriz de leds na placa bitDogLab) no código principal, eu percebi um comportamento semelhante ao esperado para o servo motor, onde o LED apresentava variação na intensidade da força,mas como o código não fora desenvolvido especificamente para a finalidade do controle do LED, essa variação é pouco perceptível, em comparação ao comportamento esperado do servo motor.**


## Descrição do Projeto

Este projeto implementa o **controle de um servo motor** utilizando **PWM (Pulse Width Modulation)** com o **Raspberry Pi Pico**. O código permite movimentar o servo para diferentes ângulos de forma precisa, utilizando sinais PWM gerados pelo microcontrolador.

### **Principais funcionalidades implementadas:**

1. **Geração de PWM para controle do servo:**
   - Modulação do sinal PWM para definir a posição do servo motor.
   - Ajuste do **duty cycle** para modificar o ângulo do servo.

2. **Definição precisa do posicionamento:**
   - Conversão da largura do pulso para movimentação do servo entre **0° e 180°**.
   - Implementação de um pequeno **delay** para suavizar o movimento.

3. **Código modular e estruturado:**
   - Funções dedicadas para **configuração do PWM** e **controle do servo**.
   - Código adaptável para diferentes tipos de servo motores.

---

## 🛠 **Componentes Utilizados**

- **Microcontrolador:** Raspberry Pi Pico.
- **Servo motor SG90 ou equivalente.**
- **Fonte de alimentação de 5V para o servo.**
- **Resistores e jumpers para conexões.**

### **Conexões do Circuito:**

| Componente      | GPIO do Raspberry Pi Pico |
|---------------|------------------------|
| **Sinal do Servo** | GPIO 15 |
| **VCC do Servo**  | 5V (Fonte externa)  |
| **GND do Servo**  | GND  |

---

## 🔍 **Funcionamento do Código**

### **Fluxo de Operação:**

1. **Inicialização do PWM:**
   - Configura o **GPIO 15** para saída de sinal PWM.
   - Define a **frequência de 50Hz**, adequada para controle de servos.

2. **Cálculo do Duty Cycle:**
   - Ajusta a largura do pulso conforme o ângulo desejado.
   - Valores típicos:
     - **1ms (duty cycle ~5%)** -> **0°**
     - **1.5ms (duty cycle ~7.5%)** -> **90°**
     - **2ms (duty cycle ~10%)** -> **180°**

3. **Movimentação do Servo:**
   - O código permite alterar o ângulo do servo dinamicamente.
   - Pequenos **delays** são adicionados para suavizar a transição.

### **Estrutura do Código:**

1. **Função `init_pwm()`**:
   - Configura o **GPIO 15** como saída PWM.
   - Define a frequência de **50Hz** e inicia o PWM.

2. **Função `set_servo_angle(angle)`**:
   - Converte o ângulo desejado para um **duty cycle** correspondente.
   - Atualiza o sinal PWM para movimentar o servo.

3. **Loop principal (`main()`)**:
   - Alterna entre diferentes posições do servo em intervalos de tempo definidos.

---

## ✅ **Pré-requisitos**

- **Ambiente de Desenvolvimento:** VS Code com extensão do Raspberry pi pico W (com RP2040).
- **Linguagem:** C.
- **Placa de Desenvolvimento:** Raspberry Pi Pico W - BitDogLab.
- **Fonte de Alimentação 5V** para alimentar o servo corretamente.

---

## 🚀 **Instruções de Uso**

1. **Clone este repositório:**
   ```bash
   git clone https://github.com/JasonSX1/EMBARCATECH-TIC37
   ```

2. **Carregue o código na Raspberry Pi Pico:**
   - Pelo VS Code, compile e gere um arquivo `.uf2`, após isso basta executar o comando `Run` com a placa conectada em estado de `BOOTSEL`.

3. **Alimente a placa e o servo motor:**
   - **Conecte uma fonte externa de 5V** para o servo.
   - Certifique-se de que os **GNDs estão conectados em comum**.

4. **Execute o código e observe o movimento do servo!**

---

## 🔹 **Observações**

- Para diferentes modelos de servos, pode ser necessário ajustar os valores do **duty cycle**.
- Se o servo estiver tremendo ou impreciso, experimente ajustar os valores de PWM.
- Para um controle mais avançado, pode-se integrar **botões ou comunicação serial** para definir o ângulo dinamicamente.

---

