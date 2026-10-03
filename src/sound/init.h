#include <SDL3/SDL.h>
#include <stdio.h>

typedef struct {
    float background_volume;
    float sfx_volume;
} SoundSetting;

// Variáveis de estado do áudio
static SDL_AudioSpec audio_spec;
static Uint8 *bg_song_buf = NULL;
static Uint32 audio_len = 0;
static SDL_AudioStream *audio_stream = NULL;
static SDL_AudioStream *sfx_stream = NULL;
static SoundSetting g_sound_settings = {
    .background_volume = 1.0f,
    .sfx_volume = 1.0f
};

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

static inline bool PlaySFX(const char* sfx_filename) {
    char asset_full_path[128];
    snprintf(asset_full_path, sizeof(asset_full_path), "assets/audio/%s", sfx_filename);

    SDL_AudioSpec sfx_spec;
    Uint8 *sfx_buf = NULL;
    Uint32 sfx_len = 0;

    // Carrega o WAV do efeito
    if (!SDL_LoadWAV(asset_full_path, &sfx_spec, &sfx_buf, &sfx_len)) {
        SDL_Log("[PlaySFX] Erro ao carregar SFX '%s': %s", asset_full_path, SDL_GetError());
        return false;
    }

    // Se o stream de SFX ainda não existe, cria um
    if (!sfx_stream) {
        sfx_stream = SDL_OpenAudioDeviceStream(
            SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, 
            &sfx_spec, 
            NULL, 
            NULL
        );

        if (!sfx_stream) {
            SDL_Log("[PlaySFX] Erro ao criar Stream SFX: %s", SDL_GetError());
            SDL_free(sfx_buf);
            return false;
        }
        SDL_ResumeAudioStreamDevice(sfx_stream);
    }

    // Envia o áudio do efeito para o stream de SFX
    SDL_PutAudioStreamData(sfx_stream, sfx_buf, sfx_len);

    // Como o stream já copiou os dados para o buffer da SDL, podemos liberar imediatamente a memória local
    SDL_free(sfx_buf);

    return true;
}

static inline void SetSoundVolume(SoundSetting settings) {
    // Garante que os valores fiquem no intervalo válido [0.0, 1.0]
    g_sound_settings.background_volume = SDL_clamp(settings.background_volume, 0.0f, 1.0f);
    g_sound_settings.sfx_volume        = SDL_clamp(settings.sfx_volume, 0.0f, 1.0f);

    // Aplica o ganho no stream da Música (BGM)
    if (audio_stream) {
        SDL_SetAudioStreamGain(audio_stream, g_sound_settings.background_volume);
    }

    // Aplica o ganho no stream de Efeitos (SFX)
    if (sfx_stream) {
        SDL_SetAudioStreamGain(sfx_stream, g_sound_settings.sfx_volume);
    }
}

// Carrega uma nova trilha e reinicia a fila do Stream
static inline bool ChangeMusic(const char* asset_filename) {
    char asset_full_path[128];
    snprintf(asset_full_path, sizeof(asset_full_path), "assets/audio/%s", asset_filename);

    // 1. Limpa o buffer de áudio anterior da memória
    if (bg_song_buf) {
        SDL_free(bg_song_buf);
        bg_song_buf = NULL;
    }

    // 2. Limpa os dados restantes que ainda estavam tocando no Stream
    if (audio_stream) {
        SDL_ClearAudioStream(audio_stream);
    }

    // 3. Carrega o novo arquivo WAV
    if (!SDL_LoadWAV(asset_full_path, &audio_spec, &bg_song_buf, &audio_len)) {
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
            SDL_free(bg_song_buf);
            bg_song_buf = NULL;
            return false;
        }
    }

    // 5. Alimenta a fila do Stream com a nova música e ativa a reprodução
    SDL_PutAudioStreamData(audio_stream, bg_song_buf, audio_len);
    SDL_ResumeAudioStreamDevice(audio_stream);

    SDL_Log("[ChangeMusic] Música alterada com sucesso para: %s", asset_filename);
    return true;
}


static inline void UpdateAudioLoop(void) {
    if (audio_stream && bg_song_buf) {
        // Se a quantidade de bytes na fila for menor que um limite seguro (ex: 4KB),
        // recolocamos os dados do áudio na fila para criar o loop contínuo.
        if (SDL_GetAudioStreamQueued(audio_stream) < 4096) {
            SDL_Log("[UpdateAudioLoop] reiniciando a musica");
            SDL_PutAudioStreamData(audio_stream, bg_song_buf, audio_len);
        }
    } else {
        SDL_Log("[UpdateAudioLoop] Audio Stream e Buffer ausentes");
    }
}

static inline void StopMusic(void) {
    if (audio_stream) {
        SDL_PauseAudioStreamDevice(audio_stream);
    }
}

static inline void CleanupAudio(void) {
    if (audio_stream) {
        SDL_DestroyAudioStream(audio_stream);
        audio_stream = NULL;
    }

    if (bg_song_buf) {
        SDL_free(bg_song_buf); // Libera a memória ao encerrar o jogo
        bg_song_buf = NULL;
    }

    if (sfx_stream) {
        SDL_DestroyAudioStream(sfx_stream);
        sfx_stream = NULL;
    }
}
