#include "personagem.h"

Personagem carrega_personagem(Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura) {
	Personagem personagem;
	personagem.x = x;
	personagem.y = y;
	personagem.largura = largura;
	personagem.altura = altura;
	personagem.sprite = sprite;
	return personagem;
}