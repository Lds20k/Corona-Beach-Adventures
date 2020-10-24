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

Sprite* criar_sprite(ALLEGRO_BITMAP* image, int x, int y, int largura, int altura, int sinalizadores) {
    Vetor2D posicao = {x, y};
    Vetor2D dimensao = { largura, altura};

    Sprite* sprite = malloc(sizeof(Sprite));
    if (sprite == NULL) return NULL;

    sprite->imagem = image;
    sprite->posicao = posicao;
    sprite->dimensao = dimensao;
    sprite->sinalizadores = sinalizadores;

    return sprite;
}

void desenhar_sprite(Sprite* sprite, Vetor2D* posicao) {
    al_draw_bitmap_region(sprite->imagem, sprite->posicao.x, sprite->posicao.y, sprite->dimensao.x, sprite->dimensao.y, posicao->x, posicao->y, sprite->sinalizadores);
}

