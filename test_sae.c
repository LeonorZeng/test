#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#pragma warning(disable: 4996 6031)

enum {
	MAX_ETUDIANTS = 100,
	NB_UE = 6, NB_SEMESTRES = 6,
	MAX_CHAR = 31, NB_ANNEE = 3
};
const float NOTE_MIN = 0.f, NOTE_MAX = 20.f;

typedef enum { S1, S2, B1, S3, S4, B2, S5, S6, B3 } Annee; //pour definir chaque semestre et annee
typedef enum { ADM, AJ } Code; 

typedef struct {
	char prenom[MAX_CHAR];
	char nom[MAX_CHAR];
	float notes[NB_SEMESTRES + NB_ANNEE][NB_UE];
	Code codes[NB_SEMESTRES + NB_ANNEE][NB_UE];
	char etat[MAX_CHAR];
	Annee ans;
} Etudiant;

typedef struct {
	int nbEtudiants;
	Etudiant etudiants[MAX_ETUDIANTS];
} Promotion;


int Verifie_id(Promotion* promo, int id); //verifie que l'identifiant utilisateur est correct

void INSCRIRE(Promotion* p, const char* nom, const char* prenom); //initialise une nouvelle valeur etudiant
void ETUDIANTS(const Promotion* p); //fait defiler la liste des etudiants
void fNOTE(Etudiant* etudiant, int ue, float note);//ajoute la note d'un etudiant pour une UE
void CURSUS(Etudiant* etudiant, int id); //permet de voir tout le cursus d'un etudiant donc toutes ses notes depuis la premiere annee
void DEMISSION(Promotion* p, int id); //change le statut d'un etudiant à demission
void DEFAILLANCE(Promotion* p, int id); //change le statut d'un etudiant à defaillance

int main() {
	Promotion p;
	p.nbEtudiants = 0;

	char cde[MAX_CHAR] = " ";
	do {
		scanf("%s", cde);
		if (strcmp(cde, "INSCRIRE") == 0) { // C1 
			char nom[MAX_CHAR];
			char prenom[MAX_CHAR];
			scanf("%s %s", nom, prenom);
			INSCRIRE(&p, nom, prenom);
		}

		else if (strcmp(cde, "NOTE") == 0) { // C2
			int nb, competence;
			float note;
			//Des scanf pour recuperer les valeurs envoyer par l'utilisateur
			scanf("%u", &nb);
			scanf("%u", &competence);
			scanf("%f", &note);
			//Test pour voir si l'etudiant est enregistrer
			if (nb-1 > p.nbEtudiants || nb == p.nbEtudiants+1)
				printf("Identifiant incorrect\n");
			//Test pour voir si l'étudiant etudit toujours à l'IUT
			else if (strcmp(p.etudiants[nb-1].etat, "en cours") != 0)
				printf("Etudiant hors formation\n");
			else
				fNOTE(&p.etudiants[nb-1], competence, note);
		}

		else if (strcmp(cde, "CURSUS") == 0) {// C3
			int nb;
			scanf("%u", &nb);
			//Test pour voir si l'etudiant est enregistrer
			//Peut-être creer une fonction pour ça
			if (Verifie_id(&p, nb))
				CURSUS(&p.etudiants[nb-1], nb);
		}
		else if (strcmp(cde, "ETUDIANTS") == 0) { // C4
			ETUDIANTS(&p);
		}

		else if (strcmp(cde, "DEMISSION") == 0) { // C5
			int id;
			scanf("%d", &id);
			DEMISSION(&p, id);
		}

		else if (strcmp(cde, "DEFAILLANCE") == 0) { // C6
			int id;
			scanf("%d", &id);
			DEFAILLANCE(&p, id);
		}

		else if (strcmp(cde, "JURY") == 0) // C7
		{
		} // TODO
		else if (strcmp(cde, "BILAN") == 0) // C8
		{
		} // TODO
	} while (strcmp(cde, "EXIT") != 0); // C0
}

//verifie que l'identifiant utilisateur est correct
int Verifie_id(Promotion* promo, int id) {
	if (id - 1 > promo->nbEtudiants || id == promo->nbEtudiants + 1){
		printf("Identifiant incorrect\n");
		return 0;
	}
	else
		return 1;
}

//initilialise le tableau de note d'un etudiant avec la valeur -1
void Init_tabNotes(Promotion* promo, int nb) {
	Etudiant* etu = &promo->etudiants[nb];
	for (Annee s = S1; s <= B3; ++s) {
		for (int i = 0; i < NB_UE; ++i) {
			etu->notes[s][i] = -1;
		}
	}
}

