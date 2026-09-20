bool LoadAssets(SDL_Renderer* renderer, SDL_Texture** logo_texture) {
    // Carrega o PNG alterando diretamente o ponteiro original
    *logo_texture = IMG_LoadTexture(renderer, "assets/logo.png");
    
    if (!(*logo_texture)) {
        SDL_Log("Erro ao carregar assets/logo.png: %s", SDL_GetError());
        return false;
    }

    // Define o modo de escala no ponteiro desreferenciado
    SDL_SetTextureScaleMode(*logo_texture, SDL_SCALEMODE_NEAREST);

    return true;
}

void RenderMenuLogo(SDL_Renderer* renderer, SDL_Texture* logo_texture, float screenWidth, float y) {
    if (!logo_texture) return;

    float tex_w = 0.0f;
    float tex_h = 0.0f;

    if (!SDL_GetTextureSize(logo_texture, &tex_w, &tex_h)) {
        return;
    }

    float scale = 0.7f; 
    float dest_w = tex_w * scale;
    float dest_h = tex_h * scale;

    SDL_FRect dest_rect = {
        (screenWidth - dest_w) / 2.0f,
        y,
        dest_w,
        dest_h
    };

    SDL_RenderTexture(renderer, logo_texture, NULL, &dest_rect);
}
