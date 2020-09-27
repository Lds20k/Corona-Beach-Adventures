#ifndef MAPA_H
#define MAPA_H

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>

#include "sprites.h"

typedef struct Tile {
	Sprite* sprite;
	struct Tile* next;
} Tile;

typedef struct Mapa {
	Tile* tiles;
	unsigned largura;
	unsigned altura;
} Mapa;

// Cria um tile
Tile* criar_tile(Sprite* sprite) {
	Tile* tile = (Tile*)malloc(sizeof(Tile));
	if (tile == NULL) {
		return NULL;
	}

	tile->sprite = sprite;
	tile->next = NULL;
	return tile;
}

// Cria uma lista de Tiles caso o parametro tile seja nulo
// Caso não nulo, adiciona na lista
void adicionar_tile(Tile* tile, Sprite* sprite) {
	if (tile == NULL) {
		tile = criar_tile(sprite);
	} else {
		Tile* aux = tile->next;
		while (aux != NULL) {
			aux = aux->next;
		}
		aux = criar_tile(sprite);
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

	// Sera substituido por uma lista de sprites do tipo tile
	Sprite sprite = carregar_sprite("four-seasons-tileset.bmp");

	// Adiciona tiles ao mapa com base na cor dos pixel de imagem_mapa
	for (unsigned i = 0; i < mapa->altura; i++) {
		for (unsigned j = 0; j < mapa->largura; j++) {
			ALLEGRO_COLOR cor = al_get_pixel(imagem_mapa, j, i);
			//printf("%d %d\t| %f %f %f\n", j, i, cor.r, cor.g, cor.b);
			adicionar_tile(mapa->tiles, &sprite);
		}
	}
	
	al_destroy_bitmap(imagem_mapa);
	return mapa;
}


void descarregar_mapa(Mapa* mapa) {
	liberar_tile(mapa->tiles);
	free(mapa);
}

#endif