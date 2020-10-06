/* -- Corona Beach Adventures -- 
* Projeto desenvolvido no Senac Santo Amaro para o
* Projeto Integrado(PI) de Jogos Digitais do curso
* de Ciência da Computação.
* 
*  -- Feito por --
* Daniel Bortoleti Melo
* Lucas da Silva dos Santos
* Rafael Nascimento Rodrigues
*/

#include <stdio.h>
#include <stdlib.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_acodec.h>

#include "sprites.h"
#include "audio.h"
#include "mapa.h"

#define FPS 60.f

#define NOME_JOGO "Corona Beach Adventures"

#define JANELA_LARGURA 640
#define JANELA_ALTURA 480

#define COR_PRETA al_map_rgb(0, 0, 0)

int main() {
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool rodando = true;
	bool desenhar = true;

	// Inicia o allegro
	if (!al_init()) {
		printf("Falha ao iniciar.\n");
		return 1;
	}

	// Inicia addon que da suporte as extensoes de imagem
	if (!al_init_image_addon()) {
		printf("Falha ao inicia addon de imagem.\n");
		return 1;
	}

	// Inicia addon que da suporte as extensoes de audio
	if (!al_init_acodec_addon()) {
		printf("Falha ao inicializar o codec de audio.\n");
		return 0;
	}

	// Cria um timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		printf("Falha ao criar timer.\n");
		return 1;
	}

	// Cria uma janela de 640px por 480px
	janela = al_create_display(JANELA_LARGURA, JANELA_ALTURA);
	if (!janela) {
		printf("Falha ao criar uma janela.\n");
		return 1;
	}
	al_set_window_title(janela, NOME_JOGO);


	// Cria a fila de eventos
	fila_eventos = al_create_event_queue();
	if (!fila_eventos) {
		printf("Falha ao criar a fila de eventos.\n");
		return 1;
	}

	// Instala audio
	if (!al_install_audio()) {
		printf("Falha ao instalar o audio.\n");
		return 0;
	}

	// Instala teclado
	if (!al_install_keyboard()) {
		printf("Falha ao instalar o teclado.\n");
		return 1;
	}

	// Instala mouse
	if (!al_install_mouse()) {
		printf("Falha ao instalar o mouse.\n");
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

	// Carregar um sprite
	ALLEGRO_BITMAP* bmp_botoes = carregar_imagem("ui.bmp");
	Sprite botoes = criar_sprite(bmp_botoes, 0, 0, 16, 16, 0);

	// Cria o mixer (e torna ele o mixer padrao), e adciona 5 samples de audio nele
	if (!al_reserve_samples(5)) {
		printf("Falha ao reservar amostrar de audio");
		return 0;
	}

	// Carrega uma audio
	Audio musica = carregar_audio("soundtrack.ogg");

	// Define que o stream vai tocar no modo repeat
	al_set_audio_stream_playmode(musica.som, ALLEGRO_PLAYMODE_LOOP);

	// Carrega mapa
	Mapa* mapa = carregar_mapa("praia.bmp");

	// Loop principal do jogo
	while (rodando) {
		ALLEGRO_EVENT evento;
		ALLEGRO_TIMEOUT timeout;

		// Conecta o audio no mixer
		al_attach_audio_stream_to_mixer(musica.som, al_get_default_mixer());

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

			al_draw_bitmap(botoes.imagem, 0, 0, 0);
			desenhar_mapa(mapa);
			al_flip_display();
			desenhar = false;
		}
	}

	// Limpa tudo
	descarregar_mapa(mapa);
	al_destroy_audio_stream(musica.som);
	al_destroy_event_queue(fila_eventos);
	al_destroy_display(janela);
	return 0;
}
