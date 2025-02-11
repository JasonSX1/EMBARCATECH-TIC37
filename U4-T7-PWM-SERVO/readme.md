# Controle de Servomotor e LED RGB com Raspberry Pi Pico W

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/ac66053d-4e88-4897-8d91-d6297116ef52" alt="Esquema do circuito">
</div>

---

### 🎥 **Demonstração do Projeto**

Para ver o funcionamento do projeto, assista ao vídeo de demonstração disponível em:

🔗 **[Link do vídeo - Em breve]**

---

## Descrição do Projeto

Este projeto implementa o **controle de um servomotor através de PWM** utilizando o **Raspberry Pi Pico W** e a **ferramenta de simulação Wokwi**. Além disso, um **LED RGB** (ânodo azul) na **GPIO 12** é controlado pelo PWM para variar sua intensidade de brilho conforme o movimento do servomotor.

### **Principais funcionalidades implementadas:**

1. **Controle de um servomotor através de PWM (GPIO 22):**
   - Definição de posições específicas: **0°, 90° e 180°**.
   - Movimentação **suave** entre 0° e 180°, com incrementos de 5µs.

2. **Controle do LED RGB (ânodo azul - GPIO 12) via PWM:**
   - O brilho do LED varia proporcionalmente ao ângulo do servomotor.
   - Em 0°: LED com brilho **baixo**.
   - Em 90°: LED com brilho **médio**.
   - Em 180°: LED com brilho **alto**.

3. **Depuração via Serial Monitor:**
   - Exibição das mudanças de PWM do servo e do LED no terminal.
   - Auxílio para monitoramento em tempo real.

---

## Componentes Utilizados

- **Microcontrolador:** Raspberry Pi Pico W.
- **Servomotor:** Motor micro servo padrão.
- **LED RGB (canal azul)**: Conectado ao **GPIO 12**.
- **Alimentação:** 3.3V da Raspberry Pi Pico W.

---

## Funcionamento do Código

### **Fluxo de Operação:**
1. O servo inicia na posição **0° (500µs de PWM)** e o LED azul tem brilho **baixo**.
2. Após 5 segundos, o servo se move para **90° (1470µs de PWM)** e o LED tem brilho **médio**.
3. Após mais 5 segundos, o servo se move para **180° (2400µs de PWM)** e o LED atinge brilho **alto**.
4. O servo então se movimenta suavemente de volta para **0°**, reduzindo gradualmente o brilho do LED.

---

## Pré-requisitos

- **Ambiente de Desenvolvimento:** VS Code configurado com o **Pico SDK**.
- **Linguagem:** C.
- **Simulador:** Wokwi e BitDogLab.
- **Placa de Desenvolvimento:** Raspberry Pi Pico W.
- **Monitor Serial:** Putty, minicom ou Monitor Serial do Arduino IDE.

---

## Instruções de Uso

1. **Clone este repositório:**
   ```bash
   git clone https://github.com/SEU_GITHUB/controle-servo-led
   ```

2. **Acesse a branch de testes para o BitDogLab:**
   ```bash
   git checkout u7-t7-teste-bitdoglab
   ```

3. **Compile e carregue o código:**
   - Use o **Raspberry Pi Pico SDK** para compilar.
   - Copie o **arquivo `.uf2`** para a **placa Raspberry Pi Pico W**.

4. **Teste no Wokwi ou BitDogLab:**
   - No **Wokwi**, o código não funcionou corretamente.
   - No **BitDogLab**, os testes ainda estão pendentes.

5. **Abra um terminal serial para monitoramento:**
   - Configuração recomendada: `115200 baud`.
   - Use **Putty**, **minicom**, ou **Arduino IDE**.

---

## Resultados dos Testes no BitDogLab

🔹 **Em breve, serão adicionados os resultados dos testes realizados no BitDogLab.**

---

## Observações

- **PWM configurado corretamente** para controlar o servomotor.
- **Brilho do LED azul controlado dinamicamente pelo PWM**.
- **Depuração via Serial Monitor** para visualizar os valores do PWM.
- **Branch `U7-T7-teste-Bitdoglab` criada para testes no BitDogLab**.
- **Código testado no Wokwi, mas não funcionou corretamente por conta de limitações no funcionamento do wokwi com PWM.**

---

