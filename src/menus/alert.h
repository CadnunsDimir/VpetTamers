typedef enum {
    MESSAGE_TYPE_ERROR,
    MESSAGE_TYPE_INFO,
    MESSAGE_TYPE_WARN,
} MessageType;

typedef struct {
    char message[100];
    MessageType type;
    Uint64 start_time_ms;
    Uint64 duration;
} Alert;

void SetColor(SDL_Renderer* renderer, Color color) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void RenderAlert(SDL_Renderer* renderer, Alert alert, float screen_w){
    Uint64 current_time = SDL_GetTicks();
    if (current_time > (alert.start_time_ms + alert.duration))
    {
        return;
    }

    size_t msg_len = strlen(alert.message);
    if (msg_len == 0 || alert.message[0] == '\0')
    {
        printf("ERROR: alert com mensagem vazia\n");
        return;
    }
    
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    
    float panel_w = msg_len * 10;
    float panel_h = 50;
    float panel_x = (screen_w - panel_w) / 2.0f;
    float panel_y = 20;
    float text_x = panel_x + 20;
    float text_y = panel_y + 20;
    Color text_color;
    SDL_FRect panel = { panel_x, panel_y, panel_w, panel_h };

    switch (alert.type)
    {
        case MESSAGE_TYPE_ERROR:
            text_color = COLOR_MESSAGE_ERROR;
            break;
        case MESSAGE_TYPE_WARN:
            text_color = COLOR_MESSAGE_WARNING;
            break;
        default:
            text_color = COLOR_WHITE;
            break;
    }

    // Fundo da Janela (Azul escuro/noturno)
    SetColor(renderer, COLOR_DIALOG_BACKGROUND);
    SDL_RenderFillRect(renderer, &panel);

    // Borda da Janela
    SetColor(renderer, COLOR_DIALOG_BORDER);
    SDL_RenderRect(renderer, &panel);    

    RenderText(renderer, alert.message, text_color, text_x, text_y, 1.5f);
}

void ShowAlert(Alert* alert, char* message, MessageType type) {
    strncpy(alert->message, message, sizeof(alert->message) - 1);
    alert->message[sizeof(alert->message) - 1] = '\0'; // Garante o terminação nula
    alert->type = type;
    alert->duration = ALERT_DURATION;
    alert->start_time_ms = SDL_GetTicks();
}