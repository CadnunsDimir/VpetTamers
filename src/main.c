#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "functions.h"
#include "maps.h"
#include "battle_mode.h"

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

    //habilita Blend para criar transparencias
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // 3. Abre o primeiro Gamepad/Joystick disponível
    SDL_Gamepad* gamepad_connection = NULL;
    int count = 0;
    SDL_JoystickID* joysticks = SDL_GetJoysticks(&count);
    if (count > 0 && joysticks) {
        gamepad_connection = SDL_OpenGamepad(joysticks[0]);
        if (gamepad_connection) {
            printf("Gamepad conectado: %s\n", SDL_GetGamepadName(gamepad_connection));
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
    GameState current_state = STATE_MENU;
    VPet my_vpet = {
        .name = "BkMbaracajamon",
        .level = 1,
        .hp = 20,
        .hp_max = 20,
        .strength = 5,
        .defense = 3,
        .type = PET_BK_MBARACAJAMON
    };

    VPet enemy;

    // Variável de controle da opção selecionada (0 = Novo Jogo, 1 = Continuar)
    int menu_opcao = 0;
    BattleMenuOption battle_menu_option = BATLLE_MENU_ATTACK;

    // Trava simples para evitar navegação ultra-rápida (pode usar eventos ou flag de liberação)
    //TODO: ajustar comportamento para permitir botão mantido pressionado. hoje ele só permite 1 movimento por vez
    static bool key_pressed = false;

    static bool pause_key_pressed = false;

    // Loop principal
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            else if (event.type == SDL_EVENT_GAMEPAD_ADDED && !gamepad_connection) {
                gamepad_connection = SDL_OpenGamepad(event.gdevice.which);
                if (gamepad_connection) {
                    printf("Gamepad conectado: %s\n", SDL_GetGamepadName(gamepad_connection));
                }
            }
            else if (event.type == SDL_EVENT_GAMEPAD_REMOVED && gamepad_connection) {
                SDL_CloseGamepad(gamepad_connection);
                gamepad_connection = NULL;
                printf("Gamepad desconectado.\n");
            }
        }

        PlayerGamePad gamepad = GetPlayerGamePad(gamepad_connection);

        if (gamepad.triggered_pause && !pause_key_pressed)
        {
            if (current_state == STATE_MAP)
            {
                current_state = STATE_PAUSE;
            }
            else if (current_state == STATE_PAUSE)
            {
                current_state = STATE_MAP;
            }
            pause_key_pressed = true;
        }

        // Libera a trava quando solta o botão
        if (!gamepad.triggered_pause)
        {
            pause_key_pressed = false;
        }        

        // --- Atualização do Menu ---
        if (current_state == STATE_MENU)
        {
            if (gamepad.down && !key_pressed)
            {
                menu_opcao = 1; // Seleciona "Continuar"
                key_pressed = true;
            }
            else if (gamepad.up && !key_pressed)
            {
                menu_opcao = 0; // Seleciona "Novo Jogo"
                key_pressed = true;
            }

            if (!gamepad.down && !gamepad.up)
            {
                key_pressed = false;
            }

            if (gamepad.triggered_confirm)
            {
                if (menu_opcao == 0)
                {
                    current_state = STATE_MAP;
                }
            }
        }

        // --- Renderização ---
        // 1. Limpa a tela com a cor de fundo (Escuro)
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        // 2. Desenha o estado atual
        if (current_state == STATE_MENU) {
            // Opção 1: NOVO JOGO
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, 200, "NOVO JOGO", menu_opcao == 0, false);

            // Opção 2: CONTINUAR (DESABILITADO)
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, 280.0f, "CONTINUAR", menu_opcao == 1, true);
        }
        else if (current_state == STATE_MAP || current_state == STATE_PAUSE) {            

            // Executa movimento único quando a tecla é pressionada
            if ((gamepad.delta_x != 0 || gamepad.delta_y != 0) && !key_pressed && current_state == STATE_MAP)
            {
                MovePlayer(&player, &current_state, &enemy, gamepad.delta_x, gamepad.delta_y, FIRST_MAP);
                key_pressed = true; // Trava para não andar continuamente
            }

            // Libera a trava quando solta o botão
            if (gamepad.delta_x == 0 && gamepad.delta_y == 0)
            {
                key_pressed = false;
            }

            RenderMap(renderer, FIRST_MAP, MAP_HEIGHT, MAP_WIDTH, TILE_SIZE);

            // Desenha o jogador (quadrado azul no centro por enquanto)
            SDL_FRect playerRect = PlayerToRect(player, TILE_SIZE);
            SDL_SetRenderDrawColor(renderer, 0, 150, 255, 255);            
            SDL_RenderFillRect(renderer, &playerRect);
            if (current_state == STATE_PAUSE)
            {
                RenderPauseOverlay(renderer, &my_vpet, SCREEN_WIDTH, SCREEN_HEIGHT);
            }
        } else if(current_state == STATE_BATTLE) {

            if (gamepad.right && battle_menu_option == BATLLE_MENU_ATTACK)
            {
                battle_menu_option = BATLLE_MENU_RUN;
            } else if (gamepad.left && battle_menu_option == BATLLE_MENU_RUN)
            {
                battle_menu_option = BATLLE_MENU_ATTACK;
            }

            RenderBattleScreen(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, &my_vpet, &enemy, battle_menu_option);
        }

        // 3. Apresenta o buffer renderizado na janela (ESSENCIAL!)
        SDL_RenderPresent(renderer);

        // 4. Pausa de ~16ms para cravar em 60 FPS e liberar a CPU
        SDL_Delay(16);
    }

    if (gamepad_connection) {
        SDL_CloseGamepad(gamepad_connection);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}