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
		free(tiles->tile);
		free(tiles);
		tiles = aux;
	}
}

void adicionar_area(ListaArea* areas, AreaTransmicao* area) {
	ListaArea* aux = areas;
	if (aux->area == NULL) {
		aux->area = area;
	}
	else {
		while (aux->next != NULL) aux = aux->next;

		aux->next = malloc(sizeof(ListaTile));
		if (aux->next == NULL) return;
		aux = aux->next;

		aux->area = area;
		aux->next = NULL;
	}
}

void liberar_area(ListaArea* areas) {
	ListaArea* aux = areas;
	while (areas != NULL) {
		aux = areas->next;
		free(areas->area);
		free(areas);
		areas = aux;
	}
}

void definir_tile(ListaTile* tiles, ALLEGRO_BITMAP* imagem_mapa, const float x, const float y) {
	const ALLEGRO_COLOR cor = al_get_pixel(imagem_mapa, x, y);
	const ALLEGRO_COLOR cor_direita = al_get_pixel(imagem_mapa, x + 1, y);
	const ALLEGRO_COLOR cor_esquerda = al_get_pixel(imagem_mapa, x - 1, y);

	const ALLEGRO_COLOR COR_BRANCO = al_map_rgb(255, 255, 255);
	const ALLEGRO_COLOR COR_VERMELHO = al_map_rgb(255, 0, 0);
	const ALLEGRO_COLOR COR_VERMELHO_ESCURO = al_map_rgb(127, 0, 0);
	const ALLEGRO_COLOR COR_AZUL_CLARO = al_map_rgb(0, 255, 255);
	const ALLEGRO_COLOR COR_PRETO = al_map_rgb(0, 0, 0);
	const ALLEGRO_COLOR COR_VERDE_CLARO = al_map_rgb(0, 255, 0);

	Sprite* sprite = NULL;
	bool eh_finalizador = false;
	char* tipo = malloc(7 * sizeof(char));

	if (tile_sheet == NULL) 
		tile_sheet = carregar_imagem("corona_beach.bmp");

	if (terra == NULL) terra = criar_sprite(tile_sheet, 16, 0, 16, 16, 0);
	if (terra_direita == NULL) terra_direita = criar_sprite(tile_sheet, 0, 0, 16, 16, 0);
	if (terra_esquerda == NULL) terra_esquerda = criar_sprite(tile_sheet, 0, 0, 16, 16, ALLEGRO_FLIP_HORIZONTAL);
	if (placa == NULL) placa = criar_sprite(tile_sheet, 0, 48, 16, 16, 0);
	if (item_mascara == NULL) item_mascara = criar_sprite(tile_sheet, 16, 32, 16, 16, 0);


	if (!memcmp(&cor, &COR_BRANCO, sizeof(ALLEGRO_COLOR))) return;

	if (!memcmp(&cor, &COR_AZUL_CLARO, sizeof(ALLEGRO_COLOR))) {
		posicao_inicial.x = x * TAMANHO_DO_TILE;
		posicao_inicial.y = y * TAMANHO_DO_TILE;

		return;
	}

	if (!memcmp(&cor, &COR_VERDE_CLARO, sizeof(ALLEGRO_COLOR))) {
		sprite = item_mascara;
		tipo = "mascara";
	}

	if (!memcmp(&cor, &COR_PRETO, sizeof(ALLEGRO_COLOR))) { 
		sprite = placa;
		tipo = "objeto";
		eh_finalizador = true;
	}

	if (!memcmp(&cor, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) { 
		sprite = terra;
		tipo = "bloco";
	}

	if (!memcmp(&cor, &COR_VERMELHO_ESCURO, sizeof(ALLEGRO_COLOR))) {
		if (!memcmp(&cor_direita, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) {
			sprite = terra_direita;
		} else{
			if (!memcmp(&cor_esquerda, &COR_VERMELHO, sizeof(ALLEGRO_COLOR))) {
				sprite = terra_esquerda;
			}
		}
		tipo = "bloco";
	}

	if (sprite == NULL) return;

	Tile* tile_criado = criar_tile(sprite, x * TAMANHO_DO_TILE, y * TAMANHO_DO_TILE, TAMANHO_DO_TILE, TAMANHO_DO_TILE, tipo);
	adicionar_tile(tiles, tile_criado);

	if (eh_finalizador) finalizador = tile_criado;
	printf("%f %f\t| %f %f %f\n", x, y, cor.r, cor.g, cor.b);
}

Mapa* carregar_mapa(const char* local) {
	Mapa* mapa = (Mapa*)malloc(sizeof(Mapa));
	if (mapa == NULL) {
		return NULL;
	}

	const const rsize_t tamanho = sizeof(char) * 40;

	char* image_local = malloc(tamanho);
	if (image_local == NULL)
		return NULL;
	strcpy_s(image_local, tamanho, local);
	strcat_s(image_local, tamanho, ".bmp");

	// Carrega o bitmap que contem as infomações do mapa
	ALLEGRO_BITMAP* imagem_mapa = al_load_bitmap(image_local);
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

	ListaArea* areas = malloc(sizeof(ListaArea));
	if (areas == NULL)
		return NULL;
	areas->area = NULL;
	areas->next = NULL;
	
	char* area_local = malloc(tamanho);
	if (area_local == NULL)
		return NULL;
	strcpy_s(area_local, sizeof(char) * 30, local);
	strcat_s(area_local, sizeof(char) * 30, ".dat");

	ALLEGRO_FILE* areasContMapa = NULL;
	areasContMapa = al_fopen(area_local, "r");

	char* buffer = malloc(tamanho);
	char* pbuffer = NULL;
	while (!al_feof(areasContMapa)) {
		pbuffer = al_fgets(areasContMapa, buffer, tamanho);
		if (pbuffer != NULL) {
			char* aux = NULL;
			int flag = 0;
			AreaTransmicao* area = malloc(sizeof(AreaTransmicao));
			if (area == NULL) return NULL;

			do {
				char* data = (aux == NULL) ? strchr(pbuffer, ' ') + 1 : aux;

				char* aux2 = strchr(data, ',');
				if (aux2 == NULL) { 
					aux2 = strchr(data, ';'); 
					aux = NULL;
				} else {
					aux = strchr(aux2, ' ') + 1;
				}
				
				int tamanho_variavel = aux2 - data;
				char aux3[10];
				substring(aux3, data, 0, tamanho_variavel);

				int valor = atoi(aux3) * TAMANHO_DO_TILE;
				switch (flag)
				{
					case 0:
						area->posicao.x = valor;
						break;
					case 1:
						area->posicao.y = valor;
						break;
					case 2:
						area->dimensao.vetor.x = valor;
						break;
					case 3:
						area->dimensao.vetor.y = valor;
						break;
				}
				flag++;
			} while (aux != NULL);
			adicionar_area(areas, area);
		}
	}
	mapa->areas = areas;

	al_fclose(areasContMapa);

	al_destroy_bitmap(imagem_mapa);
	free(image_local);
	free(area_local);
	free(buffer);
	return mapa;
}

void desenhar_mapa(Mapa* mapa) {
	ListaTile* tiles = mapa->tiles;
	ListaArea* areas = mapa->areas;

	while (tiles != NULL) {
		Tile* tile = tiles->tile;
		Sprite* sprite = tile->sprite;

		desenhar_sprite(sprite, &tile->posicao);
		tiles = tiles->next;
	}

	while (areas != NULL && areas->area != NULL) {
		AreaTransmicao* area = areas->area;
		float x2 = area->dimensao.vetor.x + area->posicao.x;
		float y2 = area->dimensao.vetor.y + area->posicao.y;
		al_draw_filled_rectangle(area->posicao.x, area->posicao.y, x2, y2, al_map_rgba(127, 0, 0, 0));
		areas = areas->next;
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

AreaTransmicao* colidiu_area(Mapa* mapa, Personagem* personagem){
	ListaArea* areas = mapa->areas;

	while (areas != NULL && areas->area != NULL) {
		AreaTransmicao* area = areas->area;

		if (colidiu_tile(area, personagem)) return area;
		areas = areas->next;
	}

	return NULL;
}

void liberar_mapa(Mapa* mapa) {
	liberar_tile(mapa->tiles);
	liberar_area(mapa->areas);
	free(mapa);
}