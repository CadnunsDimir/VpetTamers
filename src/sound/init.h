#ifndef SOUND_INIT_H
#define SOUND_INIT_H

#include <SDL3/SDL.h>
#include <stdio.h>

// Variáveis de estado do áudio
static SDL_AudioSpec audio_spec;
static Uint8 *audio_buf = NULL;
static Uint32 audio_len = 0;
static SDL_AudioStream *audio_stream = NULL;

static inline bool InitAudio(void) {
    // 1. Inicializa o subsistema de áudio da SDL3
    if (!SDL_Init(SDL_INIT_AUDIO)) {
        SDL_Log("[InitAudio] Erro ao inicializar SDL Audio: %s", SDL_GetError());
        return false;
    }
    return true;
}

static inline bool LoadMenuAudio(const char* asset_path) {
    char asset_full_path[100];
    snprintf(asset_full_path, sizeof(asset_full_path), "assets/audio/%s", asset_path);
    printf("[LoadMenuAudio] Abrindo audio '%s'\n", asset_full_path);

    // 2. Carrega o ficheiro WAV (passa o endereço da struct e do ponteiro do buffer)
    if (!SDL_LoadWAV(asset_full_path, &audio_spec, &audio_buf, &audio_len)) {
        SDL_Log("Erro ao ler o arquivo '%s': %s", asset_full_path, SDL_GetError());
        return false;
    }

    // 3. Atribui diretamente à variável GLOBAL (sem declarar 'SDL_AudioStream *' aqui dentro)
    audio_stream = SDL_OpenAudioDeviceStream(
        SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, 
        &audio_spec, 
        NULL, 
        NULL
    );

    if (!audio_stream) {
        SDL_Log("[LoadMenuAudio] Erro ao criar o Audio Stream: %s", SDL_GetError());
        SDL_free(audio_buf);
        audio_buf = NULL;
        return false;
    }

    // 4. Envia os dados do WAV para a fila do Stream
    SDL_PutAudioStreamData(audio_stream, audio_buf, audio_len);
    SDL_Log("[LoadMenuAudio] Audio carregado e Stream inicializado com sucesso!");

    return true;
}

static inline void PlayMenuMusic(void) {
    if (audio_stream) {
        SDL_ResumeAudioStreamDevice(audio_stream);
        SDL_Log("[PlayMenuMusic] Tocando o arquivo audio...");
    } else {
        SDL_Log("[PlayMenuMusic] Stream não inicializado");
    }
}

static inline void UpdateAudioLoop(void) {
    if (audio_stream && audio_buf) {
        // Se a quantidade de bytes na fila for menor que um limite seguro (ex: 4KB),
        // recolocamos os dados do áudio na fila para criar o loop contínuo.
        if (SDL_GetAudioStreamQueued(audio_stream) < 4096) {
            SDL_Log("[UpdateAudioLoop] reiniciando a musica");
            SDL_PutAudioStreamData(audio_stream, audio_buf, audio_len);
        }
    } else {
        SDL_Log("[UpdateAudioLoop] Audio Stream e Buffer ausentes");
    }
}

static inline void StopMenuMusic(void) {
    if (audio_stream) {
        SDL_PauseAudioStreamDevice(audio_stream);
    }
}

static inline void CleanupAudio(void) {
    if (audio_stream) {
        SDL_DestroyAudioStream(audio_stream);
        audio_stream = NULL;
    }

    if (audio_buf) {
        SDL_free(audio_buf); // Libera a memória ao encerrar o jogo
        audio_buf = NULL;
    }
}

#endif // SOUND_INIT_H