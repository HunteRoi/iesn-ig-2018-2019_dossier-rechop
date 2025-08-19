#include "Main.h"

void simulationSystemeFileAttente(unsigned int a, unsigned int c, unsigned int x0) {
	CoutOptimal optimum = { .cout = INT_MAX, .nbStations = -1 };

	for (int iStation = NB_STATIONS_MIN-1; iStation < NB_STATIONS_MAX; iStation++) {
		unsigned int xn = x0;
		double tempCoutMin;
		Station stations[NB_STATIONS_MAX];
		double couts[NB_TYPES_COUTS];
		int filePrio = 0;
		int fileOrd = 0;
		for (int i = 0; i < NB_TYPES_COUTS; i++) couts[i] = 0;
		for (int i = 0; i < NB_STATIONS_MAX; i++) {
			stations[i].numeroStation = i;
			stations[i].clientPrio = false;
			stations[i].dureeServiceMax = 0;
			stations[i].dureeServiceRestante = 0;
		}

		for (int iMinute = 1; iMinute <= TEMPS_SIMULATION; iMinute++) {
			int nbArrivees = genererArrivees(a, c, &xn);
			for (int i = 0; i < nbArrivees; i++) {
				double reel = genererReel(a, c, &xn);
				if (reel < 0.3) filePrio++;
				else fileOrd++;
			}

			if (iMinute <= 20) afficherInformations(nbArrivees, fileOrd, filePrio, stations, iStation);
			gestionMinute(iStation, a, c, &xn, couts, iMinute, &fileOrd, &filePrio, stations);
			if (iMinute <= 20) afficherStationsEtFiles(fileOrd, filePrio, stations, iStation);
		}
		afficherCouts(couts, iStation);

		tempCoutMin = couts[iPresencePrio] + couts[iPresenceOrd] + couts[iOccupation] + couts[iInoccupation];
		if (tempCoutMin < optimum.cout) {
			optimum.cout = tempCoutMin;
			optimum.nbStations = iStation;
		}
		//printf("Cout pour %2.d stations : %.2lf euros\n", iStation+1, tempCoutMin);
		printf("...............................\n\n");
	}
	printf("Nombre optimal de stations a ouvrir : %d\n", optimum.nbStations+1);
}

void afficherInformations(int nbArrivees, int fileOrd, int filePrio, Station stations[], int iStation) {
	if (iStation == NB_STATIONS_MIN-1 || iStation == VALEUR_DONNEE-1) {
		printf("\nNb arrivees : %d\n", nbArrivees);
		afficherStationsEtFiles(fileOrd, filePrio, stations, iStation);
	}
}

void afficherStationsEtFiles(int fileOrd, int filePrio, Station stations[], int iStation) {
	if (iStation == NB_STATIONS_MIN-1 || iStation == VALEUR_DONNEE-1) {
		printf("Etat des files :\tprioritaires : %d\t|\t ordinaires : %d\n", filePrio, fileOrd);

		for (int i = 0; i <= iStation; i++) {
			Station station = stations[i];
			printf("Station n-%d [%s] - %d min restantes/ %d min max\n", station.numeroStation+1, station.clientPrio ? "prio" : "ord", station.dureeServiceRestante, station.dureeServiceMax);
		}
		printf("\n\t\t\t==============================================\n\n");
	}
}

void afficherCouts(double couts[], int iStation) {
	printf("Nombre de stations ouvertes : %d\n", iStation + 1);
	printf("Cout total sur la presence de clients prioritaires : %.2lf\n", couts[iPresencePrio]);
	printf("Cout total sur la presence de clients ordinaires : %.2lf\n", couts[iPresenceOrd]);
	printf("Cout total sur l'occupation des stations : %.2lf\n", couts[iOccupation]);
	printf("Cout total sur l'inoccupation des stations : %.2lf\n", couts[iInoccupation]);
	printf("Total : %.2lf\n", couts[iPresencePrio] + couts[iPresenceOrd] + couts[iOccupation] + couts[iInoccupation]);
	//getchar();
}

