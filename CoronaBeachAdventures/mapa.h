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

Tile* finalizador;
Vetor2D posicao_inicial;

static ALLEGRO_BITMAP* tile_sheet = NULL;

static Sprite* terra = NULL;
static Sprite* terra_direita = NULL;
static Sprite* terra_esquerda = NULL;
static Sprite* placa = NULL;

// Cria um tile
Tile* criar_tile(Sprite* sprite, const float x, const float y, const float largura, const float altura);

// Cria uma lista de Tiles caso o parametro tile seja nulo
// Caso não nulo, adiciona na lista
Tile* adicionar_tile(Tile** tile, Sprite* sprite, const float x, const float y, const float largura, const float altura);

// Libera a memoria da lista de Tiles
void liberar_tile(Tile* tile);

void definir_tile(Mapa* mapa, ALLEGRO_BITMAP* imagem_mapa, const float x, const float y);

// Carrega um mapa com base em uma imagem bitmap
Mapa* carregar_mapa(const char* local);

void desenhar_mapa(Mapa* mapa);

bool colidiu_tile(Tile* tile, Personagem* personagem);

bool colidiu_mapa(Mapa* mapa, Personagem* personagem);

void liberar_mapa(Mapa* mapa);

#endif