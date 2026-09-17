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