
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

void HomeMenu(SDL_Renderer* renderer, SDL_Texture *game_logo, int menu_opcao){
    RenderMenuLogo(renderer, game_logo, SCREEN_WIDTH, 40.0f);
    float top = 320.0f;
    RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, top, "NOVO JOGO", menu_opcao == 0, false);
    float spacing = 80.0f;
    RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, top + spacing, "CONTINUAR", menu_opcao == 1, false);
}