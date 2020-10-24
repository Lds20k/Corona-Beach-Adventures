#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#ifndef MAPA_H
#define MAPA_H

#include "sprites.h"
#include "personagem.h"

#define TAMANHO_DO_TILE 16

static ALLEGRO_BITMAP* tile_sheet = NULL;

Sprite terra;
Sprite terra_direita;
Sprite terra_esquerda;

typedef struct Tiles {
	unsigned x;
	unsigned y;
	unsigned largura;
	unsigned altura;
	Sprite* sprite;
	struct Tile* next;
} Tile;

typedef struct Mapas {
	Tile* tiles;
	unsigned largura;
	unsigned altura;
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

static bool verificar_colisao_de_tile(Tile* tile, Personagem* personagem);

bool verificar_colisao(Mapa* mapa, Personagem* personagem);

void descarregar_mapa(Mapa* mapa);

#endif