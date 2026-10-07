#include <math.h> // Necessário para usar pow()

#define BATTLE_ATTACK_TILES 3.0f
#define BATTLE_ISO_DEPTH_STEP 70.0f
#define BATTLE_ATTACK_MILIS_DURATION 1000

// Converte coordenadas do Grid (X,Y) para Pixels Isométricos (ScreenX, ScreenY)
void GridToIsometric(int grid_x, int grid_y, float tile_w, float tile_h, float offset_x, float offset_y, float* out_x, float* out_y) {
    *out_x = (grid_x - grid_y) * (tile_w / 2.0f) + offset_x;
    *out_y = (grid_x + grid_y) * (tile_h / 2.0f) + offset_y;
}

void RenderBkMbarcajamonBySprite(SDL_Renderer *renderer, SDL_Texture *texture, Coordinates position, int column, int row, float scale){
    int full_image_w = 249;
    int full_image_h = 1003;
    int full_image_col = 2;
    int full_image_row = 5;
    int sprite_h = full_image_h / full_image_row;
    int sprite_w = full_image_w / full_image_col;
    float sprite_scale = 0.4f; 

    SDL_FRect src = {
        column * sprite_w,
        row * sprite_h,
        sprite_w,
        sprite_h
    };

    SDL_FRect dst = {
        position.x,
        position.y,
        sprite_w * sprite_scale * scale,
        sprite_h * sprite_scale * scale
    };

    SDL_RenderTexture(
        renderer,
        texture,
        &src,
        &dst
    );
}

void RenderVPetSprite(SDL_Renderer* renderer, VPetType tipo, float x, float y, float scale) {
    for (size_t i = 0; i < PET_PART_LIST_BATTLE_MODE_COUNT; i++)
    {
        if (PET_PART_LIST_BATTLE_MODE[i].type == tipo)
        {
            VpetPart* parts = PET_PART_LIST_BATTLE_MODE[i].vparts;
            size_t count = PET_PART_LIST_BATTLE_MODE[i].vparts_count;

            // 2. Garante o modo de transparência ativo para auras/chamas
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

            // 3. Renderiza cada parte do VPet selecionado
            for (size_t i = 0; i < count; i++) {
                VpetPart part = parts[i];
                float y_offset = 0.0f;

                // Animação da chama amarela interna do Jaguapirumon (índice 1 no array)
                if (tipo == PET_JAGUAPIRUMON && i == 1) {
                    // printf("criando chama\n");
                    y_offset = sinf((float)SDL_GetTicks() / 80.0f) * (2.0f * scale);
                }

                // Calcula as posições na tela escaladas (incluindo deslocamento de animação)
                SDL_FRect dst_rect = {
                    x + (part.rect.x * scale),
                    y + (part.rect.y * scale) + y_offset,
                    part.rect.w * scale,
                    part.rect.h * scale
                };

                // Usa o mesmo volume estilizado dos VPets do mapa.
                RenderVpetPartVolume(renderer, dst_rect, part.color, scale);
            }
            return;
        }        
    }

    printf("[BattleMode][RenderVPetSprite] Vpet id=%d não encontrado em modo de batalha\n", tipo);
}

void RenderIsoTile(SDL_Renderer* renderer, float centerX, float centerY, float tileW, float tileH, Color color) {
    // 4 vértices do losango isométrico
    SDL_Vertex vertices[4];
    
    SDL_FPoint top    = { centerX, centerY - (tileH / 2.0f) };
    SDL_FPoint right  = { centerX + (tileW / 2.0f), centerY };
    SDL_FPoint bottom = { centerX, centerY + (tileH / 2.0f) };
    SDL_FPoint left   = { centerX - (tileW / 2.0f), centerY };

    SDL_FColor col = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };

    vertices[0].position = top;    vertices[0].color = col;
    vertices[1].position = right;  vertices[1].color = col;
    vertices[2].position = bottom; vertices[2].color = col;
    vertices[3].position = left;   vertices[3].color = col;

    // Índices para formar 2 triângulos (quadrilátero preenchido)
    int indices[6] = { 0, 1, 2, 0, 2, 3 };

    SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
}

