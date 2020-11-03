#include "personagem.h"

Personagem* carrega_personagem(Sprite* sprite, float x, float y, float largura, float altura) {
	Vetor2D posicao = { x, y };
	Vetor2D caixa = { largura, altura };
	CaixaDelimitadora dimensao = { caixa };

	Personagem* personagem = malloc(sizeof(Personagem));
	if (personagem == NULL) return NULL;

	personagem->posicao = posicao;
	personagem->dimensao = dimensao;
	personagem->sprite = sprite;
	personagem->vida = 100;
	personagem->morto = false;

	return personagem;
}

void desenhar_personagem(Personagem* personagem) {
	desenhar_sprite(personagem->sprite, &personagem->posicao);
}

void diminuir_vida(Personagem* personagem, unsigned dano) {
	if (dano >= personagem->vida) {
		personagem->morto = true;
		return;
	}

	personagem->vida = personagem->vida - dano;
	printf("vida: %d\n", personagem->vida);
}

