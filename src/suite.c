#include "Main.h";

bool obtentionEtValidation(unsigned int* a, unsigned int* c, unsigned int* x0) {
	unsigned int xn;
	int iTete = 0;
	int iQueue = MAX_PROBABILITES;
	double probabilites[MAX_PROBABILITES];
	int repetitionsDCarre[MAX_DCARRE];

	//*a = 691;
	//*c = 199;
	//*x0 = 44;
	printf("Entrez la valeur de a : ");
	scanf_s("%d", a);
	printf("\nEntrez la valeur de c : ");
	scanf_s("%d", c);
	printf("\nEntrez le germe : ");
	scanf_s("%d", x0);
	while ((*a == 1 || *a == 0) && *a > 0 && *c > 0 && *a < M && *c < M && *x0 < M) {
		printf("Erreur! Valeurs invalides! Réessayez.\n\n");
		printf("Entrez la valeur de a : ");
		scanf_s("%d", a);
		printf("\nEntrez la valeur de c : ");
		scanf_s("%d", c);
		printf("\nEntrez le germe : ");
		scanf_s("%d", x0);
	}
	getchar();

	for (int i = iTete; i < iQueue; i++) {
		repetitionsDCarre[i] = 0;
	}
	generationProbabilites(probabilites);

	xn = *x0;
	for (int i = 0; i < N; i++) {
		double suite[NB_XN_MAX];
		double distanceCarre;
		int index;
		
		for (int j = 0; j < NB_XN_MAX; j++) {
			suite[j] = genererReel(*a, *c, &xn);
		}
		
		distanceCarre = pow((suite[0] - suite[2]), 2) + pow((suite[1] - suite[3]), 2);
		index = (int)(distanceCarre * 10);
		repetitionsDCarre[index]++;
	}

	return validation(probabilites, iTete, iQueue, repetitionsDCarre);
}

void generationProbabilites(double probabilites[]) {
	int i = 0;
	int x = 1;
	double pbPremiere;
	double pbSeconde = 0;

	while (i < 10) {
		pbPremiere = probabilitePre10(x);
		probabilites[i] = pbPremiere - pbSeconde;
		pbSeconde = pbPremiere;
		x++;
		i++;
	}
	while (i <= MAX_PROBABILITES) {
		pbPremiere = probabilitePost10(x);
		probabilites[i] = pbPremiere - pbSeconde;
		pbSeconde = pbPremiere;
		x++;
		i++;
	}
}

double probabilitePre10(double x) {
	x /= 10.0;
	double huitTiers = 8.0 / 3.0;
	double troisDemi = 3.0 / 2.0;
	double unDemi = 1.0 / 2.0;
	double pi = (M_PI * x) - (huitTiers * pow(x, troisDemi)) + (unDemi * pow(x, 2));
	return pi;
}

double probabilitePost10(double x) {
	x /= 10.0;
	double unTiers = 1.0 / 3.0;
	double unDemi = 1.0 / 2.0;
	double huitTiers = 8.0 / 3.0;
	double troisDemi = 3.0 / 2.0;
	double pi = unTiers + ((M_PI - 2)*x) + (4 * pow((x - 1), unDemi)) + (huitTiers*pow((x - 1), troisDemi)) - (pow(x, 2) / 2) - (4 * x * acos(1.0/sqrt(x)));
	return pi;
}

double genererReel(unsigned int a, unsigned int c, unsigned int * xn) {
	*xn = (*xn * a) + c;
	return (*xn) / M;
}

bool validation(double probabilites[], int iTete, int iQueue, int repetitionsDCarre[]) {
	double khiCarre;
	double khiTheoriques[NB_KHICARRE_THEORIQUES] = { 3.8415, 5.9915, 7.8147, 9.4888, 11.0705, 12.5916, 14.0671, 15.5073, 16.9190, 18.3070, 19.6751, 21.0261, 22.3620, 23.6848, 24.9958, 26.2962, 27.5871, 28.6893, 30.1435 };
	for (int i = iQueue - 1; i > iTete; i--) {
		double npi = probabilites[i] * N;
		if (npi < 5) {
			probabilites[i - 1] += probabilites[i];
			repetitionsDCarre[i - 1] += repetitionsDCarre[i];
			iQueue--;
		}
	}
	khiCarre = calculKhiCarreObservable(iQueue, repetitionsDCarre, probabilites);
	return khiCarre <= khiTheoriques[iQueue - 2];
}

double calculKhiCarreObservable(int iQueue, int repetitionsDCarre[], double probabilites[]) {
	double value = 0;
	for (int i = 0; i < iQueue; i++) {
		value += pow((repetitionsDCarre[i] - N * probabilites[i]), 2) / (N * probabilites[i]);
	}
	return value;
}