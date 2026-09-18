#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "functions.h"
#include "maps.h"

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
    PlayerPosition player = {
        FIRST_MAP_SPAWN_POINT[0],
        FIRST_MAP_SPAWN_POINT[1]
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
    static bool key_pressed = false;

    static bool pause_key_pressed = false;

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

        GamePadDelta gamepad_delta = GetGamePadDelta(gamepad);

        if (gamepad_delta.triggered_pause && !pause_key_pressed)
        {
            if (estado_atual == STATE_MAP)
            {
                estado_atual = STATE_PAUSE;
            }
            else if (estado_atual == STATE_PAUSE)
            {
                estado_atual = STATE_MAP;
            }
            pause_key_pressed = true;
        }

        // Libera a trava quando solta o botão
        if (!gamepad_delta.triggered_pause)
        {
            pause_key_pressed = false;
        }

        // --- Atualização do Menu ---
        if (estado_atual == STATE_MENU)
        {
            bool moveu_baixo = gamepad_delta.y == 1;
            bool moveu_cima = gamepad_delta.y == -1;

            if (moveu_baixo && !key_pressed)
            {
                menu_opcao = 1; // Seleciona "Continuar"
                key_pressed = true;
            }
            else if (moveu_cima && !key_pressed)
            {
                menu_opcao = 0; // Seleciona "Novo Jogo"
                key_pressed = true;
            }

            if (!moveu_baixo && !moveu_cima)
            {
                key_pressed = false;
            }

            if (gamepad_delta.triggered_confirm)
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
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, 200, "NOVO JOGO", menu_opcao == 0, false);

            // Opção 2: CONTINUAR (DESABILITADO)
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, 280.0f, "CONTINUAR", menu_opcao == 1, true);
        }
        else if (estado_atual == STATE_MAP) {
            

            // Executa movimento único quando a tecla é pressionada
            if ((gamepad_delta.x != 0 || gamepad_delta.y != 0) && !key_pressed)
            {
                MovePlayer(&player, gamepad_delta.x, gamepad_delta.y, FIRST_MAP);
                key_pressed = true; // Trava para não andar continuamente
            }

            // Libera a trava quando solta o botão
            if (gamepad_delta.x == 0 && gamepad_delta.y == 0)
            {
                key_pressed = false;
            }

            RenderMap(renderer, FIRST_MAP, MAP_HEIGHT, MAP_WIDTH, TILE_SIZE);

            // Desenha o jogador (quadrado azul no centro por enquanto)
            SDL_FRect playerRect = PlayerToRect(player, TILE_SIZE);
            SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);            
            SDL_RenderFillRect(renderer, &playerRect);
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