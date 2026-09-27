#include <math.h>

// Desenha caracteres simples (A-Z, 0-9) usando retângulos de pixels na SDL3
void RenderText(SDL_Renderer* renderer, const char* texto, const Color color, float startX, float startY, float pixelSize) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    for (int i = 0; texto[i] != '\0'; i++) {
        char c = texto[i];
        float offsetX = startX + (i * 6 * pixelSize); // Espaçamento entre letras

        // Padrão de bits 5x5 para cada caractere (1 = pixel desenhado, 0 = vazio)
        unsigned char glyph[5] = {0};

        switch (c) {
            case 'N': case 'n':
                glyph[0] = 0b10001; glyph[1] = 0b11001; glyph[2] = 0b10101; glyph[3] = 0b10011; glyph[4] = 0b10001; break;
            case 'O': case 'o':
                glyph[0] = 0b01110; glyph[1] = 0b10001; glyph[2] = 0b10001; glyph[3] = 0b10001; glyph[4] = 0b01110; break;
            case 'V': case 'v':
                glyph[0] = 0b10001; glyph[1] = 0b10001; glyph[2] = 0b10001; glyph[3] = 0b01010; glyph[4] = 0b00100; break;
            case 'J': case 'j':
                glyph[0] = 0b00111; glyph[1] = 0b00010; glyph[2] = 0b00010; glyph[3] = 0b10010; glyph[4] = 0b01100; break;
            case 'G': case 'g':
                glyph[0] = 0b01111; glyph[1] = 0b10000; glyph[2] = 0b10011; glyph[3] = 0b10001; glyph[4] = 0b01110; break;
            case 'C': case 'c':
                glyph[0] = 0b01110; glyph[1] = 0b10000; glyph[2] = 0b10000; glyph[3] = 0b10000; glyph[4] = 0b01110; break;
            case 'T': case 't':
                glyph[0] = 0b11111; glyph[1] = 0b00100; glyph[2] = 0b00100; glyph[3] = 0b00100; glyph[4] = 0b00100; break;
            case 'I': case 'i':
                glyph[0] = 0b01110; glyph[1] = 0b00100; glyph[2] = 0b00100; glyph[3] = 0b00100; glyph[4] = 0b01110; break;
            case 'A': case 'a':
                glyph[0] = 0b01110; glyph[1] = 0b10001; glyph[2] = 0b11111; glyph[3] = 0b10001; glyph[4] = 0b10001; break;
            case 'R': case 'r':
                glyph[0] = 0b11110; glyph[1] = 0b10001; glyph[2] = 0b11110; glyph[3] = 0b10010; glyph[4] = 0b10001; break;
            case 'U': case 'u':
                glyph[0] = 0b10001; glyph[1] = 0b10001; glyph[2] = 0b10001; glyph[3] = 0b10001; glyph[4] = 0b01110; break;
            case 'P': case 'p':
                glyph[0] = 0b11110; glyph[1] = 0b10001; glyph[2] = 0b11110; glyph[3] = 0b10000; glyph[4] = 0b10000; break;
            case 'S': case 's':
                glyph[0] = 0b01111; glyph[1] = 0b10000; glyph[2] = 0b01110; glyph[3] = 0b00001; glyph[4] = 0b11110; break;
            case 'E': case 'e':
                glyph[0] = 0b11111; glyph[1] = 0b10000; glyph[2] = 0b11110; glyph[3] = 0b10000; glyph[4] = 0b11111; break;
            case 'L': case 'l':
                glyph[0] = 0b10000; glyph[1] = 0b10000; glyph[2] = 0b10000; glyph[3] = 0b10000; glyph[4] = 0b11111; break;
            case 'H': case 'h':
                glyph[0] = 0b10001; glyph[1] = 0b10001; glyph[2] = 0b11111; glyph[3] = 0b10001; glyph[4] = 0b10001; break;
            case 'F': case 'f':
                glyph[0] = 0b11111; glyph[1] = 0b10000; glyph[2] = 0b11110; glyph[3] = 0b10000; glyph[4] = 0b10000; break;
            case 'K': case 'k':
                glyph[0] = 0b10001; glyph[1] = 0b10010; glyph[2] = 0b11100; glyph[3] = 0b10010; glyph[4] = 0b10001; break;
            case 'D': case 'd':
                glyph[0] = 0b11100; glyph[1] = 0b10010; glyph[2] = 0b10010; glyph[3] = 0b10010; glyph[4] = 0b11100; break;
            case 'B': case 'b':
                glyph[0] = 0b11110; glyph[1] = 0b10001; glyph[2] = 0b11110; glyph[3] = 0b10001; glyph[4] = 0b11110; break;
            case 'M': case 'm':
                glyph[0] = 0b10001; glyph[1] = 0b11011; glyph[2] = 0b10101; glyph[3] = 0b10001; glyph[4] = 0b10001; break;
            case 'X': case 'x':
                glyph[0] = 0b10001; glyph[1] = 0b01010; glyph[2] = 0b00100; glyph[3] = 0b01010; glyph[4] = 0b10001; break;
            case ':':
                glyph[0] = 0b00000; glyph[1] = 0b01100; glyph[2] = 0b00000; glyph[3] = 0b01100; glyph[4] = 0b00000; break;
            case '/':
                glyph[0] = 0b00001; glyph[1] = 0b00010; glyph[2] = 0b00100; glyph[3] = 0b01000; glyph[4] = 0b10000; break;
            case '(':
                glyph[0] = 0b00010; glyph[1] = 0b00100; glyph[2] = 0b00100; glyph[3] = 0b00100; glyph[4] = 0b00010; break;
            case ')':
                glyph[0] = 0b01000; glyph[1] = 0b00100; glyph[2] = 0b00100; glyph[3] = 0b00100; glyph[4] = 0b01000; break;
            case '.':
                glyph[0] = 0b00000; glyph[1] = 0b00000; glyph[2] = 0b00000; glyph[3] = 0b00000; glyph[4] = 0b00100; break;
            case '!':
                glyph[0] = 0b00100; glyph[1] = 0b00100; glyph[2] = 0b00100; glyph[3] = 0b00000; glyph[4] = 0b00100; break;
            case '?':
                glyph[0] = 0b01110; glyph[1] = 0b00001; glyph[2] = 0b00110; glyph[3] = 0b00000; glyph[4] = 0b00100; break;
            case '0':
                glyph[0] = 0b01110; glyph[1] = 0b10001; glyph[2] = 0b10001; glyph[3] = 0b10001; glyph[4] = 0b01110; break;
            case '1':
                glyph[0] = 0b00100; glyph[1] = 0b01100; glyph[2] = 0b00100; glyph[3] = 0b00100; glyph[4] = 0b01110; break;
            case '2':
                glyph[0] = 0b01110; glyph[1] = 0b00001; glyph[2] = 0b01110; glyph[3] = 0b10000; glyph[4] = 0b11111; break;
            case '3':
                glyph[0] = 0b11110; glyph[1] = 0b00001; glyph[2] = 0b00110; glyph[3] = 0b00001; glyph[4] = 0b11110; break;
            case '4':
                glyph[0] = 0b10001; glyph[1] = 0b10001; glyph[2] = 0b11111; glyph[3] = 0b00001; glyph[4] = 0b00001; break;
            case '5':
                glyph[0] = 0b11111; glyph[1] = 0b10000; glyph[2] = 0b11110; glyph[3] = 0b00001; glyph[4] = 0b11110; break;
            case '6':
                glyph[0] = 0b01110; glyph[1] = 0b10000; glyph[2] = 0b11110; glyph[3] = 0b10001; glyph[4] = 0b01110; break;
            case '7':
                glyph[0] = 0b11111; glyph[1] = 0b00001; glyph[2] = 0b00010; glyph[3] = 0b00100; glyph[4] = 0b00100; break;
            case '8':
                glyph[0] = 0b01110; glyph[1] = 0b10001; glyph[2] = 0b01110; glyph[3] = 0b10001; glyph[4] = 0b01110; break;
            case '9':
                glyph[0] = 0b01110; glyph[1] = 0b10001; glyph[2] = 0b01111; glyph[3] = 0b00001; glyph[4] = 0b01110; break;
            case ' ':
                continue; // Espaço em branco
            default: // Caractere desconhecido vira um bloco sólido
                printf("%c\n", c);
                glyph[0] = 0b11111; glyph[1] = 0b11111; glyph[2] = 0b11111; glyph[3] = 0b11111; glyph[4] = 0b11111; break;
        }

        // Lê cada bit da matriz e desenha retângulos na tela
        for (int row = 0; row < 5; row++) {
            for (int col = 0; col < 5; col++) {
                // Checa se o bit na coluna (4 - col) está ligado (1)
                if ((glyph[row] >> (4 - col)) & 1) {
                    SDL_FRect pixel = {
                        offsetX + (col * pixelSize),
                        startY + (row * pixelSize),
                        pixelSize,
                        pixelSize
                    };
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }
    }
}

SDL_FRect PlayerToRect(Coordinates player, float playerSize, Camera cam) {
    SDL_FRect rect = {
        (player.x * playerSize) - cam.x, // Posição X no Mundo - Câmera X
        (player.y * playerSize) - cam.y, // Posição Y no Mundo - Câmera Y
        playerSize,
        playerSize
    };

    return rect;
}

void RenderPlayer(SDL_Renderer* renderer, Coordinates player_coordinates, Camera cam, float size, bool is_moving) {
    // Posição base em pixels do player na tela
    SDL_FRect playerRect = PlayerToRect(player_coordinates, size, cam);

    // 1. Configurações de proporção do boneco
    float head_size = size * 0.35f;
    float body_w    = size * 0.35f;
    float body_h    = size * 0.40f;
    float limb_w    = size * 0.12f;
    float limb_h    = size * 0.30f;

    // Center X do player
    float center_x = playerRect.x + (size / 2.0f);

    // 2. Cálculo do deslocamento das pernas/braços ao andar
    float limb_offset = 0.0f;
    if (is_moving) {
        // Alterna entre -8px e +8px criando movimento uniforme de caminhada
        float time = (float)SDL_GetTicks() / 100.0f; 
        limb_offset = sinf(time) * (size * 0.15f);
    }

    // --- A. PERNAS (Atrás do tronco) ---
    SDL_SetRenderDrawColor(renderer, 30, 30, 120, 255); // Azul escuro (calça)
    
    // Perna Esquerda (Mexe para um lado)
    SDL_FRect left_leg = {
        center_x - (body_w / 2.0f),
        playerRect.y + head_size + body_h - (limb_h * 0.2f) + limb_offset,
        limb_w,
        limb_h
    };
    SDL_RenderFillRect(renderer, &left_leg);

    // Perna Direita (Mexe para o lado oposto)
    SDL_FRect right_leg = {
        center_x + (body_w / 2.0f) - limb_w,
        playerRect.y + head_size + body_h - (limb_h * 0.2f) - limb_offset,
        limb_w,
        limb_h
    };
    SDL_RenderFillRect(renderer, &right_leg);

    // --- B. TRONCO / CORPO ---
    SDL_SetRenderDrawColor(renderer, 220, 50, 50, 255); // Vermelho (Camisa)
    SDL_FRect body = {
        center_x - (body_w / 2.0f),
        playerRect.y + head_size,
        body_w,
        body_h
    };
    SDL_RenderFillRect(renderer, &body);

    // --- C. BRAÇOS ---
    SDL_SetRenderDrawColor(renderer, 240, 190, 150, 255); // Tom de pele
    
    // Braço Esquerdo (Mexe invertido com a perna esquerda)
    SDL_FRect left_arm = {
        center_x - (body_w / 2.0f) - limb_w,
        playerRect.y + head_size - limb_offset,
        limb_w,
        limb_h
    };
    SDL_RenderFillRect(renderer, &left_arm);

    // Braço Direito
    SDL_FRect right_arm = {
        center_x + (body_w / 2.0f),
        playerRect.y + head_size + limb_offset,
        limb_w,
        limb_h
    };
    SDL_RenderFillRect(renderer, &right_arm);

    // --- D. CABEÇA ---
    SDL_FRect head = {
        center_x - (head_size / 2.0f),
        playerRect.y,
        head_size,
        head_size
    };
    SDL_RenderFillRect(renderer, &head);
}

void DrawRect(SDL_Renderer* renderer, SDL_FRect tile, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &tile);
}

