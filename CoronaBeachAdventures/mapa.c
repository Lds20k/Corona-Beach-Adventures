#include "mapa.h"

Tile* criar_tile(Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura) {
	Vetor2D posicao = { x, y };
	Vetor2D caixa = { largura, altura };
	CaixaDelimitadora dimensao = {caixa};

	Tile* tile = malloc(sizeof(Tile));
	if (tile == NULL) {
		return NULL;
	}

	tile->posicao = posicao;
	tile->dimensao = dimensao;
	tile->sprite = sprite;
	tile->next = NULL;

	return tile;
}

void adicionar_tile(Tile** tile, Sprite* sprite, unsigned x, unsigned y, unsigned largura, unsigned altura) {
	if (*tile == NULL) {
		*tile = criar_tile(sprite, x, y, largura, altura);
	}
	else {
		Tile* aux = *tile;
		while (aux->next != NULL) {
			aux = aux->next;
		}
		aux->next = criar_tile(sprite, x, y, largura, altura);
	}
}

void liberar_tile(Tile* tile) {
	Tile* aux = tile;
	while (tile != NULL) {
		aux = tile->next;
		free(tile);
		tile = aux;
	}
}

void definir_tile(Mapa* mapa, ALLEGRO_BITMAP* imagem_mapa, const unsigned x, const unsigned y) {
	const ALLEGRO_COLOR cor = al_get_pixel(imagem_mapa, x, y);
	const ALLEGRO_COLOR cor_direita = al_get_pixel(imagem_mapa, x + 1, y);
	const ALLEGRO_COLOR cor_esquerda = al_get_pixel(imagem_mapa, x - 1, y);

	const ALLEGRO_COLOR COR_BRANCO = al_map_rgb(255, 255, 255);
	const ALLEGRO_COLOR COR_VERMELHO = al_map_rgb(255, 0, 0);
	const ALLEGRO_COLOR COR_VERMELHO_ESCURO = al_map_rgb(127, 0, 0);

	if (tile_sheet == NULL) tile_sheet = carregar_imagem("corona_beach.bmp");

	if (terra == NULL) terra = criar_sprite(tile_sheet, 16, 0, 16, 16, 0);
	if (terra_direita == NULL) terra_direita = criar_sprite(tile_sheet, 0, 0, 16, 16, 0);
	if (terra_esquerda == NULL) terra_esquerda = criar_sprite(tile_sheet, 0, 0, 16, 16, ALLEGRO_FLIP_HORIZONTAL);

	Sprite* sprite = NULL;

	if (!memcmp(&cor, &COR_BRANCO, sizeof(ALLEGRO_COLOR))) {
		return true;
	}

	if (!memcmp(&cor, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) {
		sprite = terra;
	}

	if (!memcmp(&cor, &COR_VERMELHO_ESCURO, sizeof(ALLEGRO_COLOR))) {
		if (!memcmp(&cor_direita, &COR_VERMELHO, sizeof(ALLEGRO_COLOR)))
			sprite = terra_direita;
		else
			if (!memcmp(&cor_esquerda, &COR_VERMELHO, sizeof(ALLEGRO_COLOR)))
				sprite = terra_esquerda;
	}

	adicionar_tile(&mapa->tiles, sprite, x * TAMANHO_DO_TILE, y * TAMANHO_DO_TILE, TAMANHO_DO_TILE, TAMANHO_DO_TILE);
	printf("%d %d\t| %f %f %f\n", x, y, cor.r, cor.g, cor.b);
}

Mapa* carregar_mapa(const char* local) {
	Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));
	if (mapa == NULL) {
		return NULL;
	}

	// Carrega o bitmap que contem as infomações do mapa
	ALLEGRO_BITMAP* imagem_mapa = al_load_bitmap(local);
	if (!imagem_mapa) {
		printf("Erro ao carregar mapa %s\n", local);
		return NULL;
	}

	Vetor2D dimensao = { al_get_bitmap_width(imagem_mapa), al_get_bitmap_height(imagem_mapa) };
	mapa->tiles = NULL;
	mapa->dimensao = dimensao;

	// Adiciona tiles ao mapa com base na cor dos pixel de imagem_mapa
	for (unsigned i = 0; i < mapa->dimensao.y; i++) {
		for (unsigned j = 0; j < mapa->dimensao.x; j++) {
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
		desenhar_sprite(sprite, &tile->posicao);
		tile = tile->next;
	}
}


bool colidiu_tile(Tile* tile, Personagem* personagem) {
	bool colidiu = verificar_colisao(&tile->dimensao, &tile->posicao, &personagem->dimensao, &personagem->posicao);
	if (colidiu) {
		if (tile->posicao.y - personagem->posicao.y + personagem->dimensao.vetor.y > 0) {
			personagem->posicao.y = tile->posicao.y - personagem->dimensao.vetor.y;
		}
	}
	return colidiu;
}

bool colidiu_mapa(Mapa* mapa, Personagem* personagem) {
	Tile* tile = mapa->tiles;
	bool retorno = true;

	while (tile != NULL) {

		if (colidiu_tile(tile, personagem)) return true;
		tile = tile->next;
	}

	return false;
}

void liberar_mapa(Mapa* mapa) {
	al_destroy_bitmap(tile_sheet);
	liberar_tile(mapa->tiles);
	free(mapa);
}