#ifndef AUDIO_H
#define AUDIO_H

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

typedef struct Musicas {
    ALLEGRO_AUDIO_STREAM* som;
} Musica;

// Carrega um audio
Musica carregar_audio(const char* local) {
    Musica musica;
    musica.som = al_load_audio_stream(local, 4, 1024);
    if (!musica.som)
    {
        printf("Erro ao carregar audio %s", local);
        al_destroy_audio_stream(musica.som);
    }
    
    return musica;
}

#endif

