#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>

#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define PLAYER_SIZE   40.0f
#define SPEED         5.0f

int main(int argc, char* argv[]) {
    // 1. Inicializa o SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // 2. Cria Janela e Renderizador juntos no SDL3
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    if (!SDL_CreateWindowAndRenderer("Jogo com Joystick - SDL3", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        printf("Erro ao criar janela/renderizador: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 3. Abre o primeiro Gamepad/Joystick disponível
    SDL_Gamepad* gamepad = NULL;
    int count = 0;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
    if (count > 0 && joysticks) {
        gamepad = SDL_OpenGamepad(joysticks[0]);
        if (gamepad) {
            printf("Gamepad conectado: %s\n", SDL_GetGamepadName(gamepad));
        }
    }
    if (joysticks) {
        SDL_free(joysticks);
    }

    // Posicao do jogador (SDL3 usa float para retângulos)
    SDL_FRect player = {
        (SCREEN_WIDTH - PLAYER_SIZE) / 2.0f,
        (SCREEN_HEIGHT - PLAYER_SIZE) / 2.0f,
        PLAYER_SIZE,
        PLAYER_SIZE
    };

    bool running = true;
    SDL_Event event;

    // Loop principal
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_GAMEPAD_ADDED && !gamepad) {
                gamepad = SDL_OpenGamepad(event.gdevice.which);
                if (gamepad) {
                    printf("Gamepad conectado: %s\n", SDL_GetGamepadName(gamepad));
                }
            }
            else if (event.type == SDL_EVENT_GAMEPAD_REMOVED && gamepad) {
                SDL_CloseGamepad(gamepad);
                gamepad = NULL;
                printf("Gamepad desconectado.\n");
            }
        }

        // Teclado (fallback)
        const bool* state = SDL_GetKeyboardState(NULL);
        if (state[SDL_SCANCODE_LEFT] && player.x > 0) player.x -= SPEED;
        if (state[SDL_SCANCODE_RIGHT] && player.x < SCREEN_WIDTH - PLAYER_SIZE) player.x += SPEED;
        if (state[SDL_SCANCODE_UP] && player.y > 0) player.y -= SPEED;
        if (state[SDL_SCANCODE_DOWN] && player.y < SCREEN_HEIGHT - PLAYER_SIZE) player.y += SPEED;

        // Joystick / Gamepad (Digital / D-Pad ou Analógico)
        if (gamepad) {
            // Eixos analogicos
            Sint16 axisX = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTX);
            Sint16 axisY = SDL_GetGamepadAxis(gamepad, SDL_GAMEPAD_AXIS_LEFTY);

            if (axisX < -8000 && player.x > 0) player.x -= SPEED;
            if (axisX > 8000 && player.x < SCREEN_WIDTH - PLAYER_SIZE) player.x += SPEED;
            if (axisY < -8000 && player.y > 0) player.y -= SPEED;
            if (axisY > 8000 && player.y < SCREEN_HEIGHT - PLAYER_SIZE) player.y += SPEED;

            // D-Pad / Botões direcionais
            if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_LEFT) && player.x > 0) player.x -= SPEED;
            if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_RIGHT) && player.x < SCREEN_WIDTH - PLAYER_SIZE) player.x += SPEED;
            if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP) && player.y > 0) player.y -= SPEED;
            if (SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN) && player.y < SCREEN_HEIGHT - PLAYER_SIZE) player.y += SPEED;
        }

        // --- Renderização ---
        SDL_SetRenderDrawColor(renderer, 30, 30, 35, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
        SDL_RenderFillRect(renderer, &player);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    if (gamepad) {
        SDL_CloseGamepad(gamepad);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}