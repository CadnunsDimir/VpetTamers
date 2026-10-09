#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

static const MapPortal MAPS_PORTALS[] = {
    {.origin_map_id = 1, .origin_pos = {.x = 19, .y = 10}, .dest_map_id = 2, .dest_pos = {.x = 1, .y = 6}},
    {.origin_map_id = 1, .origin_pos = {.x = 19, .y = 11}, .dest_map_id = 2, .dest_pos = {.x = 1, .y = 7}},
    {.origin_map_id = 2, .origin_pos = {.x = 0, .y = 6}, .dest_map_id = 1, .dest_pos = {.x = 18, .y = 10}},
    {.origin_map_id = 2, .origin_pos = {.x = 0, .y = 7}, .dest_map_id = 1, .dest_pos = {.x = 18, .y = 11}},
    {.origin_map_id = 2, .origin_pos = {.x = 11, .y = 14}, .dest_map_id = 3, .dest_pos = {.x = 2, .y = 1}},
    {.origin_map_id = 2, .origin_pos = {.x = 12, .y = 14}, .dest_map_id = 3, .dest_pos = {.x = 3, .y = 1}},
    {.origin_map_id = 2, .origin_pos = {.x = 6, .y = 5}, .dest_map_id = 4, .dest_pos = {.x = 7, .y = 9}},
    {.origin_map_id = 3, .origin_pos = {.x = 2, .y = 0}, .dest_map_id = 2, .dest_pos = {.x = 11, .y = 13}},
    {.origin_map_id = 3, .origin_pos = {.x = 3, .y = 0}, .dest_map_id = 2, .dest_pos = {.x = 12, .y = 13}},
    {.origin_map_id = 4, .origin_pos = {.x = 7, .y = 10}, .dest_map_id = 2, .dest_pos = {.x = 6, .y = 6}},
};

#define MAPS_PORTALS_COUNT (sizeof(MAPS_PORTALS) / sizeof(MAPS_PORTALS[0]))

bool LoadTileset(SDL_Renderer* renderer, SDL_Texture** tileset_texture, const char* filepath) {
    // Carrega o PNG do tileset diretamente como textura na GPU
    *tileset_texture = IMG_LoadTexture(renderer, filepath);

    if (!(*tileset_texture)) {
        SDL_Log("Erro ao carregar o tileset %s: %s", filepath, SDL_GetError());
        return false;
    }

    // Define o filtro para Nearest (essencial para Pixel Art 16x16 nao ficar desfocado)
    SDL_SetTextureScaleMode(*tileset_texture, SDL_SCALEMODE_NEAREST);

    SDL_Log("Tileset %s carregado com sucesso!", filepath);
    return true;
}

bool LoadMapFromCSV(const char* filepath, int world_map[TOTAL_MAPS_COUNT][TILED_MAP_HEIGHT][TILED_MAP_WIDTH], int map_code) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de mapa: %s\n", filepath);
        return false;
    }

    for (size_t row = 0; row < TILED_MAP_HEIGHT; row++) {
        for (size_t col = 0; col < TILED_MAP_WIDTH; col++) {
            if (fscanf(file, "%d,", &world_map[map_code-1][row][col]) != 1) {
                // Se o arquivo acabou antes do esperado ou falhou
                if (feof(file)) {
                    fclose(file);
                    return true;
                }
            }
        }
    }

    fclose(file);
    return true;
}

void UpdateMap(MapState *map) {
    memcpy(map->current_map, map->world_map[map->current_map_id-1], sizeof(map->current_map));
    if (map->spawned_mobs) { free(map->spawned_mobs); map->spawned_mobs = NULL; }
    map->spawn_enemies_limit = SPAWN_ENEMIES_LIMIT_PER_MAP[map->current_map_id - 1];
    map->spawned_mobs = SpawnMobs(map);
}

