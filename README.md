
# 🎮 V-Pet Tamers [`v1.0`]

> **RPG Tático em Grade de Baixa Dificuldade & Exploração de Baixo Nível (C99 + SDL3)**

`Linguagem: C (C99)` | `Engine: SDL3` | `Arquitetura: Grid-Based 2D` | `Suporte: Joystick / Teclado`

---

## 1. Sobre o Projeto

**V-Pet Tamers** é um RPG 2D com movimentação em grade e sistema de combate por turnos. Desenvolvido do zero em C puro com SDL3, o projeto tem como principal objetivo pedagógico demonstrar como um computador processa estruturas de dados, gerencia estados de execução e manipula posições de memória em tempo real.

---

## 2. Arquitetura & Aprendizado de Hardware

O que este projeto ensina sobre o funcionamento da CPU e RAM:

* **Alocação de Memória Contígua (Arrays e Structs):** O mapa do jogo é armazenado como uma matriz bidimensional `int mapa[ALTURA][LARGURA]` na memória RAM.
* **Manipulação Direta por Ponteiros:** Atributos do Tamer (Jogador) e dos Companheiros (V-Pets) são passados para funções via ponteiros para modificação direta em memória.
* **Máquina de Estados Finita (FSM):** A CPU alterna o fluxo de execução entre Exploração (Grade), Batalha (Turnos) e Menus através de `enum GameState` e instruções `switch/case` no Game Loop.
* **Entrada do Usuário & Polling de Dispositivos:** Mapeamento do D-Pad do controle e setas do teclado convertidos em deltas de coordenadas inteiras no grid.

---

## 3. Funcionalidades da Versão 1.0 (MVP)

### 🗺️ Movimentação & Exploração em Grade (Grid System)
* Grade com células de tamanho fixo (ex: 32x32 pixels).
* Movimentação discreta "passo a passo" do Tamer no mapa.
* Matriz de colisão para bloquear paredes e obstáculos (`0 = Livre`, `1 = Parede`).
* Suporte nativo para Joystick/Gamepad (D-Pad e Analógico) e Teclado.

### ⚔️ Sistema de Batalha por Turnos (Turn-Based Combat)
* Transição automática para o modo de batalha ao colidir com um inimigo no grid.
* Lógica de turnos rígida: Turno do Tamer/V-Pet → Turno do Inimigo.
* Ações básicas do menu de combate: **Atacar**, **Habilidade**, **Item** e **Fugir**.
* Cálculo de dano aritmético processado diretamente pela CPU baseado nos atributos.

### 📊 Atributos, Status e Memória RAM
Estrutura de dados dedicada para gerenciar a entidade principal e inimigos:

```c
typedef struct {
    int x;
    int y;
} Posicao;

typedef struct {
    char nome[30];
    int hp_atual;
    int hp_max;
    int ataque;
    int defesa;
    int nivel;
    int xp_atual;
    Posicao pos;
} Entidade;

```

---

## 4. Tabela de Atributos & Fórmulas da v1

| Atributo | Tipo de Dado | Descrição / Uso na Memória |
| --- | --- | --- |
| `hp_atual / hp_max` | `int` (32 bits) | Pontos de vida. Quando chega a 0, aciona estado de Derrota. |
| `ataque` | `int` (32 bits) | Valor base para o cálculo de dano na CPU. |
| `defesa` | `int` (32 bits) | Mitiga o dano recebido antes de subtrair do HP. |
| `pos` | `Posicao struct` | Coordenadas X, Y indexadas diretamente na matriz do mapa. |

### 🧮 Fórmula de Dano da v1:

```c
Dano = (Ataque_Atacante - (Defesa_Defensor / 2));
if (Dano < 1) Dano = 1; // Garante dano mínimo de 1

```

---

## 5. Estrutura do Loop Principal (Game Loop)

O ciclo de execução contínuo no `main.c` opera segundo o fluxo clássico:

`1. Input (Polling)` ➔ `2. Update (Máquina de Estados + Aritmética)` ➔ `3. Render (SDL_RenderFillRect)`

---

*V-Pet Tamers • Projeto de Aprendizado de Arquitetura de Computadores e C/SDL3*