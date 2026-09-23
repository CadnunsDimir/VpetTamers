#include <math.h> // Necessário para usar pow()

// Converte coordenadas do Grid (X,Y) para Pixels Isométricos (ScreenX, ScreenY)
void GridToIsometric(int grid_x, int grid_y, float tile_w, float tile_h, float offset_x, float offset_y, float* out_x, float* out_y) {
    *out_x = (grid_x - grid_y) * (tile_w / 2.0f) + offset_x;
    *out_y = (grid_x + grid_y) * (tile_h / 2.0f) + offset_y;
}

void RenderVPetSprite(SDL_Renderer* renderer, VPetType tipo, float x, float y, float scale) {
    switch (tipo) {
        case PET_BK_MBARACAJAMON: {
            // 1. Cauda Curvada Volumosa (Esquerda)
            SDL_FRect tail_1 = { x - (10.0f * scale), y + (6.0f * scale), 7.0f * scale, 16.0f * scale };
            SDL_FRect tail_2 = { x - (14.0f * scale), y - (2.0f * scale), 8.0f * scale, 12.0f * scale };
            SDL_FRect tail_tip = { x - (12.0f * scale), y - (8.0f * scale), 7.0f * scale, 8.0f * scale };
            SDL_SetRenderDrawColor(renderer, 90, 95, 100, 255); // Cinza médio
            SDL_RenderFillRect(renderer, &tail_1);
            SDL_RenderFillRect(renderer, &tail_2);
            SDL_RenderFillRect(renderer, &tail_tip);

            // 2. Patas Traseiras
            SDL_FRect leg_back_l = { x - (2.0f * scale), y + (22.0f * scale), 5.0f * scale, 12.0f * scale };
            SDL_FRect leg_back_r = { x + (6.0f * scale), y + (24.0f * scale), 5.0f * scale, 10.0f * scale };
            SDL_SetRenderDrawColor(renderer, 80, 85, 90, 255);
            SDL_RenderFillRect(renderer, &leg_back_l);
            SDL_RenderFillRect(renderer, &leg_back_r);

            // 3. Área Clara da Barriga / Traseiro (Detalhe bege)
            SDL_FRect belly_light = { x + (1.0f * scale), y + (16.0f * scale), 10.0f * scale, 10.0f * scale };
            SDL_SetRenderDrawColor(renderer, 235, 215, 200, 255); // Tom bege claro
            SDL_RenderFillRect(renderer, &belly_light);

            // 4. Dorso / Corpo Principal (Inclinado em perspectiva)
            SDL_FRect body = { x + (2.0f * scale), y + (10.0f * scale), 18.0f * scale, 15.0f * scale };
            SDL_SetRenderDrawColor(renderer, 100, 105, 110, 255);
            SDL_RenderFillRect(renderer, &body);

            // 5. Patas Dianteiras (Visíveis à direita, no fundo)
            SDL_FRect leg_front_l = { x + (12.0f * scale), y + (18.0f * scale), 4.0f * scale, 10.0f * scale };
            SDL_FRect leg_front_r = { x + (17.0f * scale), y + (15.0f * scale), 4.0f * scale, 10.0f * scale };
            SDL_SetRenderDrawColor(renderer, 80, 85, 90, 255);
            SDL_RenderFillRect(renderer, &leg_front_l);
            SDL_RenderFillRect(renderer, &leg_front_r);

            // 6. Cabeça Felina (3/4 Traseiro)
            SDL_FRect head = { x + (10.0f * scale), y + (2.0f * scale), 16.0f * scale, 13.0f * scale };
            SDL_FRect cheek_l = { x + (8.0f * scale), y + (6.0f * scale), 4.0f * scale, 6.0f * scale };
            SDL_FRect cheek_r = { x + (24.0f * scale), y + (6.0f * scale), 4.0f * scale, 6.0f * scale };
            SDL_SetRenderDrawColor(renderer, 100, 105, 110, 255);
            SDL_RenderFillRect(renderer, &head);
            SDL_RenderFillRect(renderer, &cheek_l);
            SDL_RenderFillRect(renderer, &cheek_r);

            // 7. Orelhas Pontudas
            SDL_FRect ear_l = { x + (10.0f * scale), y - (5.0f * scale), 5.0f * scale, 8.0f * scale };
            SDL_FRect ear_r = { x + (20.0f * scale), y - (4.0f * scale), 5.0f * scale, 8.0f * scale };
            SDL_SetRenderDrawColor(renderer, 80, 85, 90, 255);
            SDL_RenderFillRect(renderer, &ear_l);
            SDL_RenderFillRect(renderer, &ear_r);

            break;
        }

        case PET_JAGUAPIRUMON: {
            // 1. AURA DE CHAMAS EXTERNA (Efeito de brilho ao redor do corpo)
            SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
            
            // Chama Laranja Externa (Transparente)
            SDL_FRect flame_outer = { x - (8.0f * scale), y - (6.0f * scale), 48.0f * scale, 42.0f * scale };
            SDL_SetRenderDrawColor(renderer, 255, 100, 0, 120);
            SDL_RenderFillRect(renderer, &flame_outer);

            // Chama Amarela Intermediária (Efeito de calor pulsante)
            float time_offset = sinf((float)SDL_GetTicks() / 80.0f) * (2.0f * scale); // Animação de chama tremulando
            SDL_FRect flame_inner = { x - (4.0f * scale), y - (2.0f * scale) + time_offset, 40.0f * scale, 34.0f * scale };
            SDL_SetRenderDrawColor(renderer, 255, 200, 0, 160);
            SDL_RenderFillRect(renderer, &flame_inner);

            // 2. PATAS (Posicionadas para criar perspectiva isométrica)
            SDL_SetRenderDrawColor(renderer, 40, 20, 10, 255); // Pelo carbonizado escuro
            
            // Pata Traseira Esquerda
            SDL_FRect leg_back_l = { x + (22.0f * scale), y + (22.0f * scale), 6.0f * scale, 12.0f * scale };
            SDL_RenderFillRect(renderer, &leg_back_l);
            
            // Pata Traseira Direita
            SDL_FRect leg_back_r = { x + (16.0f * scale), y + (24.0f * scale), 6.0f * scale, 12.0f * scale };
            SDL_RenderFillRect(renderer, &leg_back_r);

            // Pata Dianteira Esquerda
            SDL_FRect leg_front_l = { x + (4.0f * scale), y + (22.0f * scale), 7.0f * scale, 14.0f * scale };
            SDL_RenderFillRect(renderer, &leg_front_l);

            // Pata Dianteira Direita (Avançada para frente e com garras)
            SDL_FRect leg_front_r = { x - (2.0f * scale), y + (24.0f * scale), 8.0f * scale, 14.0f * scale };
            SDL_SetRenderDrawColor(renderer, 50, 25, 12, 255);
            SDL_RenderFillRect(renderer, &leg_front_r);

            // Garras Pretas na pata dianteira
            SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
            SDL_FRect claws = { x - (3.0f * scale), y + (36.0f * scale), 9.0f * scale, 3.0f * scale };
            SDL_RenderFillRect(renderer, &claws);

            // 3. CORPO / TRONCO (Inclinado para a postura canina)
            SDL_SetRenderDrawColor(renderer, 45, 22, 12, 255); // Castanho escuro / queimado
            SDL_FRect body = { x + (2.0f * scale), y + (8.0f * scale), 26.0f * scale, 18.0f * scale };
            SDL_RenderFillRect(renderer, &body);

            // Lombo/Costas (Sombra superior)
            SDL_SetRenderDrawColor(renderer, 25, 12, 6, 255);
            SDL_FRect spine = { x + (6.0f * scale), y + (6.0f * scale), 20.0f * scale, 6.0f * scale };
            SDL_RenderFillRect(renderer, &spine);

            // 4. CORRENTE METÁLICA NAS COSTAS
            SDL_SetRenderDrawColor(renderer, 180, 180, 190, 255); // Prateado/Cinza metálico
            SDL_FRect chain_link1 = { x + (12.0f * scale), y + (4.0f * scale), 6.0f * scale, 5.0f * scale };
            SDL_FRect chain_link2 = { x + (16.0f * scale), y + (1.0f * scale), 5.0f * scale, 5.0f * scale };
            SDL_RenderFillRect(renderer, &chain_link1);
            SDL_RenderFillRect(renderer, &chain_link2);

            // 5. CABEÇA E FOCINHO (Inclinados para a esquerda/frente na batalha)
            SDL_SetRenderDrawColor(renderer, 35, 18, 8, 255);
            SDL_FRect head = { x - (4.0f * scale), y + (6.0f * scale), 16.0f * scale, 14.0f * scale };
            SDL_RenderFillRect(renderer, &head);

            // Focinho Alongado
            SDL_FRect snout = { x - (10.0f * scale), y + (12.0f * scale), 12.0f * scale, 10.0f * scale };
            SDL_RenderFillRect(renderer, &snout);

            // Orelhas / Chifres Pontiagudos
            SDL_SetRenderDrawColor(renderer, 20, 10, 5, 255);
            SDL_FRect ear1 = { x + (4.0f * scale), y - (2.0f * scale), 4.0f * scale, 10.0f * scale };
            SDL_FRect ear2 = { x + (9.0f * scale), y + (1.0f * scale), 4.0f * scale, 8.0f * scale };
            SDL_RenderFillRect(renderer, &ear1);
            SDL_RenderFillRect(renderer, &ear2);

            // 6. DETALHES RANCOROSOS (Dentes, Olhos e Fogo na Boca)
            // Dentes Expôstos (Branco Amarelado)
            SDL_SetRenderDrawColor(renderer, 240, 230, 180, 255);
            SDL_FRect teeth = { x - (10.0f * scale), y + (20.0f * scale), 10.0f * scale, 3.0f * scale };
            SDL_RenderFillRect(renderer, &teeth);

            // Olhos Incandescentes (Amarelo/Branco Flamejante)
            SDL_SetRenderDrawColor(renderer, 255, 255, 100, 255);
            SDL_FRect eye = { x - (1.0f * scale), y + (10.0f * scale), 4.0f * scale, 3.0f * scale };
            SDL_RenderFillRect(renderer, &eye);

            // Núcleo do Olho
            SDL_SetRenderDrawColor(renderer, 255, 50, 0, 255);
            SDL_FRect eye_core = { x - (1.0f * scale), y + (11.0f * scale), 2.0f * scale, 2.0f * scale };
            SDL_RenderFillRect(renderer, &eye_core);

            // Fogo saindo da Mandíbula (Detalhe final)
            SDL_SetRenderDrawColor(renderer, 255, 150, 0, 200);
            SDL_FRect jaw_fire = { x - (12.0f * scale), y + (18.0f * scale), 6.0f * scale, 6.0f * scale };
            SDL_RenderFillRect(renderer, &jaw_fire);

            break;
        }

        case PET_RUGUASUMON: {
            // 1. Cauda Verde Arqueada (À direita, no fundo)
            SDL_FRect tail_main = { x + (18.0f * scale), y - (2.0f * scale), 12.0f * scale, 20.0f * scale };
            SDL_FRect tail_top  = { x + (14.0f * scale), y - (8.0f * scale), 14.0f * scale, 10.0f * scale };
            SDL_SetRenderDrawColor(renderer, 20, 80, 65, 255); // Verde-escuro / Esmeralda
            SDL_RenderFillRect(renderer, &tail_main);
            SDL_RenderFillRect(renderer, &tail_top);

            // 2. Patas Amarelas (Alinhadas em baixo)
            SDL_FRect leg_l = { x + (6.0f * scale), y + (28.0f * scale), 3.0f * scale, 8.0f * scale };
            SDL_FRect leg_r = { x + (14.0f * scale), y + (28.0f * scale), 3.0f * scale, 8.0f * scale };
            SDL_SetRenderDrawColor(renderer, 230, 190, 40, 255); // Amarelo
            SDL_RenderFillRect(renderer, &leg_l);
            SDL_RenderFillRect(renderer, &leg_r);

            // 3. Peito / Barriga (Verde-escuro)
            SDL_FRect breast = { x + (2.0f * scale), y + (18.0f * scale), 14.0f * scale, 12.0f * scale };
            SDL_SetRenderDrawColor(renderer, 20, 80, 65, 255);
            SDL_RenderFillRect(renderer, &breast);

            // 4. Asa / Asas (Acastanhadas/Avermelhadas no dorso)
            SDL_FRect wing = { x + (8.0f * scale), y + (14.0f * scale), 14.0f * scale, 12.0f * scale };
            SDL_SetRenderDrawColor(renderer, 180, 65, 25, 255); // Castanho-avermelhado
            SDL_RenderFillRect(renderer, &wing);

            // 5. Pescoço e Cabeça (Amarelo vivo, voltado para a esquerda/frente)
            SDL_FRect neck = { x + (2.0f * scale), y + (6.0f * scale), 10.0f * scale, 14.0f * scale };
            SDL_FRect head = { x - (1.0f * scale), y + (2.0f * scale), 11.0f * scale, 8.0f * scale };
            SDL_SetRenderDrawColor(renderer, 240, 180, 30, 255); // Amarelo
            SDL_RenderFillRect(renderer, &neck);
            SDL_RenderFillRect(renderer, &head);

            // 6. Crista Vermelha no Topo da Cabeça
            SDL_FRect comb_1 = { x + (1.0f * scale), y - (4.0f * scale), 4.0f * scale, 7.0f * scale };
            SDL_FRect comb_2 = { x + (5.0f * scale), y - (5.0f * scale), 4.0f * scale, 8.0f * scale };
            SDL_SetRenderDrawColor(renderer, 220, 35, 35, 255); // Vermelho
            SDL_RenderFillRect(renderer, &comb_1);
            SDL_RenderFillRect(renderer, &comb_2);

            // 7. Barbela (Pedaço vermelho sob o bico)
            SDL_FRect wattle = { x - (2.0f * scale), y + (10.0f * scale), 4.0f * scale, 6.0f * scale };
            SDL_SetRenderDrawColor(renderer, 220, 35, 35, 255);
            SDL_RenderFillRect(renderer, &wattle);

            // 8. Bico Amarelo (Apontado para a esquerda/frente, na direção do gato)
            SDL_FRect beak = { x - (6.0f * scale), y + (4.0f * scale), 6.0f * scale, 4.0f * scale };
            SDL_SetRenderDrawColor(renderer, 245, 190, 20, 255);
            SDL_RenderFillRect(renderer, &beak);

            // 9. Olho Preto
            SDL_FRect eye = { x + (2.0f * scale), y + (4.0f * scale), 2.0f * scale, 2.0f * scale };
            SDL_SetRenderDrawColor(renderer, 10, 10, 10, 255);
            SDL_RenderFillRect(renderer, &eye);

            break;
        }
    }
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
    RenderText(renderer, message, COLOR_WHITE, 50.0f, screenHeight - 95.0f, 3.0f);
}

