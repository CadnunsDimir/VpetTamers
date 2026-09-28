#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_image/SDL_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include "game.h"
#include "vpets/vpets.h"
#include "state/load_save_game.h"
#include "functions.h"
#include "objects/objects.h"
#include "maps.h"
#include "battle_mode.h"
#include "game_logo.h"
#include "menus/hotel.h"
#include "menus/alert.h"


int main(int argc, char* argv[]) {
    // Inicializa a semente do gerador de números aleatórios com o tempo atual
    srand((unsigned int)time(NULL));

    // 1. Inicializa o SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* game_logo = NULL;
    SDL_Texture* tileset_texture = NULL;
    MapMob* spawned_mobs = NULL;
    SDL_Gamepad* gamepad_connection = NULL;
    int world_map[TOTAL_MAPS_COUNT][TILED_MAP_HEIGHT][TILED_MAP_WIDTH];

    // 2. Cria Janela e Renderizador
    if (!SDL_CreateWindowAndRenderer("VPet Tamers", SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer)) {
        printf("Erro ao criar janela/renderizador: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    if (!LoadAssets(renderer, &game_logo)) {
        printf("Erro ao carregar logo\n");
    }

    if (!LoadTileset(renderer, &tileset_texture, "assets/tile2map32.png")) {
        printf("Erro ler tileset: %s\n", SDL_GetError());
        goto cleanup; // Usa rótulo de limpeza para não vazar recursos
    }

    

    for (int i = 0; i < TOTAL_MAPS_COUNT; i++) {
        int map_id = i+1;
        printf("Carregando Map %d (%s)...\n", map_id, MAP_FILES[i]);
        if (!LoadMapFromCSV(MAP_FILES[i], world_map, map_id)) {
            printf("Erro ao carregar mapa %d (%s)\n", map_id, MAP_FILES[i]);
            goto cleanup;
        } else{
            printf("Mapa %d (%s) carregado com sucesso!\n", map_id, MAP_FILES[i]);
        }
    }    

    // 3. Conecta Gamepad inicial
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

    Coordinates player_coordinates = { 5, 5 };
    Coordinates enemy_coordinates = { 0, 0 };

    bool running = true;
    SDL_Event event;
    GameState current_state = STATE_MENU;
    int menu_opcao = 0;
    StoreType store_type = STORE_TYPE_HOTEL;
    bool sleeping = false;
    Uint64 sleep_start_time = 0;
    Alert alert = {
        .duration = 0,
        .start_time_ms = 0
    };

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
        .player = initial_vpet,
        .player_vpetcoin_balance = 0
    };

    static bool key_pressed = false;
    static bool pause_key_pressed = false;

    int current_map[TILED_MAP_HEIGHT][TILED_MAP_WIDTH];
    int current_map_id = 1;
    memcpy(current_map, world_map[current_map_id-1], sizeof(current_map));

    Camera main_camera = {0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT};

    int spawned_mobs_size = 0; // Guarda a quantidade total de mobs no map (ativos e não ativos)
    
    static Uint64 last_step_time = 0;

    Coordinates* object_tiles = NULL;
    int total_objects = 0;

    char save_data_file[10] = "save.dat";

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

        if (gamepad.triggered_pause && !pause_key_pressed) {
            if (current_state == STATE_MAP) {
                current_state = STATE_PAUSE;
            }
            else if (current_state == STATE_PAUSE) {
                current_state = STATE_MAP;
            }
            pause_key_pressed = true;
        }

        if (!gamepad.triggered_pause) {
            pause_key_pressed = false;
        }        

        // --- Atualização do Menu ---
        if (current_state == STATE_MENU) {
            if (gamepad.down && !key_pressed) {
                menu_opcao = 1;
                key_pressed = true;                
            }
            else if (gamepad.up && !key_pressed) {
                menu_opcao = 0;
                key_pressed = true;
            }

            if (!gamepad.down && !gamepad.up) {
                key_pressed = false;
            }

            if (gamepad.triggered_confirm) {
                if (menu_opcao == 0) {
                    current_state = STATE_MAP;
                    spawned_mobs = SpawnMobs(&spawned_mobs_size, SPAWN_ENEMIES_LIMIT_PER_MAP[current_map_id-1], current_map, TILED_MAP_WIDTH, TILED_MAP_HEIGHT);

                } else if (menu_opcao == 1) {
                    LoadGame(
                        save_data_file, 
                        &player_coordinates, 
                        &current_map_id,
                        &battle_system.player,
                        &battle_system.player_vpetcoin_balance
                    );
                    memcpy(current_map, world_map[current_map_id-1], sizeof(current_map));
                    if (spawned_mobs) { free(spawned_mobs); spawned_mobs = NULL; }
                    spawned_mobs = SpawnMobs(&spawned_mobs_size, SPAWN_ENEMIES_LIMIT_PER_MAP[current_map_id-1], current_map, TILED_MAP_WIDTH, TILED_MAP_HEIGHT);
                    current_state = STATE_MAP;
                    menu_opcao = 0;
                }
            }
        }

        bool hotel_free_on_early_game = battle_system.player_vpetcoin_balance < HOTEL_VPETCOIN_PRICE && battle_system.player.level < 3;

        if(current_state == STATE_STORE_MENU && store_type == STORE_TYPE_HOTEL) {
            if (gamepad.right && menu_opcao == 0) {
                menu_opcao = 1;
                key_pressed = true;               
            }
            else if (gamepad.left && menu_opcao == 1) {
                menu_opcao = 0;
            } else if(gamepad.triggered_confirm) {
                Uint64 current_time = SDL_GetTicks();
                bool showing_alert = current_time <= (alert.start_time_ms + alert.duration);
                bool have_enough_money = battle_system.player_vpetcoin_balance >= HOTEL_VPETCOIN_PRICE;
                
                if(menu_opcao == 0 && (have_enough_money || hotel_free_on_early_game)) {
                    battle_system.player.hp = battle_system.player.hp_max;
                    if(hotel_free_on_early_game) {
                        char message[100];
                        snprintf(message, sizeof(message), "Agora Voce pode se hospedar gratuitamente mas no futuro custara %.1f", HOTEL_VPETCOIN_PRICE);
                        ShowAlert(&alert, message, MESSAGE_TYPE_WARN);
                    } else {
                        battle_system.player_vpetcoin_balance -= HOTEL_VPETCOIN_PRICE;
                    }                    
                    player_coordinates.x = 13;
                    player_coordinates.y = 9;
                    SaveGame(
                        save_data_file,
                        player_coordinates,
                        current_map_id,
                        battle_system.player,
                        battle_system.player_vpetcoin_balance);
                    sleep_start_time = current_time;
                    sleeping = true;

                                      
                } else if(menu_opcao == 0 && !have_enough_money) {
                    if(!showing_alert) {
                        char message[50];
                        snprintf(message, sizeof(message), "Saldo insuficiente! Voce tem %.1f VpetCoins", battle_system.player_vpetcoin_balance);
                        ShowAlert(&alert, message, MESSAGE_TYPE_ERROR);
                    }                   
                } else {
                    current_state = STATE_MAP;
                }
            }
        }

        // --- Renderização ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        if (current_state == STATE_MENU) {
            RenderMenuLogo(renderer, game_logo, SCREEN_WIDTH, 40.0f);
            float top = 320.0f;
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, top, "NOVO JOGO", menu_opcao == 0, false);
            float spacing = 80.0f;
            RenderMenuButton(renderer, (SCREEN_WIDTH - 300) / 2.0f, top + spacing, "CONTINUAR", menu_opcao == 1, false);
        }
        else if (current_state == STATE_MAP || current_state == STATE_PAUSE || current_state == STATE_STORE_MENU) {   
            Uint64 current_time = SDL_GetTicks();

            if(battle_system.player.hp == 0){                
                battle_system.player.hp = 1;
                if (battle_system.player.level < 3)
                {
                    ShowAlert(&alert, "Seu Vpet esta esgotado! Ele precisa descansar em um hotel.", MESSAGE_TYPE_WARN);   
                }                
            }

            bool is_moving = false;         
            if (current_state == STATE_MAP) {
                is_moving = (gamepad.delta_x != 0 || gamepad.delta_y != 0);
                if (is_moving) {
                    if (current_time - last_step_time >= STEP_COOLDOWN_MS) {
                        MovePlayer(
                            &player_coordinates, 
                            &current_state, 
                            &battle_system.enemy, 
                            &enemy_coordinates, 
                            gamepad.delta_x, gamepad.delta_y, 
                            current_map, spawned_mobs, spawned_mobs_size,
                            object_tiles, total_objects
                        );
                        last_step_time = current_time;
                    }                    
                } else {
                    last_step_time = 0;
                }
                        
                float player_height = SCALED_TILED_SIZE;
                float player_width = SCALED_TILED_SIZE;

                UpdateCamera(&main_camera, player_coordinates, player_width, player_height, TILED_MAP_WIDTH, TILED_MAP_HEIGHT, SCALED_TILED_SIZE);

                if (is_moving && current_map_id == 4 && player_coordinates.x == 9 && player_coordinates.y == 5)
                {
                    current_state = STATE_STORE_MENU;
                    store_type = STORE_TYPE_HOTEL;
                }
            }
            
            if(sleeping) {
                if(current_time - sleep_start_time >= HOTEL_SLEEPING_MSG_TIMEOUT_MS) {
                    current_state = STATE_MAP;
                    sleeping = false;
                }
            }

            RenderTilemap(renderer, tileset_texture, current_map_id, main_camera, world_map);
            RenderObjectOnMap(renderer, tileset_texture, main_camera, &object_tiles, current_map_id, &total_objects);
            RenderPlayer(renderer, player_coordinates, main_camera, SCALED_TILED_SIZE, is_moving);
            RenderEnemies(renderer, main_camera, spawned_mobs, spawned_mobs_size, SCALED_TILED_SIZE);

            if (current_state == STATE_PAUSE) {
                RenderPauseOverlay(renderer, &battle_system.player, battle_system.player_vpetcoin_balance, SCREEN_WIDTH, SCREEN_HEIGHT);
            }

            if(current_state == STATE_STORE_MENU && store_type == STORE_TYPE_HOTEL) {
                if(sleeping) {
                     ShowHotelSleepingMessage(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
                } else {
                    RenderHotelMenu(renderer, menu_opcao, SCREEN_WIDTH, SCREEN_HEIGHT, !hotel_free_on_early_game, HOTEL_VPETCOIN_PRICE);
                }                
            }            

            //TODO: linhas 240-256 Criar função UpdateMapsTransition(player_coordinates, current_map_id, spawned_mobs);
            // printf("player(x=%d, y=%d)\n", player_coordinates.x, player_coordinates.y);
            bool changed_map = false;

            CheckMapPortals(&current_map_id, &player_coordinates, &changed_map);

            if (changed_map)
            {
                total_objects = 0;
                memcpy(current_map, world_map[current_map_id-1], sizeof(current_map));
                if (spawned_mobs) { free(spawned_mobs); spawned_mobs = NULL; }
                spawned_mobs = SpawnMobs(&spawned_mobs_size, SPAWN_ENEMIES_LIMIT_PER_MAP[current_map_id-1], current_map, TILED_MAP_WIDTH, TILED_MAP_HEIGHT);
                SaveGame(
                    save_data_file,
                    player_coordinates,
                    current_map_id,
                    battle_system.player,
                    battle_system.player_vpetcoin_balance
                );
            }
            
        } 
        else if(current_state == STATE_BATTLE) {

            UpdateNavigation(&battle_system, &gamepad);
            UpdateBattleState(&battle_system, &current_state, save_data_file, player_coordinates, current_map_id);

            if (battle_system.current_state == BATTLE_STATE_WIN) {
                RemoveEnemyFromMap(current_map, &enemy_coordinates, spawned_mobs, spawned_mobs_size);
            }

            RenderBattleScreen(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, battle_system);
        } else {
            printf("current_state desconhecido: %d", current_state);
        }

        RenderAlert(renderer, alert, SCREEN_WIDTH);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

printf("Finalizando programa...\n");
cleanup:
    printf("liberando recursos\n");

    if (spawned_mobs) free(spawned_mobs);
    if (game_logo) SDL_DestroyTexture(game_logo);
    if (tileset_texture) SDL_DestroyTexture(tileset_texture);
    if (gamepad_connection) SDL_CloseGamepad(gamepad_connection);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);    
    SDL_Quit();

    printf("FIM.\n");    
    exit(0); // Força a liberação da thread do OS caso o SDL3 mantenha subsistemas abertos
}