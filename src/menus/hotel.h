void Window(SDL_Renderer* renderer, float screenWidth, float screenHeight, Uint8 overlay_backgroud, SDL_FRect panel){
    // Modo de transparência ativo para sobreposições
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // 1. Escurece o fundo da tela (Overlay preto semi-transparente)
    SDL_FRect overlay = { 0.0f, 0.0f, screenWidth, screenHeight };
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, overlay_backgroud);
    SDL_RenderFillRect(renderer, &overlay);    

    // Fundo da Janela (Azul escuro/noturno)
    SDL_SetRenderDrawColor(renderer, 20, 25, 40, 230);
    SDL_RenderFillRect(renderer, &panel);

    // Borda da Janela
    SDL_SetRenderDrawColor(renderer, 200, 200, 220, 255);
    SDL_RenderRect(renderer, &panel);

    // 3. Título da Janela
    RenderText(renderer, "HOTEL VPET", COLOR_WHITE, panel.x + 100.0f, panel.y + 20.0f, 4.0f);

    // Linha Divisória Horizontal sob o Título
    SDL_FRect line = { panel.x + 20.0f, panel.y + 55.0f, panel.w - 40.0f, 2.0f };
    SDL_SetRenderDrawColor(renderer, 100, 100, 130, 255);
    SDL_RenderFillRect(renderer, &line);
}

void RenderHotelMenu(SDL_Renderer* renderer, int current_option, float screenWidth, float screenHeight) {
    // 2. Janela Central do Hotel (Tamanho: 420x260, Centralizada)
    float panel_w = 420.0f;
    float panel_h = 260.0f;
    float panel_x = (screenWidth - panel_w) / 2.0f;
    float panel_y = (screenHeight - panel_h) / 2.0f;

    SDL_FRect panel = { panel_x, panel_y, panel_w, panel_h };
    Window(renderer, screenWidth, screenHeight, 150, panel);

    // 4. Mensagem de Pergunta e Custo
    RenderText(renderer, "DESEJA SE HOSPEDAR?", COLOR_WHITE, panel_x + 70.0f, panel_y + 80.0f, 2.5f);
    RenderText(renderer, "CUSTO: 16 VPETCOINS", COLOR_YELLOW, panel_x + 45.0f, panel_y + 115.0f, 3.0f);

    // 5. Botões de Opção (SIM / NAO)
    float btn_w = 120.0f;
    float btn_h = 45.0f;
    float btn_y = panel_y + 180.0f;

    // Botão SIM (Esquerda)
    float btn_sim_x = panel_x + 50.0f;
    SDL_FRect btn_sim = { btn_sim_x, btn_y, btn_w, btn_h };

    // Botão NAO (Direita)
    float btn_nao_x = panel_x + panel_w - 170.0f;
    SDL_FRect btn_nao = { btn_nao_x, btn_y, btn_w, btn_h };

    // Desenha Botão SIM (Destaque amarelo se selecionado, cinza escuro se inativo)
    if (current_option == 0) { // 0 = SIM
        SDL_SetRenderDrawColor(renderer, 50, 180, 80, 230); // Verde ativo
        SDL_RenderFillRect(renderer, &btn_sim);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &btn_sim);
        RenderText(renderer, "SIM", COLOR_WHITE, btn_sim_x + 38.0f, btn_y + 12.0f, 3.0f);
    } else {
        SDL_SetRenderDrawColor(renderer, 40, 45, 60, 200);
        SDL_RenderFillRect(renderer, &btn_sim);
        SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
        SDL_RenderRect(renderer, &btn_sim);
        RenderText(renderer, "SIM", COLOR_GREY, btn_sim_x + 38.0f, btn_y + 12.0f, 3.0f);
    }

    // Desenha Botão NAO
    if (current_option == 1) { // 1 = NAO
        SDL_SetRenderDrawColor(renderer, 200, 60, 60, 230); // Vermelho ativo
        SDL_RenderFillRect(renderer, &btn_nao);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderRect(renderer, &btn_nao);
        RenderText(renderer, "NAO", COLOR_WHITE, btn_nao_x + 38.0f, btn_y + 12.0f, 3.0f);
    } else {
        SDL_SetRenderDrawColor(renderer, 40, 45, 60, 200);
        SDL_RenderFillRect(renderer, &btn_nao);
        SDL_SetRenderDrawColor(renderer, 100, 100, 120, 255);
        SDL_RenderRect(renderer, &btn_nao);
        RenderText(renderer, "NAO", COLOR_GREY, btn_nao_x + 38.0f, btn_y + 12.0f, 3.0f);
    }
}

void ShowHotelSleepingMessage(SDL_Renderer* renderer, float screenWidth, float screenHeight) {
    // 2. Janela Central do Hotel (Tamanho: 420x260, Centralizada)
    float panel_w = 420.0f;
    float panel_h = 260.0f;
    float panel_x = (screenWidth - panel_w) / 2.0f;
    float panel_y = (screenHeight - panel_h) / 2.0f;

    SDL_FRect panel = { panel_x, panel_y, panel_w, panel_h };
    Window(renderer, screenWidth, screenHeight, 240, panel);

    RenderText(renderer, "Dormindo...", COLOR_WHITE, panel_x + 70.0f, panel_y + 80.0f, 2.5f);
}