static Color ShadeVpetColor(Color color, float factor, int alpha) {
    Color shaded = {
        (int)(color.r * factor),
        (int)(color.g * factor),
        (int)(color.b * factor),
        alpha
    };

    return shaded;
}

static void RenderVpetPartVolume(SDL_Renderer* renderer, SDL_FRect rect, Color color, float scale) {
    float depth = fmaxf(1.0f, scale * 1.5f);
    SDL_FRect side = { rect.x + depth, rect.y + depth, rect.w, rect.h };
    SDL_FRect top = { rect.x, rect.y, rect.w, fmaxf(1.0f, scale * 1.5f) };
    SDL_FRect left = { rect.x, rect.y, fmaxf(1.0f, scale * 1.5f), rect.h };

    // Extrusao escura e duas faces de luz para sugerir volume na vista iso.
    DrawRect(renderer, side, ShadeVpetColor(color, 0.45f, color.a));
    DrawRect(renderer, rect, color);
    DrawRect(renderer, top, ShadeVpetColor(color, 1.20f, color.a));
    DrawRect(renderer, left, ShadeVpetColor(color, 1.08f, color.a));
}

void RenderMapVpetData(SDL_Renderer* renderer, const VpetPart* parts, size_t count, float pos_x, float pos_y, float leg_step_A, float leg_step_B, float scale) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (size_t i = 0; i < count; i++) {
        const VpetPart* part = &parts[i];

        // Aplica o deslocamento da animação com base no grupo da perna
        float x_offset = 0.0f;
        if (part->anim_group == ANIM_STEP_A) {
            x_offset = leg_step_A;
        } else if (part->anim_group == ANIM_STEP_B) {
            x_offset = leg_step_B;
        }

        SDL_FRect dst_rect = {
            pos_x + ((part->rect.x + x_offset) * scale),
            pos_y + (part->rect.y * scale),
            part->rect.w * scale,
            part->rect.h * scale
        };

        RenderVpetPartVolume(renderer, dst_rect, part->color, scale);
    }
}

