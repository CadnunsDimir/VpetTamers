void ExplorationMode(SDL_Renderer* renderer, BattleSystem *battle_system, PlayerAnimation *player, GameStateMachine *state_machine, PlayerGamePad gamepad, MapState *map, MenuState* menu)
{
    Uint64 current_time = SDL_GetTicks();
    UpdateDeltaAndCurrentTime(&state_machine->app_time, &state_machine->delta_time);

    if (battle_system->player.hp == 0)
    {
        battle_system->player.hp = 1;
        if (battle_system->player.level < 3)
        {
            ShowAlert(&state_machine->alert, "Seu Vpet esta esgotado! Ele precisa descansar em um hotel.", MESSAGE_TYPE_WARN);
        }
    }

    player->moving = false;
    if (state_machine->state == STATE_MAP)
    {

        UpdatePlayerDirection(player, gamepad, state_machine->delta_time);

        if (player->moving)
        {
            if (current_time - state_machine->last_step_time >= STEP_COOLDOWN_MS)
            {
                MovePlayer(
                    &map->player_coordinates,
                    &state_machine->state,
                    &battle_system->enemy,
                    &map->enemy_coordinates,
                    gamepad.delta_x, gamepad.delta_y,
                    map->current_map, 
                    map->spawned_mobs, map->spawned_mobs_size,
                    map->object_tiles, map->object_tiles_total);
                state_machine->last_step_time = current_time;
                PlaySFX(SFX_STEPS);
            }
        }
        else
        {
            state_machine->last_step_time = 0;
        }

        float player_height = SCALED_TILED_SIZE;
        float player_width = SCALED_TILED_SIZE;

        UpdateCamera(&map->camera, map->player_coordinates, player_width, player_height, map->width_tiles, map->height_tiles, SCALED_TILED_SIZE);

        if (player->moving && map->current_map_id == 4 && map->player_coordinates.x == 9 && map->player_coordinates.y == 5)
        {
            state_machine->state = STATE_STORE_MENU;
            menu->store_type = STORE_TYPE_HOTEL;
        }
    }

    if (player->sleeping)
    {
        if (current_time - state_machine->sleep_start_time >= HOTEL_SLEEPING_MSG_TIMEOUT_MS)
        {
            state_machine->state = STATE_MAP;
            player->sleeping = false;
        }
    }

    RenderTilemap(renderer, map->tileset_texture, map->camera, map->current_map);
    RenderObjectOnMap(renderer, map->tileset_texture, map->camera, &map->object_tiles, map->current_map_id, &map->object_tiles_total);
    // RenderPlayerUsingCode(renderer, player_coordinates, main_camera, SCALED_TILED_SIZE, player->moving);
    RenderEnemies(renderer, map->camera, map->spawned_mobs, map->spawned_mobs_size, SCALED_TILED_SIZE);
    RenderPlayerUsingSprites(renderer, player, map->player_coordinates, map->camera);

    if (state_machine->state == STATE_PAUSE)
    {
        RenderPauseOverlay(renderer, &battle_system->player, battle_system->player_vpetcoin_balance, SCREEN_WIDTH, SCREEN_HEIGHT);
    }

    if (state_machine->state == STATE_STORE_MENU && menu->store_type == STORE_TYPE_HOTEL)
    {
        if (player->sleeping)
        {
            ShowHotelSleepingMessage(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
        }
        else
        {
            RenderHotelMenu(renderer, menu->selected_option, SCREEN_WIDTH, SCREEN_HEIGHT, !menu->hotel_free_on_early_game, HOTEL_VPETCOIN_PRICE);
        }
    }

    bool changed_map = false;

    CheckMapPortals(&map->current_map_id, &map->player_coordinates, &changed_map);

    if (changed_map)
    {        
        UpdateMap(map);
        state_machine->pending_save_game = true;
    }
}