#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

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

bool LoadMapFromCSV(const char* filepath, int world_map[TOTAL_MAPS][TILED_MAP_HEIGHT][TILED_MAP_WIDTH], int map_code) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de mapa: %s", filepath);
        return false;
    }

    for (int row = 0; row < TILED_MAP_HEIGHT; row++) {
        for (int col = 0; col < TILED_MAP_WIDTH; col++) {
            if (fscanf(file, "%d,", &world_map[map_code][row][col]) != 1) {
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

void RenderTilemap(SDL_Renderer* renderer, SDL_Texture* tileset_texture, int map_code, Camera cam, int world_map[TOTAL_MAPS][TILED_MAP_HEIGHT][TILED_MAP_WIDTH]) {
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
            int tile_id = world_map[map_code][row][col];
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
    int object_definition[16][3], 
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

    if (current_map_id == 2)
    {
        Coordinates hotel = {5, 2};
        
        // 1. Zera o valor apontado por total_objects
        *total_objects = 0; 
        
        int object_tiles_capacity = 16;
        
        // 2. Se o ponteiro principal estiver NULO, aloca a memória inicial
        if (*object_tiles == NULL) {
            *object_tiles = malloc(object_tiles_capacity * sizeof(Coordinates));
        }

        // 3. Repassa os ponteiros de ponteiro corretamente
        RenderObject(
            renderer,
            tileset_texture, 
            cam,
            hotel,
            OBJECT_HOTEL,
            OBJECT_HOTEL_TILE_COUNT,
            object_tiles,    // Já é Coordinates**, passa direto
            total_objects    // Já é int*, passa direto
        );
    }
}