// Parede Traseira Esquerda (ao longo de X = 0, descendo para a esquerda)
void RenderIsoWallNorthWest(SDL_Renderer* renderer, float centerX, float centerY, float tileW, float tileH, float wallH, Color color) {
    SDL_Vertex vertices[4];
    
    SDL_FPoint base_top  = { centerX, centerY - (tileH / 2.0f) };
    SDL_FPoint base_left = { centerX - (tileW / 2.0f), centerY };

    SDL_FColor col = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };

    vertices[0].position = (SDL_FPoint){ base_top.x, base_top.y - wallH };   vertices[0].color = col;
    vertices[1].position = base_top;                                          vertices[1].color = col;
    vertices[2].position = base_left;                                         vertices[2].color = col;
    vertices[3].position = (SDL_FPoint){ base_left.x, base_left.y - wallH }; vertices[3].color = col;

    int indices[6] = { 0, 1, 2, 0, 2, 3 };
    SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
}

// Parede Traseira Direita (ao longo de Y = 0, descendo para a direita)
void RenderIsoWallNorthEast(SDL_Renderer* renderer, float centerX, float centerY, float tileW, float tileH, float wallH, Color color) {
    SDL_Vertex vertices[4];
    
    SDL_FPoint base_top   = { centerX, centerY - (tileH / 2.0f) };
    SDL_FPoint base_right = { centerX + (tileW / 2.0f), centerY };

    SDL_FColor col = { color.r / 255.0f, color.g / 255.0f, color.b / 255.0f, color.a / 255.0f };

    vertices[0].position = (SDL_FPoint){ base_top.x, base_top.y - wallH };     vertices[0].color = col;
    vertices[1].position = base_top;                                            vertices[1].color = col;
    vertices[2].position = base_right;                                          vertices[2].color = col;
    vertices[3].position = (SDL_FPoint){ base_right.x, base_right.y - wallH }; vertices[3].color = col;

    int indices[6] = { 0, 1, 2, 0, 2, 3 };
    SDL_RenderGeometry(renderer, NULL, vertices, 4, indices, 6);
}

void RenderBattleScreenScenario(SDL_Renderer* renderer, float screenWidth, float screenHeight, OponentsPositioning* positioning) {
    // 1. Fundo Geral da Tela
    SDL_FRect bg_dark = { 0.0f, 0.0f, screenWidth, screenHeight };
    SDL_SetRenderDrawColor(renderer, 10, 10, 15, 255);
    SDL_RenderFillRect(renderer, &bg_dark);

    // --- Parâmetros da Arena Isométrica Ampliada ---
    const int ARENA_SIZE = 5;
    float tile_w = 140.0f; // Escala ampliada da largura do tile
    float tile_h = 70.0f;  // Escala ampliada da altura do tile (proporção 2:1)

    float offset_x = screenWidth / 2.0f;
    float offset_y = 100.0f; // Elevado ligeiramente para enquadrar a arena maior

    float wall_height = 110.0f; // Parede mais alta para acompanhar a nova escala

    // --- 2. PAREDES TRASEIRAS ---
    Color color_wall_nw = { 65, 70, 80, 255 }; // Parede Esquerda (Luz)
    Color color_wall_ne = { 45, 50, 60, 255 }; // Parede Direita (Sombra)

    // A) Parede Traseira Esquerda (Trilho X = 0, varia Y)
    for (int y = 0; y < ARENA_SIZE; y++) {
        float iso_x, iso_y;
        GridToIsometric(0, y, tile_w, tile_h, offset_x, offset_y, &iso_x, &iso_y);
        RenderIsoWallNorthWest(renderer, iso_x, iso_y, tile_w, tile_h, wall_height, color_wall_nw);
    }

    // B) Parede Traseira Direita (Trilho Y = 0, varia X)
    for (int x = 0; x < ARENA_SIZE; x++) {
        float iso_x, iso_y;
        GridToIsometric(x, 0, tile_w, tile_h, offset_x, offset_y, &iso_x, &iso_y);
        RenderIsoWallNorthEast(renderer, iso_x, iso_y, tile_w, tile_h, wall_height, color_wall_ne);
    }

    // --- 3. CHÃO DA ARENA ---
    Color color_floor_a = { 70, 80, 75, 255 };
    Color color_floor_b = { 55, 65, 60, 255 };

    for (int y = 0; y < ARENA_SIZE; y++) {
        for (int x = 0; x < ARENA_SIZE; x++) {
            float iso_x, iso_y;
            GridToIsometric(x, y, tile_w, tile_h, offset_x, offset_y, &iso_x, &iso_y);

            Color current_tile_color = ((x + y) % 2 == 0) ? color_floor_a : color_floor_b;
            RenderIsoTile(renderer, iso_x, iso_y, tile_w, tile_h, current_tile_color);

            // Inimigo recuado no topo (Grid: X=1, Y=0)
            if (x == 2 && y == 0) {
                positioning->enemy.x = iso_x;
                positioning->enemy.y = iso_y;
            }

            // BkMbaracajamon avançado na frente (Grid: X=3, Y=3)
            if (x == 2 && y == 4) {
                positioning->player.x = iso_x;
                positioning->player.y = iso_y;
            }
        }
    }
}

