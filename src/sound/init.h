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


static inline void PlayMusic(void) {
    if (audio_stream) {
        SDL_ResumeAudioStreamDevice(audio_stream);
        SDL_Log("[PlayMusic] Tocando o arquivo audio...");
    } else {
        SDL_Log("[PlayMusic] Stream não inicializado");
    }
}

// Carrega uma nova trilha e reinicia a fila do Stream
static inline bool ChangeMusic(const char* asset_filename) {
    char asset_full_path[128];
    snprintf(asset_full_path, sizeof(asset_full_path), "assets/audio/%s", asset_filename);

    // 1. Limpa o buffer de áudio anterior da memória
    if (audio_buf) {
        SDL_free(audio_buf);
        audio_buf = NULL;
    }

    // 2. Limpa os dados restantes que ainda estavam tocando no Stream
    if (audio_stream) {
        SDL_ClearAudioStream(audio_stream);
    }

    // 3. Carrega o novo arquivo WAV
    if (!SDL_LoadWAV(asset_full_path, &audio_spec, &audio_buf, &audio_len)) {
        SDL_Log("[ChangeMusic] Erro ao carregar WAV '%s': %s", asset_full_path, SDL_GetError());
        return false;
    }

    // 4. Se o Stream ainda não existia (primeira execução), cria agora
   if (!audio_stream) {
        audio_stream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, 
            &audio_spec, 
            NULL, 
            NULL
        );

        if (!audio_stream) {
            SDL_Log("[ChangeMusic] Erro ao criar Audio Stream: %s", SDL_GetError());
            SDL_free(audio_buf);
            audio_buf = NULL;
            return false;
        }
    }

    // 5. Alimenta a fila do Stream com a nova música e ativa a reprodução
    SDL_PutAudioStreamData(audio_stream, audio_buf, audio_len);
    SDL_ResumeAudioStreamDevice(audio_stream);

    SDL_Log("[ChangeMusic] Música alterada com sucesso para: %s", asset_filename);
    return true;
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
