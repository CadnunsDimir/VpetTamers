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
            case ' ':
                continue; // Espaço em branco
            default: // Caractere desconhecido vira um bloco sólido
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

SDL_FRect PlayerToRect(PlayerPosition player, int playerSize) {
    SDL_FRect rect = {
        player.x  * playerSize,
        player.y * playerSize,
        playerSize,
        playerSize
    };

    return rect;
}

void DrawRect(SDL_Renderer* renderer, SDL_FRect tile, Color color) {
     SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &tile);
}

void RenderMap(SDL_Renderer* renderer, const int map[MAP_HEIGHT][MAP_WIDTH], int height, int width, int titleSize) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            SDL_FRect tile = {
                x * titleSize,
                y * titleSize,
                titleSize,
                titleSize
            };

            if (map[y][x] == 1) {
                DrawRect(renderer, tile, COLOR_GREY_WALL);
                DrawRect(renderer, tile, COLOR_GREY_WALL_BORDER);
            } else {
                DrawRect(renderer, tile, COLOR_GREEN_GROUND);
                DrawRect(renderer, tile, COLOR_GREEN_GROUND_DETAILS);
            }
        }
    }
}

void MovePlayer(PlayerPosition* player, int delta_x, int delta_y, const int map[MAP_HEIGHT][MAP_WIDTH]) {
    int next_x = player->x + delta_x;
    int next_y = player->y + delta_y;

    // 1. Checa limites do mapa (para não tentar ler fora do array na RAM)
    if (next_x < 0 || next_x >= MAP_WIDTH || next_y < 0 || next_y >= MAP_HEIGHT) {
        return; // Fora do mapa -> Bloqueia
    }

    // 2. Checa colisão com parede (1 = Parede)
    if (map[next_y][next_x] == 1) {
        return; // Colisão detectada -> Bloqueia
    }

    // 3. Movimento válido -> Atualiza a posição
    player->x = next_x;
    player->y = next_y;
}

GamePadDelta GetGamePadDelta(SDL_Gamepad* gamepad) {
    const bool *state = SDL_GetKeyboardState(NULL);

    GamePadDelta gamepad_delta = {0, 0, false, false};

    // Captura entrada (Teclado)
    if (state[SDL_SCANCODE_LEFT] || state[SDL_SCANCODE_A])
        gamepad_delta.x = -1;
    if (state[SDL_SCANCODE_RIGHT] || state[SDL_SCANCODE_D])
        gamepad_delta.x = 1;
    if (state[SDL_SCANCODE_UP] || state[SDL_SCANCODE_W])
        gamepad_delta.y = -1;
    if (state[SDL_SCANCODE_DOWN] || state[SDL_SCANCODE_S])
        gamepad_delta.y = 1;

    gamepad_delta.triggered_pause = state[SDL_SCANCODE_ESCAPE];
    gamepad_delta.triggered_confirm = state[SDL_SCANCODE_RETURN];

    // Captura entrada (Gamepad D-Pad)
    if (gamepad)
    {
        Sint16 axisX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
        Sint16 axisY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);

        if (axisX < -8000)
            gamepad_delta.x = -1;
        if (axisX > 8000)
            gamepad_delta.x = 1;
        if (axisY < -8000)
            gamepad_delta.y = -1;
        if (axisY > 8000)
            gamepad_delta.y = 1;

        if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT))
            gamepad_delta.x = -1;
        if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT))
            gamepad_delta.x = 1;
        if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP))
            gamepad_delta.y = -1;
        if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN))
            gamepad_delta.y = 1;
        
         gamepad_delta.triggered_pause |= SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_START);
         gamepad_delta.triggered_confirm  |= SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_SOUTH);
    }
    
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