#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <allegro5/allegro.h>

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
ALLEGRO_BITMAP* carregar_imagem(const char* local) {
    ALLEGRO_BITMAP* imagem = al_load_bitmap(local);
    al_convert_mask_to_alpha(imagem, MASCARA);
    if (!imagem) {
        printf("Error ao carregar imagem %s \n", local);
        al_destroy_bitmap(imagem);
    }

    return imagem;
}

// Cria um sprite
Sprite criar_sprite(ALLEGRO_BITMAP* image, unsigned x, unsigned y, unsigned largura, unsigned altura, int sinalizadores) {
    Sprite sprite = { image, x, y, altura, altura, sinalizadores};
    return sprite;
}

#endif