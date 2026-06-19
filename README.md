# MangueScience 🦀🎶

Jogo de ritmo feito em C com Raylib, inspirado no movimento Manguebeat de Recife. Acerte as notas no ritmo de Chico Science & Nação Zumbi!

---

## Gameplay

Acesse a gameplay - [Clique aqui](https://drive.google.com/file/d/1SoiAiFkHP1SiqA3jQWZd58Zb1smQrOWB/view?usp=sharing)

---

## Como Compilar

Certifique-se de possuir o [Raylib](https://www.raylib.com) instalado, então:

**Mac**
```bash
cd build
./premake5.osx gmake
cd ..
make
./game.sh
```

**Linux**
```bash
cd build
./premake5 gmake
cd ..
chmod +x game.sh
./game.sh
```

**Windows**
```bash
cd MangueScience
bin\Debug\MangueScience.exe
```

---

## Como Jogar

| Ação | Tecla |
|------|-------|
| Navegar nos menus | `↑ ↓` |
| Confirmar | `ENTER` |
| Acertar notas | `↑ ↓ ← →` |
| Pausar / Retomar | `SPACE` |

---

## Tecnologias

- **C** — lógica central do jogo
- **Raylib** — renderização gráfica e áudio
- **Lista Duplamente Encadeada** — gerenciamento dinâmico das notas
- **Insertion Sort** — ordenação do ranking de pontuações
- **Máquina de Estados** — controle de telas (Menu, Seleção, Gameplay, Pause, Score)

---

## Créditos Musicais

As músicas utilizadas neste jogo pertencem aos seus respectivos autores e são usadas apenas para fins educacionais e não comerciais.

- **Chico Science & Nação Zumbi** — Praieira, Da Lama ao Caos, Manguetown, Rios Pontes e Overdrives
- **Mundo Livre S/A** — Maracatu Atômico

Este projeto não possui fins lucrativos. Todos os direitos das músicas pertencem aos artistas e suas gravadoras.
