#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

#ifndef M_PI
#define M_PI						3.14159265358979323846 //https://ubuntuforums.org/showthread.php?t=583094
//#define M_PI						acos(-1.0)
#endif
#define M							(double)(UINT_MAX+1.0)
double genererReel(unsigned int a, unsigned int c, unsigned int* xn);

//suite
#define NB_XN_MAX					4
#define N							2640 //10560
#define MAX_PROBABILITES			20
#define MAX_DCARRE					20
#define I_GERME						0
#define NB_KHICARRE_THEORIQUES		19

bool obtentionEtValidation(unsigned int* a, unsigned int* c, unsigned int* x0);
void generationProbabilites(double probabilites[]);
double probabilitePre10(double x);
double probabilitePost10(double x);
bool validation(double probabilites[], int iTete, int iQueue, int repetitionsDCarre[]);
double calculKhiCarreObservable(int iQueue, int repetitionsDCarre[], double probabilites[]);

//simulation
#define VALEUR_DONNEE				5
#define NB_STATIONS_MAX				30
#define NB_STATIONS_MIN				4
#define PRIX_MINUTE_INOCCUPATION	20.0/60.0
#define PRIX_MINUTE_OCCUPATION		35.0/60.0
#define PRIX_MINUTE_PRESENCE_ORD	25.0/60.0
#define PRIX_MINUTE_PRESENCE_PRIO	40.0/60.0
#define TEMPS_SIMULATION			960
#define NB_TYPES_COUTS				4
#define N_ARRIVEES_PAR_MINUTE		58.0
#define N_REPETITIONS_DUREE_SERVICE	59.0

typedef struct station Station;
struct station {
	int numeroStation;
	bool clientPrio;
	int dureeServiceMax;
	int dureeServiceRestante;
};

typedef struct coutOptimal CoutOptimal;
struct coutOptimal {
	double cout;
	int nbStations;
};

enum { iPresencePrio, iPresenceOrd, iOccupation, iInoccupation };

void simulationSystemeFileAttente(unsigned int a, unsigned int c, unsigned int x0);
void afficherInformations(int nbArrivees, int fileOrd, int filePrio, Station stations[], int iStation);
void afficherStationsEtFiles(int fileOrd, int filePrio, Station stations[], int iStation);
void afficherCouts(double couts[], int iStation);
void gestionMinute(int iStation, unsigned int a, unsigned int c, unsigned int* x0, double couts[], int iMinute, int* fileOrd, int* filePrio, Station stations[]);
void gestionSimulation(int iStation, unsigned int a, unsigned int c, unsigned int* xn, int iMinute, int* fileOrd, int* filePrio, Station stations[]);
int genererArrivees(unsigned int a, unsigned int c, unsigned int* x0);
void gestionPrioritaires(unsigned int a, unsigned int c, unsigned int* x0, int* filePrio, Station stations[]);
void gestionStation(unsigned int a, unsigned int c, unsigned int* x0, Station stations[], int i, bool prio);
int genererDureeService(unsigned int a, unsigned int c, unsigned int* x0);
void gestionAutres(int iStation, unsigned int a, unsigned int c, unsigned int* x0, int* fileOrd, int* filePrio, Station stations[]);
void gestionCouts(int iStation, int filePrio, int fileOrd, double couts[], Station stations[]);
