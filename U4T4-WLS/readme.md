# Exibição de Números em uma Matriz de LEDs 5x5

---

<div align="center">
  <img src="https://github.com/user-attachments/assets/f60302d4-a989-43ed-bc72-693ee507aac9" alt="Descrição da imagem">
</div>

---

## Vídeo de Demonstração

<p align="center">
  <a href="https://youtu.be/uTl-7Aydqb8">
    <img src="https://img.youtube.com/vi/uTl-7Aydqb8/maxresdefault.jpg" alt="Video da Atividade">
  </a>
</p>

[![Assista ao vídeo](https://img.youtube.com/vi/uTl-7Aydqb8/maxresdefault.jpg)](https://youtu.be/uTl-7Aydqb8)


#### Ou você pode acessar o vídeo clicando aqui: [Link para o vídeo](https://youtu.be/uTl-7Aydqb8)

---

Este projeto faz parte da quarta atividade obrigatória da unidade 4 do curso Embarcatech em sistemas embarcados e tem como objetivo utilizar o simulador Wokwi e o microcontrolador Raspberry Pi Pico para exibir números de 0 a 9 em uma matriz de LEDs 5x5 WS2812, controlada por dois botões físicos.

## Descrição do Projeto

O sistema exibe números na matriz de LEDs e permite ao usuário alternar entre eles utilizando dois botões. O projeto inclui controle de luminosidade para reduzir o brilho excessivo e debounce nos botões para garantir a precisão na troca dos números.

### Funcionalidades Principais

1. Exibição de números de 0 a 9 na matriz de LEDs.
2. Botão A para avançar para o próximo número.
3. Botão B para retroceder para o número anterior.
4. Ajuste da luminosidade para evitar brilho excessivo.
5. Controle de debounce para evitar leituras incorretas dos botões.
6. LED RGB externo piscando para indicar funcionamento do sistema.

---

## Componentes Utilizados

- Teclado de entrada: Dois botões físicos conectados ao Raspberry Pi Pico.
- Microcontrolador: Raspberry Pi Pico.
- Matriz de LEDs WS2812B (NeoPixel) 5x5.
- Resistores de pull-up para os botões.
- LED RGB externo para indicar funcionamento.

---

## Controle do Sistema

O sistema funciona da seguinte maneira:

- O **botão A** incrementa o número exibido na matriz, voltando para 0 após 9.
- O **botão B** decrementa o número exibido na matriz, voltando para 9 após 0.
- A luminosidade dos LEDs é ajustada para 60% do brilho máximo.
- O LED RGB externo pisca continuamente para indicar que o sistema está em funcionamento.

---

## Pré-requisitos

- Ambiente de Desenvolvimento: VS Code configurado com o Kit de Desenvolvimento Pico SDK.
- Simulador: [Wokwi](https://wokwi.com) para simulação dos componentes.
- Linguagem: C.
- Controle de Versão: Git e GitHub para versionamento do código.

---

## Instruções de Uso

1. Clone este repositório:
   ```bash
   git clone https://github.com/JasonSX1/EMBARCATECH-TIC37/tree/main/U4T4-WLS

2. Abra o projeto no VS Code.

3. Configure o Pico SDK e o simulador Wokwi seguindo esta documentação.

4. Compile e execute o código no simulador Wokwi ou na sua placa BitDogLab.

5. Utilize os botões para alternar os números exibidos na matriz.