void RenderVPet(SDL_Renderer* renderer, VPetType vpet_code, int x, int y, int title_size) {
    // 1. Animação suave para as pernas (ritmo de caminhada)
    float time = (float)SDL_GetTicks() / 100.0f;
    
    // Movimento para frente/trás das patas
    float leg_step_A = sinf(time) * 2.0f;  // Pata dianteira esquerda e traseira direita
    float leg_step_B = -sinf(time) * 2.0f; // Pata dianteira direita e traseira esquerda

    float scale = (float)title_size / 32.0f; 
    
    // Posição base FIXA no tile (o corpo não pula)
    float pos_x = (float)x;
    float pos_y = (float)y;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    for (int i = 0; i < ENEMY_PART_LIST_COUNT; i++)
    {
        if(ENEMY_PART_LIST[i].type == vpet_code) {
            RenderMapVpetData(renderer, ENEMY_PART_LIST[i].vparts,  ENEMY_PART_LIST[i].vparts_count, pos_x, pos_y, leg_step_A, leg_step_B, scale);
            return;
        }
    }
    printf("[RenderVPet] VpetPart not found for %d \n", vpet_code);
}

void RenderMap(SDL_Renderer* renderer, const int map[MAP_HEIGHT][MAP_WIDTH], int height, int width, int title_size) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_FRect tile = {
                x * title_size,
                y * title_size,
                title_size,
                title_size
            };

            int cell_code = map[y][x];

            switch (cell_code)
            {
                case 1:
                    DrawRect(renderer, tile, COLOR_GREY_WALL);
                    // DrawRect(renderer, tile, COLOR_GREY_WALL_BORDER);
                    break;
                case 2:
                case 3:
                    RenderVPet(renderer, cell_code, x, y, title_size);
                    break;
                default:
                    DrawRect(renderer, tile, COLOR_GREEN_GROUND);
                    // DrawRect(renderer, tile, COLOR_GREEN_GROUND_DETAILS);
                    break;
            }
        }
    }
}

