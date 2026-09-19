// Converte coordenadas do Grid (X,Y) para Pixels Isométricos (ScreenX, ScreenY)
void GridToIsometric(int grid_x, int grid_y, float tile_w, float tile_h, float offset_x, float offset_y, float* out_x, float* out_y) {
    *out_x = (grid_x - grid_y) * (tile_w / 2.0f) + offset_x;
    *out_y = (grid_x + grid_y) * (tile_h / 2.0f) + offset_y;
}

void RenderVPetSprite(SDL_Renderer* renderer, VPetType tipo, float x, float y, float scale) {
    switch (tipo) {
        case PET_BK_MBARACAJAMON: {
            // Corpo Principal (Felino/Sombrio - Azul escuro/Cinzento)
            SDL_FRect body = { x, y + (10.0f * scale), 30.0f * scale, 25.0f * scale };
            SDL_SetRenderDrawColor(renderer, 35, 45, 60, 255);
            SDL_RenderFillRect(renderer, &body);

            // Orelhas de Gato
            SDL_FRect ear_l = { x + (2.0f * scale), y, 6.0f * scale, 10.0f * scale };
            SDL_FRect ear_r = { x + (22.0f * scale), y, 6.0f * scale, 10.0f * scale };
            SDL_SetRenderDrawColor(renderer, 20, 25, 35, 255);
            SDL_RenderFillRect(renderer, &ear_l);
            SDL_RenderFillRect(renderer, &ear_r);

            // Olhos Brilhantes (Amarelo/Verde)
            SDL_FRect eye_l = { x + (6.0f * scale), y + (14.0f * scale), 4.0f * scale, 4.0f * scale };
            SDL_FRect eye_r = { x + (20.0f * scale), y + (14.0f * scale), 4.0f * scale, 4.0f * scale };
            SDL_SetRenderDrawColor(renderer, 230, 220, 50, 255);
            SDL_RenderFillRect(renderer, &eye_l);
            SDL_RenderFillRect(renderer, &eye_r);
            break;
        }

        case PET_JAGUAPIRUMON: {
            // Corpo (Cão/Canídeo - Marrom avermelhado)
            SDL_FRect body = { x, y + (8.0f * scale), 32.0f * scale, 24.0f * scale };
            SDL_SetRenderDrawColor(renderer, 120, 60, 30, 255);
            SDL_RenderFillRect(renderer, &body);

            // Focinho
            SDL_FRect snout = { x - (4.0f * scale), y + (16.0f * scale), 10.0f * scale, 10.0f * scale };
            SDL_SetRenderDrawColor(renderer, 80, 40, 20, 255);
            SDL_RenderFillRect(renderer, &snout);

            // Olhos Vermelhos
            SDL_FRect eye = { x + (6.0f * scale), y + (12.0f * scale), 4.0f * scale, 4.0f * scale };
            SDL_SetRenderDrawColor(renderer, 220, 40, 40, 255);
            SDL_RenderFillRect(renderer, &eye);
            break;
        }

        case PET_RUGUASUMON: {
            // Corpo (Ave/Galinha - Marrom/Amarelado)
            SDL_FRect body = { x + (4.0f * scale), y + (10.0f * scale), 24.0f * scale, 22.0f * scale };
            SDL_SetRenderDrawColor(renderer, 180, 140, 60, 255);
            SDL_RenderFillRect(renderer, &body);

            // Crista (Vermelha no topo)
            SDL_FRect comb = { x + (12.0f * scale), y + (2.0f * scale), 8.0f * scale, 8.0f * scale };
            SDL_SetRenderDrawColor(renderer, 220, 30, 30, 255);
            SDL_RenderFillRect(renderer, &comb);

            // Bico (Laranja)
            SDL_FRect beak = { x + (26.0f * scale), y + (16.0f * scale), 8.0f * scale, 6.0f * scale };
            SDL_SetRenderDrawColor(renderer, 240, 150, 20, 255);
            SDL_RenderFillRect(renderer, &beak);
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

void RenderPetHUD(SDL_Renderer* renderer, float x, float y, float width, float height, const char* name, int level, int hp, int max_hp) {
    // 1. Fundo do Painel (Azul escuro semi-transparente)
    SDL_FRect hud_rect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 20, 25, 40, 230);
    SDL_RenderFillRect(renderer, &hud_rect);

    // 2. Borda do Painel (Cinza claro)
    SDL_SetRenderDrawColor(renderer, 180, 180, 200, 255);
    SDL_RenderRect(renderer, &hud_rect);

    // 3. Texto do Nome e Nível (se o nível for > 0)
    char buffer[64];
    if (level > 0) {
        snprintf(buffer, sizeof(buffer), "%s (LVL %d)", name, level);
    } else {
        snprintf(buffer, sizeof(buffer), "%s", name);
    }
    RenderText(renderer, buffer, COLOR_WHITE, x + 15.0f, y + 12.0f, 2.5f);

    // 4. Texto do HP Numérico
    snprintf(buffer, sizeof(buffer), "HP: %d/%d", hp, max_hp);
    RenderText(renderer, buffer, COLOR_WHITE, x + 15.0f, y + 42.0f, 2.5f);
}

void RenderBattleMenu(SDL_Renderer* renderer, float screenWidth, float screenHeight, int menu_opcao_batalha) {
    // --- Caixa de Comandos de Batalha (Rodapé) ---
    SDL_FRect menu_box = { 20.0f, screenHeight - 140.0f, screenWidth - 40.0f, 120.0f };
    SDL_SetRenderDrawColor(renderer, 15, 20, 35, 245);
    SDL_RenderFillRect(renderer, &menu_box);
    SDL_SetRenderDrawColor(renderer, 200, 200, 220, 255);
    SDL_RenderRect(renderer, &menu_box);

    // Opções do Turno
    Color col_atacar = (menu_opcao_batalha == 0) ? COLOR_MENU_BUTTON_SELECTED : COLOR_WHITE;
    Color col_fugir  = (menu_opcao_batalha == 1) ? COLOR_MENU_BUTTON_SELECTED : COLOR_WHITE;

    RenderText(renderer, "ATACAR", col_atacar, 60.0f, screenHeight - 100.0f, 3.5f);
    RenderText(renderer, "FUGIR",  col_fugir,  320.0f, screenHeight - 100.0f, 3.5f);
}

void RenderBattleScreen(SDL_Renderer* renderer, float screenWidth, float screenHeight, const VPet* player_pet, const VPet* enemy_pet, int menu_opcao_batalha) {
    OponentsPositioning oponentsPositioning = { 0, 0, 0, 0 };
    
    RenderBattleScreenScenario(renderer, screenWidth, screenHeight, &oponentsPositioning);

    // --- Representação Visual das Entidades ---
    // --- Representação Visual das Entidades ---

    // 1. Inimigo (Renderizado PRIMEIRO para ficar ao fundo)
    RenderVPetSprite(
        renderer, 
        enemy_pet->type, 
        oponentsPositioning.enemy.x - (16.0f * 2.5f), // Centraliza horizontalmente o sprite de 32px
        oponentsPositioning.enemy.y - (24.0f * 2.5f), // Apoia a base do sprite no centro do tile
        2.5f
    );

    // 2. BkMbaracajamon (Renderizado DEPOIS para se sobrepor caso cruzem a linha de visão)
    RenderVPetSprite(
        renderer, 
        player_pet->type, 
        oponentsPositioning.player.x - (15.0f * 2.5f), 
        oponentsPositioning.player.y - (25.0f * 2.5f), 
        2.5f
    );

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
        enemy_pet->name, 
        enemy_pet->level, 
        enemy_pet->hp, 
        enemy_pet->hp_max
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
        player_pet->name, 
        player_pet->level, 
        player_pet->hp, 
        player_pet->hp_max
    );

    RenderBattleMenu(renderer, screenWidth, screenHeight, menu_opcao_batalha);
}