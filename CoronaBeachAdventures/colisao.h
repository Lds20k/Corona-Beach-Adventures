#include <stdbool.h>
#include "vetor.h"

#ifndef COLISAO_H
#define COLISAO_H

typedef struct CaixasDelimitadoras {
	Vetor2D vetor;
} CaixaDelimitadora;

bool verificar_colisao(CaixaDelimitadora* caixaA, Vetor2D* posA, CaixaDelimitadora* caixaB, Vetor2D* posB);

#endif