VPetEncounter CheckIfFindEnemy(int enemy_code){
    VPetEncounter encounter = {
        .is_found = false
    };

    if (enemy_code == 0 || enemy_code == 1)
    {
        return encounter;
    }
    
    for (int i = 0; i < sizeof(ENEMY_LIST); i++)
    {
        VPet enemy = ENEMY_LIST[i];
        if (enemy.type == (VPetType)enemy_code) {
            printf(enemy.name);
            encounter.is_found = true;
            encounter.enemy = enemy;
            return encounter;
        } 
    }
    return encounter;
}

bool ArrayIncludes(int arr[], int tamanho, int elemento_buscado) {
    for (int i = 0; i < tamanho; i++) {
        if (arr[i] == elemento_buscado) {
            return true; // Encontrou
        }
    }
    return false; // Não encontrou
}

bool IsTileWalkable(int tile_id) {
    // Se for -1 (sem tile) ou um tile de obstáculo (ex: árvores, paredes, água)
    if (tile_id < 0) return false;

    // Exemplo: IDs de 0 a 10 são chão/grama/caminho, acima disso são obstáculos
    // Podes ajustar os IDs conforme o teu layout no Tiled
    if (ArrayIncludes(WALKABLE_TILES, WALKABLE_TILES_COUNT, tile_id)) { 
        return false; // Bloqueia árvores/construções
    }

    return true; // É chão livre
}