void RenderPetHUD(SDL_Renderer* renderer, float x, float y, float width, float height, VPet* pet) {
    // 1. Fundo do Painel (Azul escuro semi-transparente)
    SDL_FRect hud_rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 20, 25, 40, 230);
    SDL_RenderFillRect(renderer, &hud_rect);

    // 2. Borda do Painel (Cinza claro)
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderRect(renderer, &hud_rect);

    // 3. Texto do Nome e Nível (se o nível for > 0)
    char buffer[64];
    if (pet->level > 0) {
        snprintf(buffer, sizeof(buffer), "%s (LVL %d)", pet->name, pet->level);
    } else {
        snprintf(buffer, sizeof(buffer), "%s", pet->name);
    }
    RenderText(renderer, buffer, COLOR_WHITE, x + 15.0f, y + 12.0f, 2.5f);

    // 4. Texto do HP Numérico
    snprintf(buffer, sizeof(buffer), "HP: %d/%d", pet->hp, pet->hp_max);
    RenderText(renderer, buffer, COLOR_WHITE, x + 15.0f, y + 42.0f, 2.5f);
}

void RenderBattleMenu(SDL_Renderer* renderer, float screenWidth, float screenHeight, BattleState battle_state) {
    // --- Caixa de Comandos de Batalha (Rodapé) ---
    SDL_FRect menu_box = { 20.0f, screenHeight - 140.0f, screenWidth - 40.0f, 120.0f };
    SDL_SetRenderDrawColor(renderer, 15, 20, 35, 245);
    SDL_RenderFillRect(renderer, &menu_box);
    SDL_SetRenderDrawColor(renderer, 200, 200, 220, 255);
    SDL_RenderRect(renderer, &menu_box);

    // Opções do Turno
    Color col_atacar = (battle_state == BATTLE_STATE_MENU_ATTACK) ? COLOR_MENU_BUTTON_SELECTED : COLOR_WHITE;
    Color col_fugir  = (battle_state == BATTLE_STATE_MENU_RUN) ? COLOR_MENU_BUTTON_SELECTED : COLOR_WHITE;

    RenderText(renderer, "ATACAR", col_atacar, 60.0f, screenHeight - 100.0f, 3.5f);
    RenderText(renderer, "FUGIR",  col_fugir,  320.0f, screenHeight - 100.0f, 3.5f);
}

void RenderBattleMessage(SDL_Renderer* renderer, float screenWidth, float screenHeight, const char* message) {
    // Caixa de mensagem no rodapé (mesmas dimensões do menu)
    SDL_FRect msg_box = { 20.0f, screenHeight - 140.0f, screenWidth - 40.0f, 120.0f };
    SDL_SetRenderDrawColor(renderer, 15, 20, 35, 245);
    SDL_RenderFillRect(renderer, &msg_box);
    SDL_SetRenderDrawColor(renderer, 200, 200, 220, 255);
    SDL_RenderRect(renderer, &msg_box);

    // Renderiza o texto centralizado/alinhado no painel
    size_t msg_len = strlen(message);
    float text_pixel_size =  msg_len <= 35 ? 3.0f: 2.0f;
    RenderText(renderer, message, COLOR_WHITE, 50.0f, screenHeight - 95.0f, text_pixel_size);
}

