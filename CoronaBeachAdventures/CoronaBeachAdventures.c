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
#include "personagem.h"

#define FPS 60.f

#define NOME_JOGO "Corona Beach Adventures"

#define JANELA_LARGURA 640
#define JANELA_ALTURA 480

#define COR_PRETA al_map_rgb(0, 0, 0)

#define ACELERACAO 0.15

enum TECLAS { ESQUERDA, DIREITA, CIMA, BAIXO };

int main() {
	ALLEGRO_DISPLAY *janela = NULL;
	ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
	ALLEGRO_TIMER *timer = NULL;

	bool rodando = true;
	bool desenhar = true;
	bool teclas[] = {false,false,false,false};

	float velocidadeY = -1;
	float velocidadeX = 3;
	int contPulos = 0;

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
		return 1;
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
	Musica musicaFundo;
	musicaFundo = carregar_audio("soundtrack.ogg");
	
	// Define que o stream vai tocar no modo repeat
	al_set_audio_stream_playmode(musicaFundo.som, ALLEGRO_PLAYMODE_LOOP);

	// Carrega mapa
	Mapa* mapa = carregar_mapa("praia.bmp");

	//carrega personagem
	Personagem personagem;
	personagem = carrega_personagem(&botoes, 0, 0, 16, 16, 0);

	// Loop principal do jogo
	while (rodando) {

		ALLEGRO_EVENT evento;
		ALLEGRO_TIMEOUT timeout;

		// Inicializa o timer
		al_init_timeout(&timeout, 0.06);

		// Busca o evento (se houver)
		bool get_event = al_wait_for_event_until(fila_eventos, &evento, &timeout);

		//verifica se o timer ta rodando e desenha
		if (evento.type == ALLEGRO_EVENT_TIMER) {
			desenhar = true;
}

	//verifica evento de clicar em uma tecla
	if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
		switch (evento.keyboard.keycode) {
		case ALLEGRO_KEY_LEFT:
			teclas[ESQUERDA] = true;
			break;
		case ALLEGRO_KEY_RIGHT:
			teclas[DIREITA] = true;
			break;
		case ALLEGRO_KEY_UP:
			teclas[CIMA] = true;
			break;
		case ALLEGRO_KEY_DOWN:
			teclas[BAIXO] = true;
			break;
		}
	}

	//verifica senão teve coisao
	if (!verificar_colisao(mapa, &personagem)) {
		personagem.y -= velocidadeY - (2.2 * teclas[BAIXO]);
		velocidadeY -= ACELERACAO;
	}

	//verifica se teve colisao
	if (verificar_colisao(mapa, &personagem)) {
		velocidadeY = 0;
		contPulos = 0;
	}

	//pulo
	if (teclas[CIMA] && contPulos < 14) {
		velocidadeY = 3;
		personagem.y -= velocidadeY;
		contPulos++;
	}

	//verifica se a tecla foi solta
	if (evento.type == ALLEGRO_EVENT_KEY_UP) {
		switch (evento.keyboard.keycode) {
		case ALLEGRO_KEY_LEFT:
			teclas[ESQUERDA] = false;
			break;
		case ALLEGRO_KEY_RIGHT:
			teclas[DIREITA] = false;
			break;
		case ALLEGRO_KEY_UP:
			teclas[CIMA] = false;
			break;
		case ALLEGRO_KEY_DOWN:
			teclas[BAIXO] = false;
			break;
		}
	}

	//movimentacao no eixo x
	if (teclas[DIREITA] || teclas[ESQUERDA]) {
		personagem.x += velocidadeX * teclas[DIREITA];
		personagem.x -= velocidadeX * teclas[ESQUERDA];
	}

	// Verica se é necessario limpar a tela
	if (desenhar && al_is_event_queue_empty(fila_eventos)) {
		al_clear_to_color(COR_PRETA);

		al_draw_bitmap_region(personagem.sprite->imagem, personagem.sprite->x, personagem.sprite->y, personagem.sprite->largura, personagem.sprite->altura, personagem.x, personagem.y, 0);
		desenhar_mapa(mapa);
		al_flip_display();
		desenhar = false;
	}
	//fecha a aba
	if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
		rodando = false;
	}
}


	// Limpa tudo
	descarregar_mapa(mapa);
	al_destroy_bitmap(personagem.sprite->imagem);
	al_destroy_audio_stream(musicaFundo.som);
	al_destroy_event_queue(fila_eventos);
	al_destroy_display(janela);
	return 0;
}