bool IsTileAnObject(float x, float y, Coordinates objects[], int objects_count) {
    if (!objects || objects_count <= 0) {
        return false;
    }

    for (int i = 0; i < objects_count; i++) {
        // printf("Checando objeto index %d de %d em %p...\n", i, objects_count, (void*)&objects[i]);
        if (objects[i].x == x && objects[i].y == y)
        {
            return true;
        }        
    }    

    return false;
}

void MovePlayer(
    Coordinates* player, 
    GameState* game_state, 
    VPet* enemy_found, 
    Coordinates* enemy_coordinates, 
    int delta_x, int delta_y, 
    const int map[TILED_MAP_HEIGHT][TILED_MAP_WIDTH], 
    MapMob mobs[], int mob_count,
    Coordinates objects[], int objects_count
) {
    int next_x = player->x + delta_x;
    int next_y = player->y + delta_y;

    // 1. Checa limites do mapa
    if (next_x < 0 || next_x >= TILED_MAP_WIDTH || next_y < 0 || next_y >= TILED_MAP_HEIGHT) {
        return; 
    }

    // 2. Checa colisão com o cenário
    int tile_id = map[next_y][next_x];
    if (!IsTileWalkable(tile_id) || IsTileAnObject(next_x, next_y, objects, objects_count)) {
        return;
    }

    // 3. Checa se a próxima célula contém um mob inimigo ativo
    for (int i = 0; i < mob_count; i++) {
        int mob_grid_x = (int)round(mobs[i].x);
        int mob_grid_y = (int)round(mobs[i].y);

        if (mobs[i].is_active && mob_grid_x == next_x && mob_grid_y == next_y) {
            *game_state = STATE_BATTLE;
            *enemy_found = mobs[i].vpet_data;
            enemy_coordinates->x = next_x;
            enemy_coordinates->y = next_y;
            return;
        }
    }

    // 4. Move o player
    player->x = next_x;
    player->y = next_y;
    
}

