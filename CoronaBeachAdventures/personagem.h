#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include "sprites.h"

typedef struct Personagens {
	int x;
	int y;
	unsigned largura;
	unsigned altura;
	Sprite* sprite;
} Personagem;

Personagem carrega_personagem(Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura);

#endif
