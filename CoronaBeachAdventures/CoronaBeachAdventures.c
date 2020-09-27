// CoronaBeachAdventures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "sprites.h"

#define FPS 60.f

#define NOME_JOGO "Corona Beach Adventures"

#define JANELA_LARGURA 640
#define JANELA_ALTURA 480

#define COR_PRETA al_map_rgb(0, 0, 0)

int main()
{
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool rodando = true;
	bool desenhar = true;

	// Inicia o allegro
	if (!al_init()) {
		fprintf(stderr, "Falha ao iniciar.\n");
		return 1;
	}

	// Cria um timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer.\n");
		return 1;
	}

	// Cria uma janela de 640px por 480px
	janela = al_create_display(JANELA_LARGURA, JANELA_ALTURA);
	if (!janela) {
		fprintf(stderr, "Falha ao criar uma janela.\n");
		return 1;
	}
	al_set_window_title(janela, NOME_JOGO);


	// Cria a fila de eventos
	fila_eventos = al_create_event_queue();
	if (!fila_eventos) {
		fprintf(stderr, "Falha ao criar a fila de eventos.");
		return 1;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "Falha ao instalar o teclado.");
		return 1;
	}

	if (!al_init_image_addon()) {
		printf("Falha ao inicia addon de imagem");
		return 1; 

	}

	if (!al_install_mouse()) {
		fprintf(stderr, "Falha ao instalar o mouse.");
		return 1;
	}

	// Registra as fontes dos eventos
	al_register_event_source(fila_eventos, al_get_display_event_source(janela));
	al_register_event_source(fila_eventos, al_get_timer_event_source(timer));
	al_register_event_source(fila_eventos, al_get_keyboard_event_source());
	al_register_event_source(fila_eventos, al_get_mouse_event_source());
	

	// Desenha uma tela preta
	al_clear_to_color(COR_PRETA);
	al_flip_display();

	// Inicia o timer
	al_start_timer(timer);

	// Carregar sprite
	Sprite imagem = carregar_sprite("ui.bmp");

	// Loop principal do jogo
	while (rodando) {
		ALLEGRO_EVENT evento;
		ALLEGRO_TIMEOUT timeout;
		

		// Inicializa o timer
		al_init_timeout(&timeout, 0.06);

		// Busca o evento (se houver)
		bool get_event = al_wait_for_event_until(fila_eventos, &evento, &timeout);

		// Trata os eventos
		if (get_event) {
			switch (evento.type) {
			case ALLEGRO_EVENT_TIMER:
				desenhar = true;
				break;
			case ALLEGRO_EVENT_DISPLAY_CLOSE:
				rodando = false;
				break;
			default:
				fprintf(stderr, "Evento recebido nao suportado: %d\n", evento.type);
				break;
			}
		}

		// Verica se é necessario limpar a tela
		if (desenhar && al_is_event_queue_empty(fila_eventos)) {
			al_clear_to_color(COR_PRETA);
			al_draw_bitmap(imagem.imagem, 0, 0, 0);
			al_flip_display();
			desenhar = false;
		}
	}

	// Limpa tudo
	al_destroy_display(janela);
	al_destroy_event_queue(fila_eventos);

	return 0;
}
