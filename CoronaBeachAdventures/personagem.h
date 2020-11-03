#include <stdlib.h>

#include "sprites.h"
#include "vetor.h"
#include "colisao.h"

#ifndef PERSONAGEM_H
#define PERSONAGEM_H

typedef struct Personagens {
	Vetor2D posicao;
	CaixaDelimitadora dimensao;
	Sprite* sprite;
	unsigned vida; 
	bool morto;

} Personagem;

Personagem* carrega_personagem(Sprite* sprite, float x, float y, float largura, float altura);
void desenhar_personagem(Personagem* personagem);

void diminuir_vida(Personagem* personagem, unsigned dano);

#endif
