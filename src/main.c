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
#include "sound/songs.h"
#include "sound/init.h"
#include "menus/game_logo.h"
#include "menus/home_menu.h"
#include "menus/pause.h"
#include "menus/hotel.h"
#include "menus/alert.h"
#include "player_animation.h"
#include "exploration_mode.h"

int main(int argc, char* argv[]) {
    // Inicializa a semente do gerador de números aleatórios com o tempo atual
    srand((unsigned int)time(NULL));

    // 1. Inicializa o SDL3
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD)) {
        printf("Erro ao inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    //inicializa o audio
    if (!InitAudio())
    {
        printf("[InitAudio] Erro ao inicilizar o audio\n ");
        goto cleanup;
    } 

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;
    SDL_Texture* game_logo = NULL;
    SDL_Texture* tileset_texture = NULL;
    SDL_Gamepad* gamepad_connection = NULL;
    SDL_Surface *main_character_surface;

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

    if(!LoadPlayerSurface(&main_character_surface)){
        goto cleanup;
    }
    
    if (!LoadMenuAudio(SONGS_MAIN_THEME))
    {
        printf("Erro ao carregar musica do menu\n");
        goto cleanup;
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
    
    SDL_Event event;

    GameStateMachine state_machine = {
        .running = true,
        .state = STATE_MENU,
        .app_time = 0,
        .delta_time = 0,
        .last_step_time = 0,
        .sleep_start_time = 0,
        .save_data_file = "save.dat",

        .alert = {
            .duration = 0,
            .start_time_ms = 0
        }
    };

    MapState map_state = {
        .tileset_texture = tileset_texture,
        .world_map = world_map,
        .player_coordinates = { 5, 5 }, 
        .enemy_coordinates = { 0, 0 },
        .camera = {0.0f, 0.0f, SCREEN_WIDTH, SCREEN_HEIGHT},
        .spawned_mobs_size = 0,
        .width_tiles = TILED_MAP_WIDTH, 
        .height_tiles = TILED_MAP_HEIGHT
    };

    MenuState menu = {
        .selected_option = 0,
        .store_type = STORE_TYPE_HOTEL
    };

    PlayerAnimation player = {
        .texture = NULL,
        .frame = 0,
        .frame_timer = 0.0f,
        .frame_duration = 0.12f,
        .direction = DIRECTION_DOWN,
        .moving = false,
        .sleeping = false
    };

    ApplyTexture(renderer, main_character_surface, &player);
    if (player.texture == NULL) {
        printf("[ERRO] Textura do jogador está NULL! Verifique o caminho do arquivo.\n");
        goto cleanup;
    }

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

    PlayerGamePad gamepad = {0, 0, false, false, false, false, false, false, false, false};

    PlayMenuMusic();

    // Loop principal
    while (state_machine.running) {
        UpdateAudioLoop();
        
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                state_machine.running = false;
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

        UpdatePlayerGamePad(gamepad_connection, &gamepad);

        if (gamepad.triggered_pause && !gamepad.pause_key_pressed) {
            if (state_machine.state == STATE_MAP) {
                state_machine.state = STATE_PAUSE;
            }
            else if (state_machine.state == STATE_PAUSE) {
                state_machine.state = STATE_MAP;
            }
           gamepad.pause_key_pressed = true;
        }

        if (!gamepad.triggered_pause) {
           gamepad.pause_key_pressed = false;
        }        

        // --- Atualização do Menu ---
        if (state_machine.state == STATE_MENU) {
            if (gamepad.down && !gamepad.key_pressed) {
                menu.selected_option = 1;
                gamepad.key_pressed = true;                
            }
            else if (gamepad.up && !gamepad.key_pressed) {
                menu.selected_option = 0;
                gamepad.key_pressed = true;
            }

            if (!gamepad.down && !gamepad.up) {
                gamepad.key_pressed = false;
            }

            if (gamepad.triggered_confirm) {
                if (menu.selected_option == 0) {
                    map_state.current_map_id = 1;
                    state_machine.state = STATE_MAP;
                } else if (menu.selected_option == 1) {
                    LoadGame(
                        state_machine.save_data_file, 
                        &map_state.player_coordinates, 
                        &map_state.current_map_id,
                        &battle_system.player,
                        &battle_system.player_vpetcoin_balance
                    );

                    menu.selected_option = 0;
                }

                UpdateMap(&map_state);
                state_machine.state = STATE_MAP;
            }
        }

        menu.hotel_free_on_early_game = battle_system.player_vpetcoin_balance < HOTEL_VPETCOIN_PRICE && battle_system.player.level < 3;

        if(state_machine.state == STATE_STORE_MENU && menu.store_type == STORE_TYPE_HOTEL) {
            if (gamepad.right && menu.selected_option == 0) {
                menu.selected_option = 1;
                gamepad.pause_key_pressed = true;               
            }
            else if (gamepad.left && menu.selected_option == 1) {
                menu.selected_option = 0;
            } else if(gamepad.triggered_confirm) {
                Uint64 current_time = SDL_GetTicks();
                bool showing_alert = current_time <= (state_machine.alert.start_time_ms + state_machine.alert.duration);
                bool have_enough_money = battle_system.player_vpetcoin_balance >= HOTEL_VPETCOIN_PRICE;
                
                if(menu.selected_option == 0 && (have_enough_money || menu.hotel_free_on_early_game)) {
                    battle_system.player.hp = battle_system.player.hp_max;
                    if(menu.hotel_free_on_early_game) {
                        char message[100];
                        snprintf(message, sizeof(message), "Agora Voce pode se hospedar gratuitamente mas no futuro custara %.1f", HOTEL_VPETCOIN_PRICE);
                        ShowAlert(&state_machine.alert, message, MESSAGE_TYPE_WARN);
                    } else {
                        battle_system.player_vpetcoin_balance -= HOTEL_VPETCOIN_PRICE;
                    }                    
                    map_state.player_coordinates.x = 13;
                    map_state.player_coordinates.y = 9;
                    SaveGame(
                        state_machine.save_data_file,
                        map_state.player_coordinates,
                        map_state.current_map_id,
                        battle_system.player,
                        battle_system.player_vpetcoin_balance);
                    state_machine.sleep_start_time = current_time;
                    player.sleeping = true;
                    player.direction = DIRECTION_UP;                                      
                } else if(menu.selected_option == 0 && !have_enough_money) {
                    if(!showing_alert) {
                        char message[50];
                        snprintf(message, sizeof(message), "Saldo insuficiente! Voce tem %.1f VpetCoins", battle_system.player_vpetcoin_balance);
                        ShowAlert(&state_machine.alert, message, MESSAGE_TYPE_ERROR);
                    }                   
                } else {
                    state_machine.state = STATE_MAP;
                }
            }
        }

        // --- Renderização ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
        SDL_RenderClear(renderer);

        if (state_machine.state == STATE_MENU) {
            HomeMenu(renderer, game_logo, menu.selected_option);            
        }
        else if (state_machine.state == STATE_MAP || state_machine.state == STATE_PAUSE || state_machine.state == STATE_STORE_MENU) {   
            ExplorationMode(renderer, &battle_system, &player, &state_machine, gamepad, &map_state, &menu);
        } 
        else if(state_machine.state == STATE_BATTLE) {

            UpdateNavigation(&battle_system, &gamepad);
            UpdateBattleState(&battle_system, &state_machine, &map_state);

            if (battle_system.current_state == BATTLE_STATE_WIN) {
                RemoveEnemyFromMap(&map_state);
            }

            RenderBattleScreen(renderer, SCREEN_WIDTH, SCREEN_HEIGHT, battle_system);
        } else {
            printf("state_machine.state desconhecido: %d", state_machine.state);
        }

        RenderAlert(renderer, state_machine.alert, SCREEN_WIDTH);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

printf("Finalizando programa...\n");
cleanup:
    printf("liberando recursos\n");
    CleanupAudio();
    if (main_character_surface) SDL_DestroySurface(main_character_surface);
    if (map_state.spawned_mobs) free(map_state.spawned_mobs);
    if (game_logo) SDL_DestroyTexture(game_logo);
    if (map_state.tileset_texture) SDL_DestroyTexture(map_state.tileset_texture);
    if (gamepad_connection) SDL_CloseGamepad(gamepad_connection);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);    
    SDL_Quit();

    printf("FIM.\n");    
    exit(0); // Força a liberação da thread do OS caso o SDL3 mantenha subsistemas abertos
}