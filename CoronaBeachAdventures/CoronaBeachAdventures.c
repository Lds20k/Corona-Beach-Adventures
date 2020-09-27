// CoronaBeachAdventures.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include "audio.h"

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

	//addon que da suporte as extensoes de audio
	if (!al_init_acodec_addon()) {
		error_msg("Falha ao inicializar o codec de audio");
		return 0;
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

	//addon de audio
	if (!al_install_audio()) {
		error_msg("Falha ao inicializar o audio");
		return 0;
	}

	if (!al_install_keyboard()) {
		fprintf(stderr, "Falha ao instalar o teclado.");
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

	//cria o mixer (e torna ele o mixer padrao), e adciona 5 samples de audio nele
	if (!al_reserve_samples(5)) {
		error_msg("Falha ao reservar amostrar de audio");
		return 0;
	}

	//liga o stream no mixer
	Audio musica = carregar_audio("soundtrack.ogg");
	//define que o stream vai tocar no modo repeat
	al_set_audio_stream_playmode(musica.som, ALLEGRO_PLAYMODE_LOOP);

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
			al_flip_display();
			desenhar = false;
		}
	}

	// Limpa tudo
	al_destroy_audio_stream(musica.som);
	al_destroy_event_queue(fila_eventos);
	al_destroy_display(janela);
	return 0;
}
