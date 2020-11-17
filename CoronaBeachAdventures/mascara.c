#include "mascara.h"

Mascara* carrega_mascara(unsigned vida) {
	Mascara* mascara = malloc(sizeof(Mascara));
	if (mascara == NULL) return NULL;

	mascara->vida = vida;
	mascara->usando = false;
	
	return mascara;
}

unsigned usando_mascara(Mascara mascara) {
	if (mascara.vida > 0) {
		mascara.vida -= 5;
	}
	return mascara.vida;
}

