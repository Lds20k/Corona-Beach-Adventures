#include "personagem.h"

Personagem* carrega_personagem(Sprite* sprite, int x, int y, int largura, int altura) {
	Vetor2D posicao = { x, y };
	Vetor2D caixa = { largura, altura };
	CaixaDelimitadora dimensao = { caixa };

	Personagem* personagem = malloc(sizeof(Personagem));
	if (personagem == NULL) return NULL;

	personagem->posicao = posicao;
	personagem->dimensao = dimensao;
	personagem->sprite = sprite;
	return personagem;
}

void desenhar_personagem(Personagem* personagem) {
	desenhar_sprite(personagem->sprite, &personagem->posicao);
}