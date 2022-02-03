#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

class lgAudio
{
public:
    lgAudio();
    void cargarAudio(const char* Ruta);
    void playAudio(const int audio) const;
    private:
    std::vector<Mix_Chunk*> lgAudioBank;
};