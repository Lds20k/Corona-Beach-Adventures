#include <stdio.h>
#include <allegro5/allegro.h>

#ifndef SPRITE_H
#define SPRITE_H

#define MASCARA al_map_rgb(255, 0, 255)

typedef struct Sprite {
    ALLEGRO_BITMAP* imagem;
    unsigned x;
    unsigned y;
    unsigned largura;
    unsigned altura;
    int sinalizadores;
} Sprite;

// Carrega uma imagem bitmap
ALLEGRO_BITMAP* carregar_imagem(const char* local);

// Cria um sprite
Sprite criar_sprite(ALLEGRO_BITMAP* image, unsigned x, unsigned y, unsigned largura, unsigned altura, int sinalizadores);

#endif