void RenderTilemap(SDL_Renderer* renderer, SDL_Texture* tileset_texture, Camera cam, int map[TILED_MAP_HEIGHT][TILED_MAP_WIDTH]) {
    if (!tileset_texture) return;

    int tile_size = 32;       // Mantido o seu tamanho
    int tileset_columns = 4; // Mantido o seu número de colunas
    float draw_scale = 2.0f; 
    float scaled_tile = tile_size * draw_scale;

    // Culling (desenha apenas o que cabe na tela)
    int start_col = (int)(cam.x / scaled_tile);
    int end_col = (int)((cam.x + cam.width) / scaled_tile) + 1;
    int start_row = (int)(cam.y / scaled_tile);
    int end_row = (int)((cam.y + cam.height) / scaled_tile) + 1;

    if (start_col < 0) start_col = 0;
    if (end_col > TILED_MAP_WIDTH) end_col = TILED_MAP_WIDTH;
    if (start_row < 0) start_row = 0;
    if (end_row > TILED_MAP_HEIGHT) end_row = TILED_MAP_HEIGHT;

    for (int row = start_row; row < end_row; row++) {
        for (int col = start_col; col < end_col; col++) {
            int tile_id = map[row][col];
            if (tile_id < 0) continue;

            int tile_col = tile_id % tileset_columns;
            int tile_row = tile_id / tileset_columns;

            float src_x = (float)(tile_col * tile_size);
            float src_y = (float)(tile_row * tile_size);

            SDL_FRect src_rect = { src_x, src_y, (float)tile_size, (float)tile_size };

            // Subtrai cam.x e cam.y para a câmera funcionar
            SDL_FRect dst_rect = {
                (float)(col * scaled_tile) - cam.x,
                (float)(row * scaled_tile) - cam.y,
                scaled_tile,
                scaled_tile
            };

            SDL_RenderTexture(renderer, tileset_texture, &src_rect, &dst_rect);
        }
    }
}

void RenderObject(
    SDL_Renderer* renderer, 
    SDL_Texture* tileset_texture, 
    Camera cam, 
    Coordinates coordinates, 
    const int object_definition[16][3], 
    int object_tiles_count, 
    Coordinates** objects_pointer, // Alterado para Coordinates** para atualizar o ponteiro externo
    int* objects_pointer_size
) {
    if (!tileset_texture || !objects_pointer || !objects_pointer_size) return;

    int tile_size = 32;       
    int tileset_columns = 4; 
    float draw_scale = 2.0f; 
    float scaled_tile = tile_size * draw_scale;

    // 1. Garante uma capacidade inicial mínima maior que 0 caso o tamanho atual seja 0
    int capacidade = (*objects_pointer_size == 0) ? 16 : (*objects_pointer_size);

    int pos = *objects_pointer_size;

    for (int i = 0; i < object_tiles_count; i++)
    {
        // 2. CORREÇÃO: Desreferencia (*objects_pointer_size) em vez de usar o ponteiro cru
        if (pos >= capacidade) {
            capacidade *= 2;
            
            // 3. Usa realloc no ponteiro real trazido de fora
            Coordinates* temp = realloc(*objects_pointer, capacidade * sizeof(Coordinates));
            if (!temp) {
                printf("Falha ao realocar memoria no indice %d\n", pos);
                break;
            }
            *objects_pointer = temp; // Atualiza o ponteiro principal com o novo endereço
        }

        int tile_id = object_definition[i][0];
        float relative_x = object_definition[i][1];
        float relative_y = object_definition[i][2];
        float real_x = coordinates.x + relative_x;
        float real_y = coordinates.y + relative_y;

        Coordinates obj_coordinates = {
            .x = real_x,
            .y = real_y
        };

        // printf("RenderObject | real coordinates | x = %d, y = %d\n", obj_coordinates.x, obj_coordinates.y);

        // Adiciona a coordenada ao array alocado
        (*objects_pointer)[pos] = obj_coordinates;
        pos += 1;        

        // --- RENDERIZAÇÃO ---
        int tile_col = tile_id % tileset_columns;
        int tile_row = tile_id / tileset_columns;

        float src_x = (float)(tile_col * tile_size);
        float src_y = (float)(tile_row * tile_size);

        SDL_FRect src_rect = { src_x, src_y, (float)tile_size, (float)tile_size };

        SDL_FRect dst_rect = {
            (float)(real_x * scaled_tile) - cam.x,
            (float)(real_y * scaled_tile) - cam.y,
            scaled_tile,
            scaled_tile
        };

        SDL_RenderTexture(renderer, tileset_texture, &src_rect, &dst_rect);
    }

    // Atualiza a quantidade real de objetos ocupados
    *objects_pointer_size = pos;
}

