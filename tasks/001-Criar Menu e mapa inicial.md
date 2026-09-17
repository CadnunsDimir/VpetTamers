Aqui está o **Backlog** organizado em tarefas (*User Stories* e Tarefas Técnicas) para implementarmos esse fluxo inicial passo a passo no seu `main.c`:

---

# 📋 Backlog — V-Pet Tamers: Sprint 1 (Core Flow & UI)

### 🎯 Objetivo da Sprint

Estabelecer a máquina de estados do jogo, permitindo navegar do **Menu Inicial** para o **Mapa** e abrir a tela de **Pause/Status**, mantendo o controle via Teclado e Gamepad.

---

## 1. 🏗️ Arquitetura & Estruturas de Dados Básicas

* **[TASK-01] Definir a Máquina de Estados Finita (`enum GameState`)**
* Criar o `enum GameState` com os estados: `STATE_MENU`, `STATE_MAP`, `STATE_PAUSE`.
* Adicionar a variável global/local `GameState estado_atual = STATE_MENU;`.


* **[TASK-02] Definir as Structs do Jogador e V-Pet**
* Criar `typedef struct { int x; int y; } Posicao;` (em coordenadas de grade).
* Criar `typedef struct { char nome[30]; int level; int hp; int hp_max; int ataque; int defesa; } VPet;`
* Instanciar o V-Pet inicial do jogador (ex: Level 1, HP 20/20, Ataque 5, Defesa 3).



---

## 2. 🎛️ Feature 1: Menu Inicial

* **[TASK-03] Renderizar e Controlar o Menu Inicial (`STATE_MENU`)**
* Opções visíveis: `[1] Novo Jogo` e `[2] Continuar (Desabilitado)`.
* Sistema de seleção via D-Pad/Setas (alternar índice selecionado) e Botão A/Enter para confirmar.
* Renderizar a opção "Continuar" com cor cinza/desativada para sinalizar que não está disponível.
* Ao confirmar `Novo Jogo`, transicionar para `STATE_MAP`.



---

## 3. 🗺️ Feature 2: Primeiro Mapa & Spawn do Personagem

* **[TASK-04] Criar Matriz do Mapa 2D Simples**
* Matriz simples de grade (ex: 20x15 blocos de 32x32 pixels).
* `0 = Chão (Livre)`, `1 = Parede/Obstáculo`.


* **[TASK-05] Renderização e Movimentação no Mapa (`STATE_MAP`)**
* Desenhar o mapa (chão e paredes) usando retângulos coloridos com `SDL_RenderFillRect`.
* Posicionar o personagem no ponto de spawn inicial (ex: $X=2, Y=2$).
* Permitir movimentação em grade (1 passo por clique/direção) respeitando as colisões da matriz.



---

## 4. ⏸️ Feature 3: Menu de Pause & Tela de Atributos

* **[TASK-06] Controle de Pause (`STATE_PAUSE`)**
* Capturar o botão `START` (Gamepad) ou `ESC` (Teclado) durante o `STATE_MAP` para mudar para `STATE_PAUSE`.
* Pressionar `START` ou `ESC` novamente dentro do Pause para retornar ao `STATE_MAP`.


* **[TASK-07] Renderização dos Status do V-Pet**
* Desenhar a janela de Pause/Status sobrepondo o jogo.
* Exibir o nome, nível, barra/valores de HP, Ataque e Defesa armazenados na `struct` do V-Pet.



---

### 🚀 Próximo Passo

Podemos começar pela **[TASK-01]** e **[TASK-02]**, reestruturando o seu `main.c` para suportar os três estados e as estruturas de dados. Quer que eu gere a primeira versão desse código?