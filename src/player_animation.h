#define MAIN_CHAR_H 120
#define MAIN_CHAR_W 64
#define MAIN_CHAR_TILE_WIDTH 210
#define MAIN_CHAR_TILE_HEIGHT 350
#define MAIN_CHAR_TILE_ROWS 5
#define MAIN_CHAR_TILE_COLS 2
#define WALK_ANIMATION_TIMES 0.120f

typedef enum {
    DIRECTION_DOWN,
    DIRECTION_UP,
    DIRECTION_LEFT,
    DIRECTION_RIGHT
} PlayerAnimationDirection;

typedef struct {
    SDL_Texture *texture;
    int frame;
    float frame_timer;
    float frame_duration;
    PlayerAnimationDirection direction;
    bool moving;
} PlayerAnimation;

// Altere o parâmetro para receber o endereço do ponteiro (SDL_Surface **)
bool LoadPlayerSurface(SDL_Surface **main_character_surface) {
    // 1. Verificação de segurança: o ponteiro para o ponteiro é válido?
    if (!main_character_surface) {
        SDL_Log("Erro interno: Ponteiro de destino inválido (NULL)");
        return false;
    }

    char* main_character_file = "assets/main_character_sprite.png";

    // 2. Atribui o retorno do IMG_Load ao ponteiro que está no endereço passado
    *main_character_surface = IMG_Load(main_character_file);

    // 3. Verifica se a imagem foi carregada com sucesso (se o ponteiro resultante não é NULL)
    if (*main_character_surface == NULL) {
        SDL_Log("Erro ao carregar '%s': %s", main_character_file, SDL_GetError());
        return false;
    }

    SDL_Log("Superfície '%s' carregada com sucesso.", main_character_file);
    return true;
}

void ApplyTexture(SDL_Renderer *renderer, SDL_Surface *surface,  PlayerAnimation *player){
    player->texture = SDL_CreateTextureFromSurface(renderer, surface);
}

SDL_FRect GetTileRect(int index)
{
    int column = index % MAIN_CHAR_TILE_COLS;
    int row = index / MAIN_CHAR_TILE_COLS;

    SDL_FRect rect = {
        column * MAIN_CHAR_TILE_WIDTH,
        row * MAIN_CHAR_TILE_HEIGHT,
        MAIN_CHAR_TILE_WIDTH,
        MAIN_CHAR_TILE_HEIGHT
    };

    return rect;
}

void UpdateDeltaAndCurrentTime(float *app_time, float *delta_time) {
    if (!app_time || !delta_time) return;

    // SDL_GetTicks() retorna Uint64 no SDL3 (ou Uint32 no SDL2)
    float current_time = (float)SDL_GetTicks(); 

    // Derefencia *app_time para fazer a subtração entre dois números floats
    // Derefencia *delta_time para gravar o resultado na memória de destino
    *delta_time = (current_time - *app_time) / 1000.0f;
    *app_time = current_time;
}

void UpdatePlayerAnimation(PlayerAnimation *player, float delta_time)
{
    if(!player->moving){
        switch (player->direction)
        {
            case DIRECTION_LEFT:
                player->frame = 8;
                break;
            case DIRECTION_RIGHT:
                player->frame = 8;
                break;
            case DIRECTION_UP:
                player->frame = 7;
                break;
            default:
                player->frame = 6;
                break;
        }
        return;
    }

    player->frame_timer += delta_time;

    if (player->frame_timer >= WALK_ANIMATION_TIMES)
    {
        player->frame_timer -= WALK_ANIMATION_TIMES;

        switch (player->direction)
        {
            case DIRECTION_DOWN:
                player->frame = (player->frame == 0) ? 1 : 0;
                break;

            case DIRECTION_UP:
                player->frame = (player->frame == 2) ? 3 : 2;
                break;

            case DIRECTION_LEFT:
            case DIRECTION_RIGHT:
                player->frame = (player->frame == 4) ? 5 : 4;
                break;
        }
    }
}

void UpdatePlayerDirection(
    PlayerAnimation *player,
    PlayerGamePad gamepad,
    float delta_time) {
    player->moving = (gamepad.delta_x != 0 || gamepad.delta_y != 0);

    // Captura entrada (Teclado)
    if (gamepad.left){
        player->direction = DIRECTION_LEFT;
    }       
    if (gamepad.right){
        player->direction = DIRECTION_RIGHT;
    }
    if (gamepad.up){
        player->direction = DIRECTION_UP;
    }
    if (gamepad.down){
        player->direction = DIRECTION_DOWN;
    }

    UpdatePlayerAnimation(player, delta_time);
}

void RenderPlayerUsingSprites(
    SDL_Renderer *renderer, 
    PlayerAnimation *player,
    Coordinates player_coordinates,
    Camera cam) {

    float playerSize = SCALED_TILED_SIZE;
    float adjust_h = MAIN_CHAR_H /2;
    Coordinates playerRect = {
        (player_coordinates.x * playerSize) - cam.x,
        (player_coordinates.y * playerSize) - cam.y - adjust_h,
    };
    
    bool flip = player->direction == DIRECTION_LEFT;

    SDL_FRect src = GetTileRect(player->frame);

    SDL_FRect dst = {
        playerRect.x,
        playerRect.y,
        MAIN_CHAR_W,
        MAIN_CHAR_H
    };

    SDL_RenderTextureRotated(
        renderer,
        player->texture,
        &src,
        &dst,
        0.0,
        NULL,
        flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE
    );
}