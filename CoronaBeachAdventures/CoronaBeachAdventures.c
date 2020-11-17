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
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_acodec.h>

#include "sprites.h"
#include "audio.h"
#include "mapa.h"
#include "personagem.h"
#include "colisao.h"

#define FPS 60.f

#define NOME_JOGO "Corona Beach Adventures"

#define JANELA_LARGURA 640
#define JANELA_ALTURA 480

#define COR_PRETA al_map_rgb(0, 0, 0)

#define GRAVIDADE 0.085
#define VELOCIDADE_MAX_X 6
#define VELOCIDADE_MAX_Y 20 
#define ALTURA_MAX_PULO 15
#define DELTA_PULO 1.2
#define REBOTE_Y 0.3

enum TECLAS { ESQUERDA, DIREITA, CIMA, BAIXO };

int main() {
	ALLEGRO_DISPLAY* janela = NULL;
	ALLEGRO_EVENT_QUEUE* fila_eventos = NULL;
	ALLEGRO_TIMER* timer = NULL;

	bool rodando = true;
	bool desenhar = true;
	bool teclas[] = { false,false,false,false };

	float velocidadeGravidade = -1;
	float alturaPulo = 0;
	unsigned frames = 0;

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

	// Inicia os dados primitivos do allegro
	if (!al_init_primitives_addon()) {
		printf("Falha ao inicializar os primitivos.\n");
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

	// Cria o mixer (e torna ele o mixer padrao), e adciona 5 samples de audio nele
	if (!al_reserve_samples(5)) {
		printf("Falha ao reservar amostrar de audio");
		return 1;
	}

	// Carrega uma audio
	Musica musicaFundo;
	musicaFundo = carregar_audio("soundtrack.ogg");

	// Define que o stream vai tocar no modo repeat
	al_set_audio_stream_playmode(musicaFundo.som, ALLEGRO_PLAYMODE_LOOP);

	// Carrega mapa
	Mapa* mapa = carregar_mapa("praia");

	// Carrega personagem
	// Carregar um sprite
	ALLEGRO_BITMAP* bmp_botoes = carregar_imagem("ui.bmp");
	ALLEGRO_BITMAP* gameover = carregar_imagem("gameover.bmp");
	ALLEGRO_BITMAP* vitoria_img = carregar_imagem("vitoria.bmp");

	Sprite* gameover_sprite = criar_sprite(gameover, 0, 0, JANELA_LARGURA, JANELA_ALTURA, 0);
	Sprite* vitoria_sprite = criar_sprite(vitoria_img, 0, 0, JANELA_LARGURA, JANELA_ALTURA, 0);
	Sprite* botoes = criar_sprite(bmp_botoes, 0, 0, 16, 16, 0);
	Personagem* personagem = carrega_personagem(botoes, posicao_inicial.x, posicao_inicial.y, 16, 16);
	
	//cria um vetor que guarda a velocidade do personagem
	Vetor2D velocidadePersonagem;
	velocidadePersonagem.y = 0;
	velocidadePersonagem.x = 0;

	Vetor2D aux = { 0, 0 };
	bool vitoria = false;

	// Loop principal do jogo
	while (rodando) {

		ALLEGRO_EVENT evento;

		// Busca o evento (se houver)
		al_wait_for_event(fila_eventos, &evento);

		if (evento.type) {

			// Verifica se o timer ta rodando e desenha
			if (evento.type == ALLEGRO_EVENT_TIMER) {
				desenhar = true;
				frames++;
			}

			// Verifica evento de clicar em uma tecla
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
				case ALLEGRO_KEY_BACKSPACE:
					diminuir_vida(personagem, 10);
					break;
				case ALLEGRO_KEY_ENTER:
					if(verificar_colisao(&finalizador->dimensao, &finalizador->posicao, &personagem->dimensao, &personagem->posicao))
						vitoria = true;
					break;
				}
			}

			// Verifica se a tecla foi solta
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

			// Fecha a aba
			if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				rodando = false;
			}
		}

		Tile* tile_colidido = colidiu_mapa(mapa, personagem);
		if (tile_colidido != NULL) {

			// verifica quando colidi na vertical
			if (velocidadePersonagem.y != 0) {
				// colisao de cima para baixo
				if (velocidadePersonagem.y > 0){
					personagem->posicao.y = tile_colidido->posicao.y - personagem->dimensao.vetor.y;

					// reinicia os valores caso haja colisao
					velocidadeGravidade = 0;
					velocidadePersonagem.y = 0;
					alturaPulo = 0;
				}
				else {
					// colisao de baixo para cima
					personagem->posicao.y = (tile_colidido->posicao.y + tile_colidido->dimensao.vetor.y);
					velocidadePersonagem.y *= -REBOTE_Y;
				}
			}

			float cantoEsqPlataforma = tile_colidido->posicao.x;
			float cantoDirPlataforma = tile_colidido->posicao.x + tile_colidido->dimensao.vetor.x;
			float cantoEsqPersonagem = personagem->posicao.x;
			float cantoDirPersonagem = personagem->posicao.x + personagem->dimensao.vetor.x;
			float eixoPersonagem = personagem->dimensao.vetor.x/2;
			
			// verifica se colidiu com o canto esquerdo com objeto em direcao a direita
			// ha um rebote com perda de forca
			if (velocidadePersonagem.x > 0 && cantoEsqPlataforma <= cantoDirPersonagem && cantoEsqPlataforma >= cantoDirPersonagem - (eixoPersonagem * 0.5)) {
				printf("olha o rebote para esquerda");
				velocidadePersonagem.x = 0;
			}
			
			/*
			* falta ajustes
			// verifica se colidiu com o canto direito com objeto em direcao a esquerda
			// ha um rebote com perda de forca
			if (velocidadePersonagem.x < 0 && cantoDirPlataforma >= cantoEsqPersonagem && cantoDirPlataforma <= cantoEsqPersonagem + (eixoPersonagem * 0.5)) {
				printf("pqp");
				velocidadePersonagem.x = 0;
			}
			*/

			// Inicio pulo quando colidir
			if (teclas[CIMA] && alturaPulo < ALTURA_MAX_PULO) {
				velocidadePersonagem.y -= DELTA_PULO;
				alturaPulo += DELTA_PULO;
			}

			// Movimentacao no eixo x
			//enquando esta colidindo
			if ((teclas[DIREITA] || teclas[ESQUERDA])) {

				//caso esteja no intervalo entre a maior e menor velocidade
				if ((velocidadePersonagem.x < VELOCIDADE_MAX_X) && (velocidadePersonagem.x > -VELOCIDADE_MAX_X)) {
					velocidadePersonagem.x += 0.6 * teclas[DIREITA];
					velocidadePersonagem.x -= 0.6 * teclas[ESQUERDA];
				}
			} else {
				velocidadePersonagem.x *= 0.5;
			}
		}

		//quando nao ha colisao
		if (tile_colidido == NULL) {

			/*
			Mecanica de Pulo
			se a tecla cima for acionada
			e 
			a altura do pulo for menor que a altura maxima permitida
			e 
			nao estiver colidindo com nada
			*/
			if (teclas[CIMA] && alturaPulo < ALTURA_MAX_PULO) {
				velocidadePersonagem.y -= DELTA_PULO;
				alturaPulo += DELTA_PULO;
			}
			else {
				//se a altura max do pulo for alcancada e necessario esperar ate colidir
				teclas[CIMA] = false;
			}

			//caso seja menor que o limite de velocidade 
			if (velocidadePersonagem.y <= VELOCIDADE_MAX_Y) {
				velocidadePersonagem.y -= velocidadeGravidade - (2.2 * teclas[BAIXO]);
				velocidadeGravidade -= GRAVIDADE;
			}
			//senao continua na mesma velocidade/velocidade maxima de y

			/* 
			Movimentacao no eixo x
			enquando esta colidindo
			*/
			if ((teclas[DIREITA] || teclas[ESQUERDA])) {

				//caso esteja no intervalo entre a maior e menor velocidade
				if ((velocidadePersonagem.x < VELOCIDADE_MAX_X) && (velocidadePersonagem.x > -VELOCIDADE_MAX_X)) {
					velocidadePersonagem.x += 0.6 * teclas[DIREITA];
					velocidadePersonagem.x -= 0.6 * teclas[ESQUERDA];
				}
			}
			else {
				velocidadePersonagem.x *= 0.5;
			}
		}
		/*
		movimentacao personagem
		baseada no vetor do personagem
		*/
		personagem->posicao.y += velocidadePersonagem.y;
		personagem->posicao.x += velocidadePersonagem.x;

		if (personagem->posicao.y > 580) {
			diminuir_vida(personagem, 100);
		}

		if (colidiu_area(mapa, personagem)) {
			printf("na area\n");
		}

		// Verica se é necessario limpar a tela
		if (desenhar && al_is_event_queue_empty(fila_eventos)) {
			al_clear_to_color(COR_PRETA);
			
			if (personagem->morto) {
				desenhar_sprite(gameover_sprite, &aux);
			} else {
				if (vitoria) {
					desenhar_sprite(vitoria_sprite, &aux);
				} else {
					desenhar_mapa(mapa);
					desenhar_personagem(personagem);
				}
			}
			
			//printf("%d\n", frames);

			al_flip_display();
			desenhar = false;
		}
	}


	// Limpa tudo
	liberar_mapa(mapa);
	al_destroy_bitmap(personagem->sprite->imagem);
	al_destroy_audio_stream(musicaFundo.som);
	al_destroy_event_queue(fila_eventos);
	al_destroy_display(janela);
	return 0;
}