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

#endif
