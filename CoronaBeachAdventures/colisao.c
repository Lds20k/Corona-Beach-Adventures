#include "colisao.h"

bool verificar_colisao(CaixaDelimitadora* caixaA, Vetor2D* posA, CaixaDelimitadora* caixaB, Vetor2D* posB) {
	if (posA->y + caixaA->vetor.y < posB->y) return false;
	if (posA->y > posB->y + caixaB->vetor.y) return false;

	if (caixaA->vetor.x + posA->x < posB->x) return false;
	if (posA->x > posB->x + caixaB->vetor.x) return false;
	return true;
}
