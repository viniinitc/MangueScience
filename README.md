# MangueScience 🦀🎶

Um jogo de ritmo feito em C com a biblioteca Raylib, inspirado no movimento Manguebeat. Desvie e acerte as notas no ritmo da lama ao caos!

---

## Como Jogar

- **Menu / Seleção**: Use as `SETAS` para navegar pelas músicas e `ENTER` para confirmar.
- **Gameplay**: 
  - Use `SETAS (CIMA, BAIXO, ESQUERDA, DIREITA)` para direcionar o jogador em direção às notas.
  - Pressione `1` para pausar/retomar a música a qualquer momento.

---

## Tecnologias e Conceitos Utilizados

- **Linguagem C**: Estruturação e lógica central do jogo.
- **Raylib**: Renderização gráfica 2D e gerenciamento de streams de áudio.
- **Estrutura de Dados**: Implementação de uma **Lista Duplamente Encadeada** para gerenciar, mover e destruir as notas musicais na tela de forma dinâmica e eficiente.
- **aubio**: Análise de áudio em tempo real, detecção de onsets, beats e extração de features musicais.
- **Algoritmo de Ordenação (InsertSort)**: Ordena o score de pontuação do usuário.
- **Máquina de Estados**: Controle limpo de transição entre as telas (Menu, Seleção e Jogo).