void TriggerBattleMessage(BattleSystem* battle, BattleState new_state, const char* text, Uint64 duration_ms) {
    battle->current_state = new_state;
    battle->message_start_time = SDL_GetTicks();
    battle->message_duration = duration_ms;
    snprintf(battle->current_message, sizeof(battle->current_message), "%s", text);
}

static float GetBattleAttackProgress(BattleSystem battle) {
    if (battle.current_state != BATTLE_STATE_ATTACKING &&
        battle.current_state != BATTLE_STATE_ENEMY_ATACKING) {
        return 0.0f;
    }

    Uint64 elapsed = SDL_GetTicks() - battle.message_start_time;
    if (battle.message_duration == 0 || elapsed >= battle.message_duration) {
        return 0.0f;
    }

    float progress = (float)elapsed / (float)battle.message_duration;
    return progress < 0.5f ? progress * 2.0f : (1.0f - progress) * 2.0f;
}

static void GetBattleAttackOffsets(
    BattleSystem battle,
    OponentsPositioning positioning,
    float* player_offset_x,
    float* player_offset_y,
    float* enemy_offset_x,
    float* enemy_offset_y
) {
    float delta_x = positioning.enemy.x - positioning.player.x;
    float delta_y = positioning.enemy.y - positioning.player.y;
    float distance = sqrtf((delta_x * delta_x) + (delta_y * delta_y));
    float progress = GetBattleAttackProgress(battle);

    if (distance <= 0.0f || progress <= 0.0f) {
        return;
    }

    const float attack_distance = BATTLE_ATTACK_TILES * BATTLE_ISO_DEPTH_STEP * progress;
    float direction_x = delta_x / distance;
    float direction_y = delta_y / distance;

    if (battle.current_state == BATTLE_STATE_ATTACKING) {
        *player_offset_x = direction_x * attack_distance;
        *player_offset_y = direction_y * attack_distance;
    } else {
        *enemy_offset_x = -direction_x * attack_distance;
        *enemy_offset_y = -direction_y * attack_distance;
    }
}

