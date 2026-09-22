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
        SDL_Log("Erro ao abrir o arquivo de mapa: %s", filepath);
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