
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

void RenderPauseDivider(SDL_Renderer* renderer, float pos_y, float panel_x, float panel_w){
    SDL_FRect line = { panel_x + 20.0f, pos_y, panel_w - 40.0f, 2.0f };
    SDL_SetRenderDrawColor(renderer, 100, 100, 130, 255);
    SDL_RenderFillRect(renderer, &line);
}

void RenderPauseVpetCoinsBalance(SDL_Renderer* renderer, float vpetcoin_balance ,float pos_y, float panel_x) {
    char buffer[32];
    snprintf(buffer, sizeof(buffer), "%.1f VPETCOINS", vpetcoin_balance);
    RenderText(renderer, buffer, COLOR_WHITE, panel_x + 30.0f, pos_y, 3.0f);
}

void RenderPauseOverlay(SDL_Renderer* renderer, VPet *vpet, float vpetcoin_balance, float screenWidth, float screenHeight) {
    // 1. Escurece a tela toda com um preto semi-transparente (Alpha = 150)
    SDL_FRect overlay = { 0.0f, 0.0f, screenWidth, screenHeight };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_RenderFillRect(renderer, &overlay);

    // 2. Janela Central de Status (Tamanho: 400x320, Centralizada)
    float panel_w = 400.0f;
    float panel_h = 330.0f;
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
    float divider1 = panel_y + 60.0f;
    RenderPauseDivider(renderer,  divider1, panel_x, panel_w);
    RenderVPetStats(renderer, vpet, panel_x, panel_y);
    float divider2 = divider1 + 210.0f;
    RenderPauseDivider(renderer,  divider2, panel_x, panel_w);
    RenderPauseVpetCoinsBalance(renderer, vpetcoin_balance, divider2+ 20.0f, panel_x);
}