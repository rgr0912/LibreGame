#include "lgSDLmixer.h"
lgAudio::lgAudio()
{
    std::cout << "iniciando sdl_mixer" << std::endl;
    int audio_rate = 22050;
    Uint16 audio_format = AUDIO_S16SYS;
    int audio_chanenels = 2;
    int audio_buffers = 4096;

    if (Mix_OpenAudio(audio_rate, audio_format, audio_chanenels, audio_buffers) != 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No se puede iniciar audio: %s", Mix_GetError());
        exit(-1);
    }
}
void lgAudio::cargarAudio(const char *audio_ruta)
{
    Mix_Chunk* tmpChunk = Mix_LoadWAV(audio_ruta);

    if (tmpChunk != nullptr)
    {
        lgAudioBank.push_back(tmpChunk);
        std::cout << (lgAudioBank.size() - 1) << " -audio listo : " << audio_ruta << std::endl;
    }
    else
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "No se puede iniciar audio: %s", Mix_GetError());
    }
}
void lgAudio::playAudio(const int audio) const
{
    if (audio > lgAudioBank.size() - 1)
    {
        std::cout << "Sonido fuera de rango" << std::endl;
    }

    Mix_PlayChannel(-1, lgAudioBank[audio], 0);
    std::cout << "Reproduciendo audio" << std::endl;
}