#include "Main.h"

void main(void) {
	unsigned int a, c, x0;
	bool estValide = obtentionEtValidation(&a, &c, &x0);

	if (estValide) simulationSystemeFileAttente(a, c, x0);
	else printf("Erreur : suite invalide");
	getchar();
}