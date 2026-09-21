#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>

    // 0 = Chão (cor azul-escuro/cinza), 1 = Parede (cor cinza-claro/vermelho), ou codigo dos vpets inimigos
    static const int FIRST_MAP[MAP_HEIGHT][MAP_WIDTH] = {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 2, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };

    static const int FIRST_MAP_SPAWN_POINT[2] = {1, 1};


static int WORLD_MAP[TOTAL_MAPS][TILED_MAP_HEIGHT][TILED_MAP_WIDTH];

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

bool LoadMapFromCSV(const char* filepath, int map_code) {
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo de mapa: %s\n", filepath);
        return false;
    }

    for (int row = 0; row < TILED_MAP_HEIGHT; row++) {
        for (int col = 0; col < TILED_MAP_WIDTH; col++) {
            // Le o numero do tile e ignora a virgula
            if (fscanf(file, "%d,", &WORLD_MAP[map_code][row][col]) != 1) {
                // Caso chegue ao fim da linha sem ler
            }
        }
    }

    fclose(file);
    return true;
}

void RenderTilemap(SDL_Renderer* renderer, SDL_Texture* tileset_texture, int map_code) {
    if (!tileset_texture) return;

    int tile_size = 32;       // Tamanho de cada tile
    int tileset_columns = 4; // Numero de colunas no PNG do tileset

    for (int row = 0; row < TILED_MAP_HEIGHT; row++) {
        for (int col = 0; col < TILED_MAP_WIDTH; col++) {
            int tile_id = WORLD_MAP[map_code][row][col];

            // Se o tile_id for -1 (vazio no Tiled), ignora
            if (tile_id < 0) continue;

            // Calcula a coluna e linha do tile na imagem PNG
            int tile_col = tile_id % tileset_columns;
            int tile_row = tile_id / tileset_columns;

            // Leva em consideracao a Margem (1px) e o Espacamento (2px)
            float src_x = (float)(tile_col * tile_size);
            float src_y = (float)(tile_row * tile_size);

            SDL_FRect src_rect = { src_x, src_y, (float)tile_size, (float)tile_size };

            // Onde desenhar na tela (podemos escalar para 32x32 para ficar maior)
            float draw_scale = 2.0f; // Escala para pixel art retro
            SDL_FRect dst_rect = {
                (float)(col * tile_size * draw_scale),
                (float)(row * tile_size * draw_scale),
                (float)(tile_size * draw_scale),
                (float)(tile_size * draw_scale)
            };

            SDL_RenderTexture(renderer, tileset_texture, &src_rect, &dst_rect);
        }
    }
}