Com base na documentação do README e no progresso atual do projeto (onde cobrimos a movimentação no grid, a colisão, a máquina de estados e o Menu de Pause com HUD de atributos), o próximo passo natural dentro do escopo da **Versão 1.0 (MVP)** é iniciar a **Feature 4: Sistema de Batalha por Turnos (STATE_BATTLE)**.

Para mantermos o fluxo organizado e evolutivo em passos curtos, a implementação da batalha deve começar pela **Estrutura de Transição de Estado e Spawn do Inimigo no Mapa**.

---

### ⚔️ Próxima Etapa: Batalha por Turnos (Passo 1 de 3)

#### [TASK-08] Inimigo no Mapa & Transição para `STATE_BATTLE`

1. **Representação do Inimigo na RAM (`struct Entidade` / `VPet`):**
* Criar uma variável/instância de inimigo no mapa com posição $(X, Y)$ na grade e atributos iniciais (ex: `HP: 20/20`, `Ataque: 8`, `Defesa: 3`).


2. **Identificador na Matriz de Colisão:**
* Adicionar o valor `2` na matriz do mapa para representar o bloco do inimigo.


3. **Detecção de Colisão e Alternância de Estado:**
* Ao tentar mover o jogador para a célula do inimigo, acionar a transição de estado da FSM de `STATE_MAP` para `STATE_BATTLE`.


4. **Fundo da Tela de Batalha:**
* Desenhar a transição de tela e o painel de arena de combate em `STATE_BATTLE`.



---

Quer iniciar a **[TASK-08]** implementando a detecção da colisão com o inimigo e a transição do `STATE_MAP` para `STATE_BATTLE`?