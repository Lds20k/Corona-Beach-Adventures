#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>

#ifndef AUDIO_H
#define AUDIO_H

typedef struct Musicas {
    ALLEGRO_AUDIO_STREAM* som;
} Musica;

// Carrega um audio na memoria
// local - caminho do arquivo do audio a ser carregado
Musica carregar_audio(const char* local);

#endif

