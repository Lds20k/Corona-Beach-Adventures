#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

typedef struct Audio {
    ALLEGRO_AUDIO_STREAM* som;
} Audio;

// Carrega um audio
Audio carregar_audio(const char* local) {
    Audio audio;
    audio.som = al_load_audio_stream(local, 4, 1024);

    if (!audio.som)
    {
        printf("Erro ao carregar audio %s", local);
        al_destroy_audio_stream(audio.som);
    }
    
    return audio;
}

#endif