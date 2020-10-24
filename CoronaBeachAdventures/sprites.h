#include <stdio.h>
#include <allegro5/allegro.h>

#include "vetor.h"

#ifndef SPRITE_H
#define SPRITE_H

#define MASCARA al_map_rgb(255, 0, 255)

typedef struct Sprite {
    ALLEGRO_BITMAP* imagem;
    Vetor2D posicao;
    Vetor2D dimensao;
    int sinalizadores;
} Sprite;

// Carrega uma imagem bitmap
ALLEGRO_BITMAP* carregar_imagem(const char* local);

// Cria um sprite
Sprite* criar_sprite(ALLEGRO_BITMAP* image, int x, int y, int largura, int altura, int sinalizadores);

void desenhar_sprite(Sprite* sprite, Vetor2D* posicao);

#endif