#include "sprites.h"

ALLEGRO_BITMAP* carregar_imagem(const char* local) {
    ALLEGRO_BITMAP* imagem = al_load_bitmap(local);
    al_convert_mask_to_alpha(imagem, MASCARA);
    if (!imagem) {
        printf("Error ao carregar imagem %s \n", local);
        al_destroy_bitmap(imagem);
    }

    return imagem;
}

Sprite criar_sprite(ALLEGRO_BITMAP* image, unsigned x, unsigned y, unsigned largura, unsigned altura, int sinalizadores) {
    Sprite sprite = { image, x, y, altura, altura, sinalizadores };
    return sprite;
}