void RenderObjectOnMap(
    SDL_Renderer* renderer, 
    SDL_Texture* tileset_texture,
    Camera cam,
    Coordinates** object_tiles, // Ponteiro para o ponteiro da main
    int current_map_id,
    int* total_objects          // Ponteiro para a contagem da main
){
    if (!object_tiles || !total_objects) return;

    // 1. Zera a contagem de objetos
    *total_objects = 0;

    // 2. Calcula quantos tiles no total o mapa atual precisa alocar
    int total_needed_tiles = 0;
    for (size_t i = 0; i < ALL_OBJECTS_ON_MAPS_COUNT; i++) {
        if (ALL_OBJECTS_ON_MAPS[i].map_id == current_map_id) {
            total_needed_tiles += ALL_OBJECTS_ON_MAPS[i].title_count; // (Atentar ao typo 'title_count' vs 'tile_count')
        }
    }

    // 3. Se o mapa atual não tem objetos, libera a memória anterior e encerra
    if (total_needed_tiles == 0) {
        if (*object_tiles != NULL) {
            free(*object_tiles);
            *object_tiles = NULL;
        }
        return;
    }

    // 4. Redimensiona o array para caber TODOS os objetos do mapa ativo de uma só vez
    Coordinates* temp = realloc(*object_tiles, total_needed_tiles * sizeof(Coordinates));
    if (!temp) {
        printf("Erro ao realocar memoria para os objetos do mapa %d\n", current_map_id);
        return;
    }
    *object_tiles = temp;

    // 5. Renderiza cada objeto. O RenderObject vai preencher o array sequencialmente
    //    conforme o *total_objects for sendo incrementado (de 0 até total_needed_tiles).
    for (size_t i = 0; i < ALL_OBJECTS_ON_MAPS_COUNT; i++)
    {
        if (ALL_OBJECTS_ON_MAPS[i].map_id == current_map_id)
        {        
            RenderObject(
                renderer,
                tileset_texture, 
                cam,
                ALL_OBJECTS_ON_MAPS[i].pos,
                ALL_OBJECTS_ON_MAPS[i].tiles,
                ALL_OBJECTS_ON_MAPS[i].title_count,
                object_tiles,    // Já é Coordinates**, passa direto
                total_objects    // O RenderObject vai inserindo a partir de *total_objects
            );
        }        
    }    
}

void CheckMapPortals(int *current_map_id, Coordinates *player_coordinates, bool *changed_map){
    if (!current_map_id || !player_coordinates || !changed_map) return;

    for (size_t i = 0; i < MAPS_PORTALS_COUNT; i++)
    {
        if (
            *current_map_id == MAPS_PORTALS[i].origin_map_id &&  // Adicionado '*' aqui
            player_coordinates->x == MAPS_PORTALS[i].origin_pos.x && 
            player_coordinates->y == MAPS_PORTALS[i].origin_pos.y)
        {
            *current_map_id = MAPS_PORTALS[i].dest_map_id;
            player_coordinates->x = MAPS_PORTALS[i].dest_pos.x;
            player_coordinates->y = MAPS_PORTALS[i].dest_pos.y;
            *changed_map = true;
            break; // Boa prática: sai do loop assim que encontrar o portal correspondente
        }
    }
}