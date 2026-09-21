# Melhorias pos batalha
---

### 1. Sistema de Recompensas e Pós-Batalha (Prioridade Alta)

Agora que a batalha termina e o inimigo é removido do mapa, o jogador precisa sentir que progrediu.

* **Ganho de Experiência (XP) e Nível:**
* Calcular a XP concedida pelo inimigo derrotado (`enemy->level * 10`).
* Atualizar o progresso no `player_pet` e verificar se subiu de nível (`Level Up`), recalculando os atributos base (`HP Max`, `Ataque`, `Defesa`).


* **Mensagem de Vitória no Rodapé:**
* Encadear os estados: `BATTLE_STATE_WIN` $\rightarrow$ Exibe `"VENCEU! +15 XP"` $\rightarrow$ Retorna ao mapa.



---

### 2. Tela / Estado de Derrota (Game Over)

Atualmente, se o HP do player chega a `0`, a batalha só encerra o turno.

* **Estado `STATE_GAME_OVER`:** Criar uma tela simples informando que o V-Pet foi derrotado.
* **Reset / Respawn:** Permitir reiniciar o jogo, restaurar o HP do V-Pet no ponto inicial do mapa, ou levar para um estado de recuperação.

---

### 3. Ciclo de Vida do V-Pet (Atributos de Tamagotchi / V-Pet)

O diferencial de um V-Pet em relação a um RPG tradicional são as necessidades biológicas do pet fora das batalhas:

* **Status Passivos:**
* **Fome (`Hunger`):** Diminui com o tempo (usando um timer baseado em `SDL_GetTicks()`).
* **Felicidade (`Happiness`):** Afetada por vitórias, derrotas ou falta de comida.
* **Energia (`Energy`):** Gasta em batalhas e recuperada ao descansar.


* **Impacto nas Batalhas:** Se a Fome estiver zerada ou a Energia muito baixa, aplicar debuffs no Ataque/Defesa do V-Pet durante o combate.

---

### 4. Interface e Menu de Exploração (Overworld HUD)

Durante a navegação pelo mapa (`STATE_MAP`), exiba as informações vitais sem precisar entrar em batalha:

* Um painel discreto no topo da tela com: Nome do Pet, Nível, Barra de HP e ícones de Fome/Energia.
* Menu de Pausa/Itens para usar poções de cura ou alimentar o pet no mapa.

---

### 5. Persistência de Dados (Save / Load)

Como o estado do mapa (`current_map`) e do pet mudam constantemente:

* **Salvar Estado em Arquivo Binário/Texto:** Salvar a posição do jogador `(x, y)`, o estado dos tiles do mapa (quais inimigos já foram derrotados) e as structs do `VPet` (HP, XP, Nível) em um arquivo `.dat` ou `.json`.
* **Carregar ao Iniciar:** Ler os dados na inicialização do jogo.

---

### 6. Polimento Visual e Animações (Juice)

* **Animações Procedurais de Ação:** Dar um pequeno "tranco/chacoalhada" no sprite do atacante para a frente durante o estado `BATTLE_STATE_ATTACKING`.
* **Piscar em Vermelho ao Receber Dano:** Mudar a cor de preenchimento do sprite temporariamente quando o HP diminuir.
* **Efeitos de Transição de Tela:** Efeito de *Fade-in / Fade-out* (escurecer a tela) ao transitar entre `STATE_MAP` e `STATE_BATTLE`.

---

Por qual dessas áreas você gostaria de começar agora? Recomendo seguir para o **Ganho de Experiência e Nível pós-batalha** ou a **Animação/Polimento dos ataques**.