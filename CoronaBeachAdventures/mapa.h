#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "util.h"
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
	char* tipo;
} Tile;

typedef struct ListaTiles {
	Tile* tile;
	struct ListaTiles* next;
} ListaTile;

typedef struct AreasTransmicoes{
	Vetor2D posicao;
	CaixaDelimitadora dimensao;
} AreaTransmicao;

typedef struct ListaAreas {
	AreaTransmicao* area;
	struct ListaAreas* next;
} ListaArea;

typedef struct Mapas {
	ListaTile* tiles;
	ListaArea* areas;
	Vetor2D dimensao;
} Mapa;

Tile* finalizador;
Vetor2D posicao_inicial;

static ALLEGRO_BITMAP* tile_sheet = NULL;

static Sprite* terra = NULL;
static Sprite* terra_direita = NULL;
static Sprite* terra_esquerda = NULL;
static Sprite* placa = NULL;
static Sprite* item_mascara = NULL;

// Cria um tile
Tile* criar_tile(Sprite* sprite, const float x, const float y, const float largura, const float altura, const char* tipo);

// Adiciona um item na lista de tiles
void adicionar_tile(ListaTile* tiles, Tile* tile);

// Libera a memoria da lista de Tiles
void liberar_tile(ListaTile* tiles);

// Adiciona um item na lista de areas
void adicionar_area(ListaArea* areas, AreaTransmicao* area);

void definir_tile(ListaTile* tiles, ALLEGRO_BITMAP* imagem_mapa, const float x, const float y);

// Carrega um mapa com base em uma imagem bitmap
Mapa* carregar_mapa(const char* local);

void desenhar_mapa(Mapa* mapa);

bool colidiu_tile(Tile* tile, Personagem* personagem);

Tile* colidiu_mapa(Mapa* mapa, Personagem* personagem);

AreaTransmicao* colidiu_area(Mapa* mapa, Personagem* personagem);

void liberar_mapa(Mapa* mapa);

void excluir_mascara(ListaTile* tiles);

void destruir_tile_sheet();

#endif