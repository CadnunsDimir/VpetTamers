#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "functions.h"

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
    GameState estado_atual = STATE_MENU;
    VPet meu_vpet = {
        .name = "Agumon",
        .level = 1,
        .hp = 20,
        .hp_max = 20,
        .strength = 5,
        .defense = 3
    };

    // Variável de controle da opção selecionada (0 = Novo Jogo, 1 = Continuar)
    int menu_opcao = 0;

    // Trava simples para evitar navegação ultra-rápida (pode usar eventos ou flag de liberação)
    static bool tecla_pressionada = false;

    Color COLOR_WHITE = { 255,255,255,255 };
    Color COLOR_GREY = {150, 150, 150, 255};

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

        
        // --- Atualização do Menu ---
        if (estado_atual == STATE_MENU)
        {
            const bool *state = SDL_GetKeyboardState(NULL);

            bool moveu_baixo = state[SDL_SCANCODE_DOWN];
            bool moveu_cima = state[SDL_SCANCODE_UP];

            if (gamepad)
            {
                moveu_baixo |= SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_DOWN);
                moveu_cima |= SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_DPAD_UP);
            }

            if (moveu_baixo && !tecla_pressionada)
            {
                menu_opcao = 1; // Seleciona "Continuar"
                tecla_pressionada = true;
            }
            else if (moveu_cima && !tecla_pressionada)
            {
                menu_opcao = 0; // Seleciona "Novo Jogo"
                tecla_pressionada = true;
            }

            if (!moveu_baixo && !moveu_cima)
            {
                tecla_pressionada = false;
            }

            bool confirma = state[SDL_SCANCODE_RETURN];
            if (gamepad)
            {
                confirma |= SDL_GetGamepadButton(gamepad, SDL_GAMEPAD_BUTTON_SOUTH);
            }

            if (confirma)
            {
                if (menu_opcao == 0)
                {
                    estado_atual = STATE_MAP;
                }
            }
        }

        // --- Renderização ---
        // 1. Limpa a tela com a cor de fundo (Escuro)
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        // 2. Desenha o estado atual
        if (estado_atual == STATE_MENU) {
            // Opção 1: NOVO JOGO
            SDL_FRect btn_novo_jogo = {
                (SCREEN_WIDTH - 300) / 2.0f,
                200.0f,
                300.0f,
                60.0f
            };

            if (menu_opcao == 0) {
                SDL_SetRenderDrawColor(renderer, 0, 180, 255, 255); // Selecionado
            } else {
                SDL_SetRenderDrawColor(renderer, 0, 70, 130, 255);  // Não Selecionado
            }
            SDL_RenderFillRect(renderer, &btn_novo_jogo);
            RenderText(renderer, "NOVO JOGO", COLOR_WHITE, ((SCREEN_WIDTH - 300) / 2.0f) + 40.0f, 218.0f, 4.0f);

            // Opção 2: CONTINUAR (DESABILITADO)
            SDL_FRect btn_continuar = {
                (SCREEN_WIDTH - 300) / 2.0f,
                280.0f,
                300.0f,
                60.0f
            };

            if (menu_opcao == 1) {
                SDL_SetRenderDrawColor(renderer, 120, 120, 120, 255); // Focado (Desabilitado)
            } else {
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);   // Apagado
            }
            SDL_RenderFillRect(renderer, &btn_continuar);
            RenderText(renderer, "CONTINUAR", COLOR_GREY, ((SCREEN_WIDTH - 300) / 2.0f) + 40.0f, 298.0f, 4.0f);
        }
        else if (estado_atual == STATE_MAP) {
            // Desenha o jogador (quadrado azul no centro por enquanto)
            SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);
            SDL_RenderFillRect(renderer, &player);
        }

        // 3. Apresenta o buffer renderizado na janela (ESSENCIAL!)
        SDL_RenderPresent(renderer);

        // 4. Pausa de ~16ms para cravar em 60 FPS e liberar a CPU
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