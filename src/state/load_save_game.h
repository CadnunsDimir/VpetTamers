#include <stdio.h>
#include <stdbool.h>

bool SaveGame(const char* filename, Coordinates player_pos, int current_map_id, VPet vpet) {
    FILE* file = fopen(filename, "wb"); // "wb" = write binary
    if (!file) {
        printf("Erro ao abrir arquivo para salvar!\n");
        return false;
    }

    SaveData data = {
        .player_pos = player_pos,
        .current_map_id = current_map_id,        
        .vpt = vpet
    };

    // Grava a struct inteira de uma só vez
    size_t written = fwrite(&data, sizeof(SaveData), 1, file);
    fclose(file);

    if (written == 1) {
        printf("Sessao salva com sucesso em '%s'!\n", filename);
        return true;
    }

    return false;
}

bool LoadGame(const char* filename, Coordinates* player_pos, int* current_map_id, VPet* vpet) {
    FILE* file = fopen(filename, "rb"); // "rb" = read binary
    if (!file) {
        printf("Nenhum save encontrado em '%s'.\n", filename);
        return false;
    }

    SaveData data;
    size_t read_count = fread(&data, sizeof(SaveData), 1, file);
    fclose(file);

    if (read_count == 1) {
        // Restaura os valores para o jogo
        *player_pos = data.player_pos;
        *current_map_id = data.current_map_id;
        *vpet = data.vpt;

        printf("Sessao carregada com sucesso!\n");
        return true;
    }

    return false;
}