void TriggerBattleMessage(BattleSystem* battle, BattleState new_state, const char* text, Uint64 duration_ms) {
    battle->current_state = new_state;
    battle->message_start_time = SDL_GetTicks();
    battle->message_duration = duration_ms;
    snprintf(battle->current_message, sizeof(battle->current_message), "%s", text);
}

void RenderBattleScreen(SDL_Renderer* renderer, float screenWidth, float screenHeight, BattleSystem battle) {
    OponentsPositioning oponentsPositioning = { 0, 0, 0, 0 };
    
    RenderBattleScreenScenario(renderer, screenWidth, screenHeight, &oponentsPositioning);

    // --- Representação Visual das Entidades ---    

    if (battle.current_state != BATTLE_STATE_WIN && battle.current_state != BATTLE_STATE_WIN_CLOSE_BATTLE)
    {
        // 1. Inimigo (Renderizado PRIMEIRO para ficar ao fundo)
       RenderVPetSprite(
            renderer, 
            battle.enemy.type,
            oponentsPositioning.enemy.x - (16.0f * 2.5f), // Centraliza horizontalmente o sprite de 32px
            oponentsPositioning.enemy.y - (24.0f * 2.5f), // Apoia a base do sprite no centro do tile
            2.5f
        );
    }
    
    if (battle.current_state != BATTLE_STATE_LOST) {
       // 2. BkMbaracajamon (Renderizado DEPOIS para se sobrepor caso cruzem a linha de visão)
        RenderVPetSprite(
            renderer, 
            battle.player.type,
            oponentsPositioning.player.x - (15.0f * 2.5f), 
            oponentsPositioning.player.y - (25.0f * 2.5f), 
            2.5f
        );
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
                    TriggerBattleMessage(battle, BATTLE_STATE_ATTACKING, "Atacando...", 2000);
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

bool CheckLevelUp(VPet* vpet){
    int next_level = vpet->level +1;
    printf("prox nivel: %d\n", next_level);
    int exp_next_level = pow(next_level, 2);
    printf("experiencia necessária: %d\n", exp_next_level);
    printf("experiencia atual: %d\n", vpet->total_exp);
    if (vpet->total_exp >= exp_next_level)
    {
        vpet->level = next_level;
        vpet->strength += vpet->level*2;
        vpet->defense += vpet->level*2;
        vpet->hp_max += vpet->level*5;
        vpet->hp = vpet->hp_max;
        CheckLevelUp(vpet);
        return true;
    }
    return false;
}

void UpdateBattleState(
    BattleSystem* battle, 
    GameState* game_state, 
    const char* save_data_file, 
    Coordinates player_pos, 
    int current_map_id) {

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
                        TriggerBattleMessage(battle, BATTLE_STATE_ENEMY_ATACKING, "INIMIGO ATACA!", 2000);
                    }
                    break;

                case BATTLE_STATE_ENEMY_ATACKING:
                    battle->player.hp -= CalculateDamage(&battle->enemy, &battle->player);
                    if (battle->player.hp < 0)
                        battle->player.hp = 0;

                    // 2. Checa se meu VPet fui derrotado
                    if (battle->player.hp == 0)
                    {
                        TriggerBattleMessage(battle, BATTLE_STATE_LOST, "VOCÊ FOI DERROTADO", 2000);
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
                            *game_state = STATE_MAP;
                        } else {
                            TriggerBattleMessage(battle, BATTLE_STATE_ENEMY_ATACKING, "Nao foi possivel fugir! Sendo atacado...", 1000);
                        }
                    break;
                case BATTLE_STATE_WIN:
                    char player_win_message[64];
                    int exp = CalculateExp(&battle->enemy);
                    char level_up_msg[25];
                    battle->player.total_exp += exp;
                    if(CheckLevelUp(&battle->player)) {
                        snprintf(level_up_msg, sizeof(level_up_msg), "Subiu para LVL %d!!!", battle->player.level);
                    } else {
                        snprintf(level_up_msg, sizeof(level_up_msg), " ");
                    }                    
                    snprintf(player_win_message, sizeof(player_win_message), "Ganhou %d EXP! %s", exp, level_up_msg);                    
                    TriggerBattleMessage(battle, BATTLE_STATE_WIN_CLOSE_BATTLE, player_win_message, 3000);
                    break;
                case BATTLE_STATE_WIN_CLOSE_BATTLE:
                case BATTLE_STATE_LOST: 
                    *game_state = STATE_MAP;
                    battle->current_state = BATTLE_STATE_MENU_ATTACK;
                    SaveGame(
                        save_data_file,
                        player_pos,
                        current_map_id,
                        battle->player);
                    break;
                default:
                    battle->current_state = BATTLE_STATE_MENU_ATTACK;
                    break;
            }
        }
    }
}