#ifndef MAPA_H
#define MAPA_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

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
Tile* criar_tile(Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura) {
	Tile* tile = (Tile*)malloc(sizeof(Tile));
	if (tile == NULL) {
		return NULL;
	}
	tile->x = x;
	tile->y = y;
	tile->largura = largura;
	tile->altura = altura;
	tile->sprite = sprite;
	tile->next = NULL;
	return tile;
}

// Cria uma lista de Tiles caso o parametro tile seja nulo
// Caso não nulo, adiciona na lista
void adicionar_tile(Tile** tile, Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura) {
	if (*tile == NULL) {
		*tile = criar_tile(sprite, x, y, largura, altura);
	} else {
		Tile* aux = *tile;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = criar_tile(sprite, x, y, largura, altura);
	}
}

// Libera a memoria da lista de Tiles
void liberar_tile(Tile* tile) {
	Tile* aux = tile;
	while (tile != NULL) {
		aux = tile->next;
		free(tile);
		tile = aux;
	}
}

bool definir_tile(Mapa* mapa, ALLEGRO_BITMAP* imagem_mapa, const unsigned x, const unsigned y) {
	const ALLEGRO_COLOR cor = al_get_pixel(imagem_mapa, x, y);
	const ALLEGRO_COLOR cor_direita = al_get_pixel(imagem_mapa, x+1, y);
	const ALLEGRO_COLOR cor_esquerda = al_get_pixel(imagem_mapa, x-1, y);

	const ALLEGRO_COLOR COR_BRANCO = al_map_rgb(255, 255, 255);
	const ALLEGRO_COLOR COR_VERMELHO = al_map_rgb(255, 0, 0);
	const ALLEGRO_COLOR COR_VERMELHO_ESCURO = al_map_rgb(127, 0, 0);

	if(tile_sheet == NULL) tile_sheet = carregar_imagem("corona_beach.bmp");

	if(terra.imagem == NULL) terra = criar_sprite(tile_sheet, 16, 0, 16, 16, 0);
	if(terra_direita.imagem == NULL) terra_direita = criar_sprite(tile_sheet, 0, 0, 16, 16, 0);
	if(terra_esquerda.imagem == NULL) terra_esquerda = criar_sprite(tile_sheet, 0, 0, 16, 16, ALLEGRO_FLIP_HORIZONTAL);

	Sprite* sprite = NULL;

	if (!memcmp(&cor, &COR_BRANCO, sizeof(ALLEGRO_COLOR))) {
		return true;
	}

	if (!memcmp(&cor, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) {
		sprite = &terra;
	}

	if (!memcmp(&cor, &COR_VERMELHO_ESCURO, sizeof(ALLEGRO_COLOR))) {
		if (!memcmp(&cor_direita, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) 
			sprite = &terra_direita;
		else
			if (!memcmp(&cor_esquerda, &COR_VERMELHO, sizeof(ALLEGRO_COLOR)))
				sprite = &terra_esquerda;
	}

	adicionar_tile(&mapa->tiles, sprite, x * TAMANHO_DO_TILE, y * TAMANHO_DO_TILE, TAMANHO_DO_TILE, TAMANHO_DO_TILE);
	printf("%d %d\t| %f %f %f\n", x, y, cor.r, cor.g, cor.b);
}

// Carrega um mapa com base em uma imagem bitmap
Mapa* carregar_mapa(const char* local) {
	ALLEGRO_BITMAP *imagem_mapa = NULL;
	Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));
	if (mapa == NULL) {
		return NULL;
	}

	// Carrega o bitmap que contem as infomações do mapa
	imagem_mapa = al_load_bitmap(local);
	if (!imagem_mapa) {
		printf("Erro ao carregar mapa %s\n", local);
		return NULL;
	}

	mapa->tiles = NULL;
	mapa->largura = al_get_bitmap_width(imagem_mapa);
	mapa->altura = al_get_bitmap_height(imagem_mapa);

	// Adiciona tiles ao mapa com base na cor dos pixel de imagem_mapa
	for (unsigned i = 0; i < mapa->altura; i++) {
		for (unsigned j = 0; j < mapa->largura; j++) {
			definir_tile(mapa, imagem_mapa, j, i);
		}
	}
	
	al_destroy_bitmap(imagem_mapa);
	return mapa;
}

void desenhar_mapa(Mapa* mapa) {
	Tile* tile = mapa->tiles;
	while (tile != NULL) {
		Sprite* sprite = tile->sprite;
		al_draw_bitmap_region(sprite->imagem, sprite->x, sprite->y, sprite->largura, sprite->altura, tile->x, tile->y, sprite->sinalizadores);
		tile = tile->next;
	}
}

bool verificar_colisao(Mapa* mapa, Personagem* personagem) {
	Tile* tile = mapa->tiles;

	while (tile != NULL) {

		if (personagem->y + personagem->altura < tile->y) return false;
		if (personagem->y > tile->y + tile->altura) return false;

		if (personagem->largura + personagem->x < tile->x) return false;
		if (personagem->x > tile->x + tile->largura) return false;
		tile = tile->next;
	}

	return true;
}

void descarregar_mapa(Mapa* mapa) {
	al_destroy_bitmap(tile_sheet);
	liberar_tile(mapa->tiles);
	free(mapa);
}

#endif