void RenderBattleScreen(SDL_Renderer* renderer, float screenWidth, float screenHeight, BattleSystem battle, SDL_Texture *texture) {
    OponentsPositioning oponentsPositioning = { 0, 0, 0, 0 };
    
    RenderBattleScreenScenario(renderer, screenWidth, screenHeight, &oponentsPositioning);

    float player_offset_x = 0.0f;
    float player_offset_y = 0.0f;
    float enemy_offset_x = 0.0f;
    float enemy_offset_y = 0.0f;
    GetBattleAttackOffsets(
        battle,
        oponentsPositioning,
        &player_offset_x,
        &player_offset_y,
        &enemy_offset_x,
        &enemy_offset_y
    );

    // --- Representação Visual das Entidades ---    

    if (battle.current_state != BATTLE_STATE_WIN && battle.current_state != BATTLE_STATE_WIN_CLOSE_BATTLE)
    {
        // 1. Inimigo (Renderizado PRIMEIRO para ficar ao fundo)
       RenderVPetSprite(
            renderer, 
            battle.enemy.type,
            oponentsPositioning.enemy.x + enemy_offset_x - (16.0f * 2.5f), // Centraliza horizontalmente o sprite de 32px
            oponentsPositioning.enemy.y + enemy_offset_y - (24.0f * 2.5f), // Apoia a base do sprite no centro do tile
            2.5f
        );
    }
    
    if (battle.current_state != BATTLE_STATE_LOST) {
       // 2. BkMbaracajamon (Renderizado DEPOIS para se sobrepor caso cruzem a linha de visão)
        // RenderVPetSprite(
        //     renderer, 
        //     battle.player.type,
        //     oponentsPositioning.player.x + player_offset_x - (15.0f * 2.5f), 
        //     oponentsPositioning.player.y + player_offset_y - (25.0f * 2.5f), 
        //     2.5f
        // );
       

        float dest_w = 64; // Largura final desenhada (80px)
        float dest_h = 150; // Altura final desenhada (80px)

        // 2. Calcule a posição para alinhar o SPRITE exatamente onde o Retângulo ficava
        Coordinates pos = {
            // Alinha o centro horizontal do sprite com a posição base
            (oponentsPositioning.player.x + player_offset_x) - dest_w,
            
            // Alinha a base (pés) do sprite com a posição base
            (oponentsPositioning.player.y + player_offset_y) - dest_h
        };
        RenderBkMbarcajamonBySprite(renderer, texture, pos, 0, 4, 2.5f);
    }

    

    // --- Painéis de Status (HUD) ---
    float hud_width = 380.0f;
    float hud_height = 80.0f;

    // Status do Inimigo (Topo Direito)
    float enemy_hud_x = screenWidth - hud_width - 40.0f;
    float enemy_hud_y = 40.0f;

    RenderPetHUD(
        renderer, 
        enemy_hud_x, 
        enemy_hud_y, 
        hud_width, 
        hud_height, 
        &battle.enemy
    );

    // Status do Player (Inferior Esquerdo - Acima do Menu)
    float player_hud_x = 40.0f;
    float player_hud_y = screenHeight - 230.0f;

    RenderPetHUD(
        renderer, 
        player_hud_x, 
        player_hud_y, 
        hud_width, 
        hud_height, 
        &battle.player
    );
    bool showing_battle_menu = battle.current_state == BATTLE_STATE_MENU_ATTACK || battle.current_state == BATTLE_STATE_MENU_RUN;

    if (showing_battle_menu) {
        RenderBattleMenu(renderer, screenWidth, screenHeight, battle.current_state);
    } else {
        RenderBattleMessage(renderer, screenWidth, screenHeight, battle.current_message);
    }
}

void UpdateNavigation(BattleSystem* battle, PlayerGamePad* gamepad) {
    bool showing_battle_menu = battle->current_state == BATTLE_STATE_MENU_ATTACK || battle->current_state == BATTLE_STATE_MENU_RUN;
    if (showing_battle_menu)
    {
        if (gamepad->right && battle->current_state == BATTLE_STATE_MENU_ATTACK)
        {
            battle->current_state = BATTLE_STATE_MENU_RUN;
        }
        else if (gamepad->left && battle->current_state == BATTLE_STATE_MENU_RUN)
        {
            battle->current_state = BATTLE_STATE_MENU_ATTACK;
        }
        else if (gamepad->triggered_confirm)
        {
            switch (battle->current_state)
            {
                case BATTLE_STATE_MENU_ATTACK:
                    TriggerBattleMessage(battle, BATTLE_STATE_ATTACKING, "Atacando...", BATTLE_ATTACK_MILIS_DURATION);
                    break;
                case BATTLE_STATE_MENU_RUN:
                    TriggerBattleMessage(battle, BATTLE_STATE_RUNNING, "Fugindo...", 2000);
                    break;
                default:
                    break;
            }
        }
    }
}

int CalculateDamage(VPet* attacker, VPet* defender) {
    int damage = attacker->strength - (defender->defense / 2);
    return (damage < 1) ? 1 : damage;
}

int CalculateExp(VPet* enemy){
    return enemy->level * 2;
}

int RandomBonus(int max_value) {
    return rand() % max_value;
}

bool CheckLevelUp(VPet* vpet){
    int next_level = vpet->level +1;
    int exp_next_level = pow(next_level, 3);

    //para monitorar a evolução do vpet
    printf("prox nivel: %d\n", next_level);    
    printf("experiencia necessária: %d\n", exp_next_level);
    printf("experiencia atual: %d\n", vpet->total_exp);

    if (vpet->total_exp >= exp_next_level)
    {

        vpet->level = next_level;
        vpet->strength += vpet->level+ RandomBonus(3);
        vpet->defense += vpet->level + RandomBonus(3);
        vpet->hp_max += vpet->level + 5;
        CheckLevelUp(vpet);
        return true;
    }
    return false;
}

