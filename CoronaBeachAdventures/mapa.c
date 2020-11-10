#include "mapa.h"

Tile* criar_tile(Sprite* sprite, const float x, const float y, const float largura, const float altura, const char* tipo) {
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
	tile->tipo = tipo;

	return tile;
}

void adicionar_tile(ListaTile* tiles, Tile* tile) {	
	ListaTile* aux = tiles;
	if (aux->tile == NULL) {
		aux->tile = tile;
	} else {
		while (aux->next != NULL) aux = aux->next;
		
		aux->next = malloc(sizeof(ListaTile));
		if (aux->next == NULL) return;
		aux = aux->next;

		aux->tile = tile;
		aux->next = NULL;
	}
}

void liberar_tile(ListaTile* tiles) {
	ListaTile* aux = tiles;
	while (tiles != NULL) {
		aux = tiles->next;
		free(tiles->tile->tipo);
		free(tiles->tile);
		free(tiles);
		tiles = aux;
	}
}

void definir_tile(ListaTile* tiles, ALLEGRO_BITMAP* imagem_mapa, const float x, const float y) {
	const ALLEGRO_COLOR cor = al_get_pixel(imagem_mapa, x, y);
	const ALLEGRO_COLOR cor_direita = al_get_pixel(imagem_mapa, x + 1, y);
	const ALLEGRO_COLOR cor_esquerda = al_get_pixel(imagem_mapa, x - 1, y);

	const ALLEGRO_COLOR COR_BRANCO = al_map_rgb(255, 255, 255);
	const ALLEGRO_COLOR COR_VERMELHO = al_map_rgb(255, 0, 0);
	const ALLEGRO_COLOR COR_VERMELHO_ESCURO = al_map_rgb(127, 0, 0);
	const ALLEGRO_COLOR COR_AZUL_CLARO = al_map_rgb(0, 148, 255);
	const ALLEGRO_COLOR COR_PRETO = al_map_rgb(0, 0, 0);

	Sprite* sprite = NULL;
	char* tipo = malloc(7 * sizeof(char));

	if (tile_sheet == NULL) tile_sheet = carregar_imagem("corona_beach.bmp");

	if (terra == NULL) terra = criar_sprite(tile_sheet, 16, 0, 16, 16, 0);
	if (terra_direita == NULL) terra_direita = criar_sprite(tile_sheet, 0, 0, 16, 16, 0);
	if (terra_esquerda == NULL) terra_esquerda = criar_sprite(tile_sheet, 0, 0, 16, 16, ALLEGRO_FLIP_HORIZONTAL);
	if (placa == NULL) placa = criar_sprite(tile_sheet, 0, 48, 16, 16, 0);

	if (!memcmp(&cor, &COR_BRANCO, sizeof(ALLEGRO_COLOR))) return;

	if (!memcmp(&cor, &COR_AZUL_CLARO, sizeof(ALLEGRO_COLOR))) {
		posicao_inicial.x = x * TAMANHO_DO_TILE;
		posicao_inicial.y = y * TAMANHO_DO_TILE;

		return;
	}

	if (!memcmp(&cor, &COR_PRETO, sizeof(ALLEGRO_COLOR))) { 
		sprite = placa;
		strcpy_s(tipo, 7,"objeto");
	}

	if (!memcmp(&cor, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) { 
		sprite = terra;
		strcpy_s(tipo, 7, "bloco");
	}

	if (!memcmp(&cor, &COR_VERMELHO_ESCURO, sizeof(ALLEGRO_COLOR))) {
		if (!memcmp(&cor_direita, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) {
			sprite = terra_direita;
		} else{
			if (!memcmp(&cor_esquerda, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) {
				sprite = terra_esquerda;
			}
		}
		strcpy_s(tipo, 7,"bloco");
	}

	if (sprite == NULL) return;

	Tile* tile_criado = criar_tile(sprite, x * TAMANHO_DO_TILE, y * TAMANHO_DO_TILE, TAMANHO_DO_TILE, TAMANHO_DO_TILE, tipo);
	adicionar_tile(tiles, tile_criado);

	//if (!memcmp(&cor, &COR_PRETO, sizeof(ALLEGRO_COLOR))) finalizador = tile_criado;
	printf("%f %f\t| %f %f %f\n", x, y, cor.r, cor.g, cor.b);
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
	mapa->dimensao = dimensao;
	mapa->tiles = malloc(sizeof(ListaTile));
	if (mapa->tiles == NULL) return NULL;
	mapa->tiles->tile = NULL;
	mapa->tiles->next = NULL;

	// Adiciona tiles ao mapa com base na cor dos pixel de imagem_mapa
	for (unsigned i = 0; i < mapa->dimensao.y; i++) {
		for (unsigned j = 0; j < mapa->dimensao.x; j++) {
			definir_tile(mapa->tiles, imagem_mapa, j, i);
		}
	}

	al_destroy_bitmap(imagem_mapa);
	return mapa;
}

void desenhar_mapa(Mapa* mapa) {
	ListaTile* tiles = mapa->tiles;

	while (tiles != NULL) {
		Tile* tile = tiles->tile;
		Sprite* sprite = tile->sprite;

		desenhar_sprite(sprite, &tile->posicao);
		tiles = tiles->next;
	}
}

bool colidiu_tile(Tile* tile, Personagem* personagem) {
	return verificar_colisao(&tile->dimensao, &tile->posicao, &personagem->dimensao, &personagem->posicao);
}

Tile* colidiu_mapa(Mapa* mapa, Personagem* personagem) {
	ListaTile* tiles = mapa->tiles;

	while (tiles != NULL) {
		Tile* tile = tiles->tile;

		if (colidiu_tile(tile, personagem)) return tile;
		tiles = tiles->next;
	}

	return NULL;
}

void liberar_mapa(Mapa* mapa) {
	al_destroy_bitmap(tile_sheet);
	liberar_tile(mapa->tiles);
	free(mapa);
}