PlayerGamePad GetPlayerGamePad(SDL_Gamepad* gamepad_connection) {
    const bool *state = SDL_GetKeyboardState(NULL);

    PlayerGamePad gamepad_delta = {0, 0, false, false, false, false, false, false};

    // Captura entrada (Teclado)
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
        gamepad_delta.delta_x = -1;
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
        gamepad_delta.delta_x = 1;
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
        gamepad_delta.delta_y = -1;
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
        gamepad_delta.delta_y = 1;

    gamepad_delta.triggered_pause = state[SDL_SCANCODE_ESCAPE];
    gamepad_delta.triggered_confirm = state[SDL_SCANCODE_RETURN];

    // Captura entrada (Gamepad D-Pad)
    if (gamepad_connection)
    {
        Sint16 axisX = SDL_GetGamepadAxis(gamepad_connection, SDL_GAMEPAD_AXIS_LEFTX);
        Sint16 axisY = SDL_GetGamepadAxis(gamepad_connection, SDL_GAMEPAD_AXIS_LEFTY);

        if (axisX < -8000)
            gamepad_delta.delta_x = -1;
        if (axisX > 8000)
            gamepad_delta.delta_x = 1;
        if (axisY < -8000)
            gamepad_delta.delta_y = -1;
        if (axisY > 8000)
            gamepad_delta.delta_y = 1;

        if (SDL_GetGamepadButton(gamepad_connection, SDL_GAMEPAD_BUTTON_DPAD_LEFT))
            gamepad_delta.delta_x = -1;
        if (SDL_GetGamepadButton(gamepad_connection, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
            gamepad_delta.delta_x = 1;
        if (SDL_GetGamepadButton(gamepad_connection, SDL_GAMEPAD_BUTTON_DPAD_UP))
            gamepad_delta.delta_y = -1;
        if (SDL_GetGamepadButton(gamepad_connection, SDL_GAMEPAD_BUTTON_DPAD_DOWN))
            gamepad_delta.delta_y = 1;
        
         gamepad_delta.triggered_pause |= SDL_GetGamepadButton(gamepad_connection, SDL_GAMEPAD_BUTTON_START);
         gamepad_delta.triggered_confirm  |= SDL_GetGamepadButton(gamepad_connection, SDL_GAMEPAD_BUTTON_SOUTH);
    }

    gamepad_delta.down = gamepad_delta.delta_y == 1;
    gamepad_delta.up = gamepad_delta.delta_y == -1;    
    gamepad_delta.right = gamepad_delta.delta_x == 1;
    gamepad_delta.left = gamepad_delta.delta_x == -1;
    
    return gamepad_delta;
}

void RenderMenuButton(SDL_Renderer* renderer, float x, float y, const char *text, bool selected, bool disabled) {
    SDL_FRect btn_novo_jogo = {
        x,
        y,
        300.0f,
        60.0f};
    
    Color background_color;
    Color text_color = disabled ? COLOR_GREY: COLOR_WHITE;

    if (selected && !disabled)
    {
       background_color = COLOR_MENU_BUTTON_SELECTED;
    } else if(selected && disabled) {
        background_color = COLOR_MENU_BUTTON_DISABLED_SELECTED;
    } else if(disabled) {
        background_color = COLOR_MENU_BUTTON_DISABLED;
    } else {
        background_color = COLOR_MENU_BUTTON_NOT_SELECTED;
    }

    SDL_SetRenderDrawColor(renderer, background_color.r, background_color.g, background_color.b, background_color.a);
    SDL_RenderFillRect(renderer, &btn_novo_jogo);
    RenderText(renderer, text, text_color, x + 40.0f, y + 18.0f, 4.0f);
}

void RenderVPetStats(SDL_Renderer* renderer, VPet *vpet, float panel_x, float panel_y) {
    // Buffer de texto temporário
    char buffer[32];

    // Nome do V-Pet e Nível
    snprintf(buffer, sizeof(buffer), "NOME: %s", vpet->name);
    RenderText(renderer, buffer, COLOR_WHITE, panel_x + 30.0f, panel_y + 80.0f, 3.0f);

    snprintf(buffer, sizeof(buffer), "LVL : %d", vpet->level);
    RenderText(renderer, buffer, COLOR_WHITE, panel_x + 30.0f, panel_y + 120.0f, 3.0f);

    // HP Numérico
    snprintf(buffer, sizeof(buffer), "HP  : %d/%d", vpet->hp, vpet->hp_max);
    RenderText(renderer, buffer, COLOR_WHITE, panel_x + 30.0f, panel_y + 160.0f, 3.0f);

    // --- Barra Visual de HP ---
    float bar_x = panel_x + 230.0f;
    float bar_y = panel_y + 160.0f;
    float bar_w = 140.0f;
    float bar_h = 16.0f;

    // Fundo da Barra (Vermelho/Escuro - HP Faltante)
    SDL_FRect hp_bg = {bar_x, bar_y, bar_w, bar_h};
    SDL_SetRenderDrawColor(renderer, 80, 20, 20, 255);
    SDL_RenderFillRect(renderer, &hp_bg);

    // Preenchimento da Barra (Verde - HP Atual)
    float hp_ratio = (float)vpet->hp / (float)vpet->hp_max;
    if (hp_ratio < 0.0f)
        hp_ratio = 0.0f;
    if (hp_ratio > 1.0f)
        hp_ratio = 1.0f;

    SDL_FRect hp_fill = {bar_x, bar_y, bar_w * hp_ratio, bar_h};
    SDL_SetRenderDrawColor(renderer, 40, 200, 80, 255);
    SDL_RenderFillRect(renderer, &hp_fill);

    // Borda da Barra
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderRect(renderer, &hp_bg);

    // Ataque e Defesa
    snprintf(buffer, sizeof(buffer), "ATK : %d", vpet->strength);
    RenderText(renderer, buffer, COLOR_WHITE, panel_x + 30.0f, panel_y + 200.0f, 3.0f);

    snprintf(buffer, sizeof(buffer), "DEF : %d", vpet->defense);
    RenderText(renderer, buffer, COLOR_WHITE, panel_x + 30.0f, panel_y + 240.0f, 3.0f);
}

void RenderPauseOverlay(SDL_Renderer* renderer, VPet *vpet, float screenWidth, float screenHeight) {
    // 1. Escurece a tela toda com um preto semi-transparente (Alpha = 150)
    SDL_FRect overlay = { 0.0f, 0.0f, screenWidth, screenHeight };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &overlay);

    // 2. Janela Central de Status (Tamanho: 400x320, Centralizada)
    float panel_w = 400.0f;
    float panel_h = 320.0f;
    float panel_x = (screenWidth - panel_w) / 2.0f;
    float panel_y = (screenHeight - panel_h) / 2.0f;

    SDL_FRect panel = { panel_x, panel_y, panel_w, panel_h };

    // Fundo da Janela (Azul escuro com transparência)
    SDL_SetRenderDrawColor(renderer, 20, 25, 40, 230);
    SDL_RenderFillRect(renderer, &panel);

    // Borda da Janela (Cinza claro/Branco)
    SDL_SetRenderDrawColor(renderer, 200, 200, 220, 255);
    SDL_RenderRect(renderer, &panel);

    // Título da Janela
    RenderText(renderer, "PAUSE", COLOR_WHITE, panel_x + 160.0f, panel_y + 20.0f, 4.0f);

    // Linha Divisória Horizontal sob o Título
    SDL_FRect line = { panel_x + 20.0f, panel_y + 60.0f, panel_w - 40.0f, 2.0f };
    SDL_SetRenderDrawColor(renderer, 100, 100, 130, 255);
    SDL_RenderFillRect(renderer, &line);

    RenderVPetStats(renderer, vpet, panel_x, panel_y);
}

void RemoveEnemyFromMap(int map[TILED_MAP_HEIGHT][TILED_MAP_WIDTH], Coordinates* position, MapMob* mobs, int mob_count) {
    for (int i = 0; i < mob_count; i++) {
        // Altera diretamente no array original
        if (mobs[i].x == position->x && mobs[i].y == position->y) {
            mobs[i].is_active = false;
            return;
        }
    }
}

void UpdateCamera(Camera* cam, Coordinates player_grid_coords, float player_w, float player_h, int map_width_tiles, int map_height_tiles, float scaled_tile_size) {

    // 1. Converte a posição do jogador do Grid para Pixels no Mundo
    float player_pixel_x = player_grid_coords.x * scaled_tile_size;
    float player_pixel_y = player_grid_coords.y * scaled_tile_size;

    // Tamanho real total do mapa em Pixels no Mundo
    float map_pixel_width = map_width_tiles * scaled_tile_size;
    float map_pixel_height = map_height_tiles * scaled_tile_size;

    // 2. Centraliza a câmera no jogador (usando as posições em pixels)
    cam->x = (player_pixel_x + player_w / 2.0f) - (cam->width / 2.0f);
    cam->y = (player_pixel_y + player_h / 2.0f) - (cam->height / 2.0f);

    // 3. Trava a câmera se o mapa for maior que a tela
    if (map_pixel_width > cam->width) {
        if (cam->x < 0) cam->x = 0;
        if (cam->x > map_pixel_width - cam->width) cam->x = map_pixel_width - cam->width;
    } else {
        // Se o mapa couber inteiro na tela, centraliza o mapa
        cam->x = -(cam->width - map_pixel_width) / 2.0f;
    }

    if (map_pixel_height > cam->height) {
        if (cam->y < 0) cam->y = 0;
        if (cam->y > map_pixel_height - cam->height) cam->y = map_pixel_height - cam->height;
    } else {
        cam->y = -(cam->height - map_pixel_height) / 2.0f;
    }
}

MapMob* SpawnMobs(int *tamanho_retorno, int count,int map[TILED_MAP_HEIGHT][TILED_MAP_WIDTH], int map_width_tiles, int map_height_tiles) {
    int capacidade = 2;
    int mob_array_size = 0;
    
    // CORREÇÃO 1: Usar sizeof(MapMob) na alocação inicial
    MapMob *mob_array = malloc(capacidade * sizeof(MapMob));
    if (!mob_array) return NULL; // Boa prática: verificar se o malloc falhou
    
    for (int i = 0; i < count; i++) {
        if (mob_array_size >= capacidade) {
            capacidade *= 2;
            // CORREÇÃO 2: Usar sizeof(MapMob) no realloc
            MapMob *temp = realloc(mob_array, capacidade * sizeof(MapMob));
            if (!temp) {
                // Tratar falha de memória se necessário
                break;
            }
            mob_array = temp;
        }

        
        int indice_sorteado = rand() % ENEMY_LIST_COUNT;
        VPet pet = ENEMY_LIST[indice_sorteado];

        int pos_x, pos_y;
        int tile_id;

        // Sorteia posições até encontrar um tile onde seja possível caminhar
        do {
            pos_x = rand() % map_width_tiles;
            pos_y = rand() % map_height_tiles;
            tile_id = map[pos_y][pos_x];
        } while (!IsTileWalkable(tile_id));

        MapMob mob = {
            .is_active = true,
            .vpet_data = pet,
            .x = pos_x,
            .y = pos_y
        };

        mob_array[mob_array_size] = mob; // Agora há espaço suficiente na memória!
        mob_array_size++;
    }

    // Não te esqueças de preencher o ponteiro de tamanho no final
    if (tamanho_retorno) {
        *tamanho_retorno = mob_array_size;
    }

    return mob_array;
}

void RenderEnemies(SDL_Renderer* renderer, Camera cam, MapMob mobs[], int mob_count, float scaled_tile) {
    for (int i = 0; i < mob_count; i++) {
        if (mobs[i].is_active) {
            RenderVPet(renderer, 
                mobs[i].vpet_data.type,
                (float)(mobs[i].x * scaled_tile) - cam.x,
                (float)(mobs[i].y * scaled_tile) - cam.y,
                scaled_tile
            );
        }
    }
}