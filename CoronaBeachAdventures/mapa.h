#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "vetor.h"
#include "colisao.h"
#include "sprites.h"
#include "personagem.h"

#ifndef MAPA_H
#define MAPA_H

#define TAMANHO_DO_TILE 16

static ALLEGRO_BITMAP* tile_sheet = NULL;

static Sprite* terra = NULL;
static Sprite* terra_direita = NULL;
static Sprite* terra_esquerda = NULL;

typedef struct Tiles {
	Vetor2D posicao;
	CaixaDelimitadora dimensao;
	Sprite* sprite;
	struct Tiles* next;
} Tile;

typedef struct Mapas {
	Tile* tiles;
	Vetor2D dimensao;
} Mapa;

// Cria um tile
Tile* criar_tile(Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura);

// Cria uma lista de Tiles caso o parametro tile seja nulo
// Caso não nulo, adiciona na lista
void adicionar_tile(Tile** tile, Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura);

// Libera a memoria da lista de Tiles
void liberar_tile(Tile* tile);

void definir_tile(Mapa* mapa, ALLEGRO_BITMAP* imagem_mapa, const unsigned x, const unsigned y);

// Carrega um mapa com base em uma imagem bitmap
Mapa* carregar_mapa(const char* local);

void desenhar_mapa(Mapa* mapa);

bool colidiu_tile(Tile* tile, Personagem* personagem);

bool colidiu_mapa(Mapa* mapa, Personagem* personagem);

void liberar_mapa(Mapa* mapa);

#endif