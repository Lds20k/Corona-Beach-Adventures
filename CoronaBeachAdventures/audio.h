#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>

void error_msg(char* text) {
    al_show_native_message_box(NULL, "ERRO",
        "Ocorreu o seguinte erro e o programa sera finalizado:",
        text, NULL, ALLEGRO_MESSAGEBOX_ERROR);
}

//musica de fundo
typedef struct {
    ALLEGRO_AUDIO_STREAM* som;
}Audio;

Audio carregar_audio(const char* local) {
    Audio audio;
    audio.som = al_load_audio_stream(local, 4, 1024);
    if (!audio.som)
    {
        printf("Erro ao carregar audio %s", local);
        al_destroy_audio_stream(audio.som);
    }
    al_attach_audio_stream_to_mixer(audio.som, al_get_default_mixer());
    return audio;
}

