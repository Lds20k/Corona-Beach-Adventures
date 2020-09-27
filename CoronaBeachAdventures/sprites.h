#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <allegro5/allegro.h>

typedef struct Sprite {
    ALLEGRO_BITMAP* imagem;
    unsigned largura;
    unsigned altura;
    unsigned x;
    unsigned y;
} Sprite;

// Carrega uma imagem bitmap
Sprite carregar_sprite(const char* local) {
    Sprite sprite; 

    sprite.imagem = al_load_bitmap(local);
    if (!sprite.imagem) {
        printf("Error ao carregar imagem %s \n", local);
        al_destroy_bitmap(sprite.imagem);
    }

    sprite.largura = 0;
    sprite.altura = 0;
    sprite.x = 0;
    sprite.y = 0;

    return sprite; 
};

#endif