//initialise une nouvelle valeur etudiant
void INSCRIRE(Promotion* p, const char* nom, const char* prenom) {
	for (int i = 0; i < p->nbEtudiants; ++i) {
		if (strcmp(p->etudiants[i].nom, nom) == 0 && strcmp(p->etudiants[i].prenom, prenom) == 0) {
			printf("Nom incorrect\n");
			return;
		}
	}

	Etudiant* e = &p->etudiants[p->nbEtudiants];

	strcpy(e->nom, nom);
	strcpy(e->prenom, prenom);
	e->ans = S1;
	strcpy(e->etat, "en cours");


	Init_tabNotes(p, p->nbEtudiants);


	p->nbEtudiants++;
	printf("Inscription enregistree (%d)\n", p->nbEtudiants);
}

//fait defiler la liste des etudiants
void ETUDIANTS(const Promotion* p) {
	for (int i = 0; i < p->nbEtudiants; ++i) {
		const Etudiant* e = &p->etudiants[i];
		printf("%d - %s %s - %s\n", i + 1, e->nom, e->prenom, e->etat);
	}
}

//ajoute la note d'un etudiant pour une UE
void fNOTE(Etudiant* etudiant, int ue, float note) {
	//Verifie si la note et l'UE donner sont correctes (peut-être le mettre dans le main)
	if (ue < 1 || ue > 6)
		printf("UE incorrecte\n");
	else if (note < NOTE_MIN || note > NOTE_MAX)
		printf("Note incorrecte\n");
	else {
		etudiant->notes[etudiant->ans][ue-1] = note;
		printf("Note enregistree\n");
		if (note >= 10)
			etudiant->codes[etudiant->ans][ue-1] = ADM;
		else if (note < 10)
			etudiant->codes[etudiant->ans][ue-1] = AJ;
	}
}

//permet de voir tout le cursus d'un etudiant donc toutes ses notes depuis la premiere annee
void CURSUS(Etudiant* etudiant, int id) {
	printf("%u %s %s \n", id, etudiant->nom, etudiant->prenom);
	Annee semestre = etudiant->ans;
	for (Annee s = S1; s <= semestre; ++s) {
		switch (semestre){
		case S1:
			printf("S1 - ");
			break;
		case S2:
			printf("S2 - ");
			break;
		case B1:
			printf("B1 - ");
			break;
		case S3:
			printf("S3 - ");
			break;
		case S4:
			printf("S4 - ");
			break;
		case B2:
			printf("B2 - ");
			break;
		case S5:
			printf("S5 - ");
			break;
		case S6:
			printf("S6 - ");
			break;
		case B3:
			printf("B3 - ");
			break;
		}
		for (int i = 0; i < NB_UE; ++i) {
			float note = etudiant->notes[s][i];
			Code c = etudiant->codes[s][i];
			if (note == -1)
				printf("* ");
			else
				printf("%.1f ", floorf(note * 10.f) / 10.f);
			switch (c) {
			case ADM:
				printf("(ADM) - ");
				break;
			case AJ:
				printf("(AJ) - ");
				break;
			default:
				printf("(*) - ");
			}
		}
	}
	printf("%s \n", etudiant->etat);
}

//change le statut d'un etudiant à demission
void DEMISSION(Promotion* p, int id) {
	if (id < 1 || id > p->nbEtudiants) {
		printf("Identifiant incorrect\n");
		return;
	}

	Etudiant* e = &p->etudiants[id - 1]; // -1 car tableau commence à 0

	if (strcmp(e->etat, "en cours") != 0) {
		printf("Etudiant hors formation\n");
		return;
	}

	strcpy(e->etat, "demission");
	printf("Demission enregistree\n");
}

//change le statut d'un etudiant à defaillance
void DEFAILLANCE(Promotion* p, int id) {
	if (id < 1 || id > p->nbEtudiants) {
		printf("Identifiant incorrect\n");
		return;
	}

	Etudiant* e = &p->etudiants[id - 1];

	if (strcmp(e->etat, "en cours") != 0) {
		printf("Etudiant hors formation\n");
		return;
	}

	strcpy(e->etat, "defaillance");
	printf("Defaillance enregistree\n");
}

void jury_paire(Promotion* p, Annee semestre) {
	for (int i = 0; i < p->nbEtudiants; ++i) {

	}
}