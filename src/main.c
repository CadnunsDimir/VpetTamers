#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include "game.h"
#include "functions.h"
#include "maps.h"
#include "battle_mode.h"
#include "game_logo.h"

int main(int argc, char* argv[]) {
    // 1. Inicializa o SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // 2. Cria Janela e Renderizador juntos no SDL3
    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* game_logo = NULL;
    SDL_Texture* tileset_texture = NULL;

    if (!SDL_CreateWindowAndRenderer("VPet Tamers", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        printf("Erro ao criar janela/renderizador: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    //habilita Blend para criar transparencias
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!LoadAssets(renderer, &game_logo)) {
        printf("Erro ao carregar logo");
    }

    if(!LoadTileset(renderer, &tileset_texture, "assets/tile2map32.png")){
        printf("Erro ler tileset: %s\n", SDL_GetError());
        return 1;
    }
    if(!LoadMapFromCSV("assets/Map1.csv", 1)) {
        printf("Erro ao carregar mapa 1");
        return 1;
    }

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
    Coordinates player_coordinates = {
        FIRST_MAP_SPAWN_POINT[0],
        FIRST_MAP_SPAWN_POINT[1]
    };

    Coordinates enemy_coordinates = {
        0,
        0
    };

    bool running = true;
    SDL_Event event;
    GameState current_state = STATE_MENU;

    // Variável de controle da opção selecionada (0 = Novo Jogo, 1 = Continuar)
    int menu_opcao = 0;
    VPet initial_vpet = {
        .name = "BkMbaracajamon",
        .level = 1,
        .hp = 20,
        .hp_max = 20,
        .strength = 5,
        .defense = 3,
        .type = PET_BK_MBARACAJAMON
    };

    BattleSystem battle_system = {
        .current_state = BATTLE_STATE_MENU_ATTACK,
        .player =  initial_vpet
    };

    // Trava simples para evitar navegação ultra-rápida (pode usar eventos ou flag de liberação)
    //TODO: ajustar comportamento para permitir botão mantido pressionado. hoje ele só permite 1 movimento por vez
    static bool key_pressed = false;

    static bool pause_key_pressed = false;

    int current_map[TILED_MAP_HEIGHT][TILED_MAP_WIDTH];
    
    int current_map_id = 1;
    memcpy(current_map, WORLD_MAP[current_map_id], sizeof(current_map));

    Camera main_camera = {0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT};

    int spawned_mobs_size = 0.0f;
    MapMob* spawned_mobs = SpawnMobs(&spawned_mobs_size, 10, TILED_MAP_WIDTH, TILED_MAP_HEIGHT);

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
            RenderMenuLogo(renderer, game_logo, SCREEN_WIDTH, 40.0f);
            // Opção 1: NOVO JOGO
            float top = 320.0f;
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, top, "NOVO JOGO", menu_opcao == 0, false);

            // Opção 2: CONTINUAR (DESABILITADO)
            float spacing = 80.0f;
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, top + spacing, "CONTINUAR", menu_opcao == 1, true);
        }
        else if (current_state == STATE_MAP || current_state == STATE_PAUSE) {            
            
            // Executa movimento único quando a tecla é pressionada
            if (current_state == STATE_MAP) {
                if ((gamepad.delta_x != 0 || gamepad.delta_y != 0) && !key_pressed) {
                    MovePlayer(&player_coordinates, &current_state, &battle_system.enemy, &enemy_coordinates, gamepad.delta_x, gamepad.delta_y, current_map, spawned_mobs, spawned_mobs_size);
                    key_pressed = true; // Trava para não andar continuamente
                }

                // Libera a trava quando solta o botão
                if (gamepad.delta_x == 0 && gamepad.delta_y == 0) {
                    key_pressed = false;
                }

                // Atualiza a câmera apenas quando estiver no mapa ativo
                UpdateCamera(&main_camera, player_coordinates, 32, 32, TILED_MAP_WIDTH, TILED_MAP_HEIGHT);
            }

            // RenderTilemap(renderer, tileset_texture, current_map_id, main_camera);
            RenderTilemap(renderer, tileset_texture, current_map_id);
            RenderPlayer(renderer, player_coordinates, main_camera);
            RenderEnemies(renderer, spawned_mobs, spawned_mobs_size);

            if (current_state == STATE_PAUSE)
            {
                RenderPauseOverlay(renderer, &battle_system.player, SCREEN_WIDTH, SCREEN_HEIGHT);
            }
        } else if(current_state == STATE_BATTLE) {
            UpdateNavigation(&battle_system, &gamepad);
            UpdateBattleState(&battle_system, &current_state);

            if (battle_system.current_state == BATTLE_STATE_WIN)
            {
                RemoveEnemyFromMap(current_map, &enemy_coordinates, spawned_mobs, spawned_mobs_size);
            }

            RenderBattleScreen(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, battle_system);
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