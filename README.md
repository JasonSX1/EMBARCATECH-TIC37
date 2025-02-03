# Repositório EMBARCATECH-TIC37 - Atividades de Sistemas Embarcados

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/e2df901b-a732-40cd-b8ba-0015ec9fbfa7" alt="Logo do repositório">
</div>

---

Este repositório central contém as atividades desenvolvidas para o curso **EMBARCATECH-TIC37**, abordando projetos práticos utilizando o **Raspberry Pi Pico W** e conceitos fundamentais de sistemas embarcados. Cada atividade foi implementada seguindo os requisitos específicos e testada na plataforma **BitDogLab**.

## 📌 Estrutura das Atividades

As atividades estão organizadas em sub-repositórios, conforme o cronograma de desenvolvimento:

### 🔹 [Atividade 1 - Exibição de Números em uma Matriz de LEDs 5x5 (27/01)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4T4-WLS)

> **Descrição:** Implementação de um sistema utilizando o **simulador Wokwi** e o **Raspberry Pi Pico**, para exibir números de **0 a 9** em uma matriz de LEDs **5x5 WS2812**, controlada por dois botões físicos. O projeto inclui:
> - Controle para alternar entre os números utilizando botões.
> - Ajuste de luminosidade para evitar brilho excessivo.
> - Implementação de **debounce** nos botões para garantir precisão.
> - Um LED RGB externo piscando para indicar funcionamento do sistema.

### 🔹 [Atividade 2 - Parte 1: Temporizador de Um Disparo (One-Shot) (29/01)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T5-ClockTemp-Ativ2)

> **Descrição:** Implementação de um **temporizador de um disparo** (`add_alarm_in_ms()`), onde um **botão** aciona a sequência de desligamento dos LEDs em intervalos de **3 segundos**. O sistema garante que um novo acionamento só possa ocorrer após a finalização do ciclo.

### 🔹 [Atividade 2 - Parte 2: Sequência de Cores e Controle via Botão (29/01)](https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4-T5-ClockTemp-Ativ3)

> **Descrição:** Extensão da Atividade 2, adicionando **controle de cores** para LEDs RGB na plataforma **BitDogLab**. O sistema exibe a sequência de transições **branco → roxo → vermelho → apagado**, garantindo que apenas um ciclo seja executado por vez.

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