void gestionMinute(int iStation, unsigned int a, unsigned int c, unsigned int* xn, double couts[], int iMinute, int* fileOrd, int* filePrio, Station stations[]) {
	gestionSimulation(iStation, a, c, xn, iMinute, fileOrd, filePrio, stations);
	gestionCouts(iStation, *filePrio, *fileOrd, couts, stations);
}

void gestionSimulation(int iStation, unsigned int a, unsigned int c, unsigned int* xn, int iMinute, int* fileOrd, int* filePrio, Station stations[]) {
	gestionPrioritaires(a, c, xn, filePrio, stations);
	gestionAutres(iStation, a, c, xn, fileOrd, filePrio, stations);
}

int genererArrivees(unsigned int a, unsigned int c, unsigned int* xn) {
	double reel = genererReel(a, c, xn) * N_ARRIVEES_PAR_MINUTE;
	if (reel < 16.0) return 0;
	else if (reel < 36.0) return 1;
	else if (reel < 48.0) return 2;
	else if (reel < 53.0) return 3;
	else if (reel < 56.0) return 4;
	else return 5;
}

void gestionPrioritaires(unsigned int a, unsigned int c, unsigned int* xn, int* filePrio, Station stations[]) {
	for (int i = 0; i < 2; i++) {
		if (stations[i].dureeServiceRestante == 0) {
			if (*filePrio != 0) {
				gestionStation(a, c, xn, stations, i, true);
				(*filePrio)--;
			}
		} else stations[i].dureeServiceRestante--;
	}
}

void gestionStation(unsigned int a, unsigned int c, unsigned int* xn, Station stations[], int i, bool prio) {
	int dureeService = genererDureeService(a, c, xn);
	stations[i].dureeServiceMax = dureeService;
	stations[i].dureeServiceRestante = dureeService;
	stations[i].clientPrio = prio;
}

int genererDureeService(unsigned int a, unsigned int c, unsigned int* xn) {
	double reel = genererReel(a, c, xn) * N_REPETITIONS_DUREE_SERVICE;
	if (reel < 18.0 ) return 1;
	else if (reel < 39.0) return 2;
	else if (reel < 54.0) return 3;
	else if (reel < 57.0) return 4;
	else if (reel < 58.0) return 5;
	else return 6;
}

void gestionAutres(int iStation, unsigned int a, unsigned int c, unsigned int* xn, int* fileOrd, int* filePrio, Station stations[]) {
	for (int i = 2; i <= iStation; i++) {
		if (stations[i].dureeServiceRestante == 0) {
			if (*fileOrd != 0) {
				gestionStation(a, c, xn, stations, i, false);
				(*fileOrd)--;
			} else if (*filePrio != 0) {
				gestionStation(a, c, xn, stations, i, true);
				(*filePrio)--;
			}
		} else stations[i].dureeServiceRestante--;
	}
}

void gestionCouts(int iStation, int filePrio, int fileOrd, double couts[], Station stations[]) {
	couts[iPresencePrio] += (double)(filePrio * PRIX_MINUTE_PRESENCE_PRIO);
	couts[iPresenceOrd] += (double)(fileOrd * PRIX_MINUTE_PRESENCE_ORD);
	for (int i = 0; i <= iStation; i++) {
		if (stations[i].dureeServiceRestante == 0) {
			couts[iInoccupation] += (double)PRIX_MINUTE_INOCCUPATION;
		} else {
			couts[iOccupation] += (double)PRIX_MINUTE_OCCUPATION;
			if (stations[i].clientPrio) {
				couts[iPresencePrio] += (double)PRIX_MINUTE_PRESENCE_PRIO;
			} else couts[iPresenceOrd] += (double)PRIX_MINUTE_PRESENCE_ORD;
		}
	}
}