void UpdateBattleState(
    BattleSystem* battle, 
    GameStateMachine* state_machine,
    MapState* map
) {

    Uint64 current_time = SDL_GetTicks();

    // Se estiver em um estado de mensagem temporizada
    if (battle->current_state != BATTLE_STATE_MENU_ATTACK && battle->current_state != BATTLE_STATE_MENU_RUN) {
        
        // Verifica se a duração da mensagem expirou
        if (current_time - battle->message_start_time >= battle->message_duration) {
            switch (battle->current_state) {
                case BATTLE_STATE_ATTACKING:
                    // 1. Player ataca o Inimigo
                    battle->enemy.hp -= CalculateDamage(&battle->player, &battle->enemy);
                    if (battle->enemy.hp < 0)
                        battle->enemy.hp = 0;

                    // 2. Checa se o Inimigo foi derrotado
                    if (battle->enemy.hp == 0)
                    {
                        TriggerBattleMessage(battle, BATTLE_STATE_WIN, "INIMIGO FOI DERROTADO!", 2000);
                    }
                    else
                    {
                        // Após a animação/mensagem do ataque do jogador, passa a vez para o ataque do inimigo
                        TriggerBattleMessage(battle, BATTLE_STATE_ENEMY_ATACKING, "INIMIGO ATACA!", BATTLE_ATTACK_MILIS_DURATION);
                    }
                    break;

                case BATTLE_STATE_ENEMY_ATACKING:
                    battle->player.hp -= CalculateDamage(&battle->enemy, &battle->player);
                    if (battle->player.hp < 0)
                        battle->player.hp = 0;

                    // 2. Checa se meu VPet fui derrotado
                    if (battle->player.hp == 0)
                    {
                        TriggerBattleMessage(battle, BATTLE_STATE_LOST, "VOCE FOI DERROTADO", 2000);
                    }
                    else
                    {
                        battle->current_state = BATTLE_STATE_MENU_ATTACK;
                    }                    
                    break;
                case BATTLE_STATE_RUNNING:
                        int running_chance = rand() % 100;
                        if (running_chance < BATTLE_RUNNING_CHANCE) {
                            // Volta para o mapa de exploração
                            state_machine->state = STATE_MAP;
                            ChangeMusic(SONGS_EXPLORATION_THEME);
                        } else {
                            TriggerBattleMessage(battle, BATTLE_STATE_ENEMY_ATACKING, "Nao foi possivel fugir! Sendo atacado...", 1000);
                        }
                    break;
                case BATTLE_STATE_WIN:
                    char player_win_message[64];
                    int exp = CalculateExp(&battle->enemy);
                    char level_up_msg[25];
                    battle->player.total_exp += exp;
                    battle->player_vpetcoin_balance += battle->enemy.vpetcoin_reward;
                    if(CheckLevelUp(&battle->player)) {
                        snprintf(level_up_msg, sizeof(level_up_msg), "Subiu para LVL %d!!!", battle->player.level);
                    } else {
                        snprintf(level_up_msg, sizeof(level_up_msg), " ");
                    }                    
                    snprintf(player_win_message, sizeof(player_win_message), "Ganhou %d EXP e %.1f VPETCOINS! %s", exp, battle->enemy.vpetcoin_reward, level_up_msg);
                    TriggerBattleMessage(battle, BATTLE_STATE_WIN_CLOSE_BATTLE, player_win_message, 3000);
                    break;
                case BATTLE_STATE_WIN_CLOSE_BATTLE:
                case BATTLE_STATE_LOST: 
                    state_machine->state = STATE_MAP;
                    battle->current_state = BATTLE_STATE_MENU_ATTACK;
                    state_machine->pending_save_game = true;
                    ChangeMusic(SONGS_EXPLORATION_THEME);
                    break;
                default:
                    battle->current_state = BATTLE_STATE_MENU_ATTACK;
                    break;
            }
        }
    }
}