#include <stdlib.h>
#include "sprites.h"

#ifndef MASCARA_H
#define MASCARA_H

typedef struct Mascaras {
	unsigned vida;
	bool usando;

} Mascara;

Mascara* carrega_mascara(unsigned vida);

unsigned usando_mascara(Mascara mascara);

#endif
