# Repositório EMBARCATECH-TIC37 - Atividades de Sistemas Embarcados

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/e2df901b-a732-40cd-b8ba-0015ec9fbfa7" alt="Logo do repositório">
</div>

---

Este repositório central contém as atividades desenvolvidas para o curso **EMBARCATECH-TIC37**, abordando projetos práticos utilizando o **Raspberry Pi Pico W** e conceitos fundamentais de sistemas embarcados. Cada atividade foi implementada seguindo os requisitos específicos e testada na plataforma **BitDogLab**.

## 📌 Estrutura das Atividades

As atividades estão organizadas em sub-repositórios, conforme o cronograma de desenvolvimento:

---

## Unidade 4:

### 🔹 [Atividade 1 - Exibição de Números em uma Matriz de LEDs 5x5 (27/01)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4T4-WLS)

> **Descrição:** Implementação de um sistema utilizando o **simulador Wokwi** e o **Raspberry Pi Pico**, para exibir números de **0 a 9** em uma matriz de LEDs **5x5 WS2812**, controlada por dois botões físicos. **O projeto inclui:**
> - Controle para alternar entre os números utilizando botões.
> - Ajuste de luminosidade para evitar brilho excessivo.
> - Implementação de **debounce** nos botões para garantir precisão.
> - Um LED RGB externo piscando para indicar funcionamento do sistema.

### 🔹 [Atividade 2 - Parte 1: Temporizador de Um Disparo (One-Shot) (29/01)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T5-ClockTemp-Ativ2)

> **Descrição:** Implementação de um **temporizador de um disparo** (`add_alarm_in_ms()`), onde um **botão** aciona a sequência de desligamento dos LEDs em intervalos de **3 segundos**. O sistema garante que um novo acionamento só possa ocorrer após a finalização do ciclo.

### 🔹 [Atividade 2 - Parte 2: Sequência de Cores e Controle via Botão (29/01)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T5-ClockTemp-Ativ3)

> **Descrição:** Extensão da Atividade 2, adicionando **controle de cores** para LEDs RGB na plataforma **BitDogLab**. O sistema exibe a sequência de transições **branco → roxo → vermelho → apagado**, garantindo que apenas um ciclo seja executado por vez.

### 🔹 [Atividade 3 - Comunicação Serial e Controle de Periféricos UART/I2C (03/02)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T6-UART-I2C-LED)

> **Descrição:** Esta atividade visa consolidar o conhecimento sobre **comunicação serial (UART e I2C)** no **RP2040**, utilizando a placa **BitDogLab**. O projeto integra o controle de LEDs comuns, **LEDs endereçáveis WS2812**, botões de acionamento com **interrupções e debounce**, e exibição de caracteres no **display SSD1306**. **O projeto inclui:**  
> - **Recebimento de caracteres via Serial Monitor (VS Code)** e exibição no display SSD1306.  
> - **Exibição de números na matriz de LEDs 5x5 WS2812** quando um número entre 0 e 9 for digitado.  
> - **Controle do LED RGB:**  
>   - **Botão A (GPIO 5):** Alterna o estado do LED Verde e exibe a atualização no **display e Serial Monitor**.  
>   - **Botão B (GPIO 6):** Alterna o estado do LED Azul e exibe a atualização no **display e Serial Monitor**.  
> - **Modificação da biblioteca font.h** para incluir caracteres minúsculos.  
> - **Uso de interrupções (IRQ) para os botões** e **implementação de debounce via software**.   

### 🔹 [Atividade 4 - Controle de Servomotor com PWM (10/02)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T7-PWM-SERVO)

> **Descrição:** Implementação de um sistema para controle de um **servomotor** utilizando **PWM (Pulse Width Modulation)** no **RP2040**. O projeto permite movimentar o servo em diferentes ângulos de forma precisa e suave.  
> - Configuração do **PWM na GPIO 22** com uma frequência de 50Hz, ideal para servomotores.  
> - Controle do servo para posicionamento em **0°, 90° e 180°**, ajustando o ciclo de trabalho do sinal PWM.  
> - Implementação de um deslocamento progressivo entre os ângulos, proporcionando transições mais suaves.  

---

## 🛠️ Tecnologias Utilizadas

- **Placa:** Raspberry Pi Pico W
- **Linguagem:** C
- **SDK:** Raspberry Pi Pico SDK
- **Simulação:** BitDogLab e Wokwi
- **Ambiente de Desenvolvimento:** VS Code

## 🚀 Como Clonar o Repositório Central

```bash
git clone https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main
```

Acesse cada sub-repositório pelos links acima para mais detalhes sobre cada atividade.

---

📌 **Status:** Todas as atividades foram implementadas e testadas com sucesso no **BitDogLab e Wokwi**, seguindo os requisitos estabelecidos.
