#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
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

/* Peut-être pas utile car on compare directement la chaine de caractère dans le main pour vérifier si on quitte
void EXIT() {
	EXIT(0);
}*/

void INSCRIRE(char nom[], char prenom[]) {
	for (int i = 0; i < nbEtudiants; i++) {
		if (strcmp(etudiant[i].nom, nom) == 0 && strcmp(etudiant[i].prenom, prenom) == 0) {
			printf("Nom incorrect\n");
		}
	}
}

void ETUDIANT;







void CURSUS(Etudiant* etudiant, int id);
void NOTE(Etudiant* etudiant, int ue, float note);

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
			inscrire(nom, prenom);

		}

		else if (strcmp(cde, "NOTE") == 0) { // C2
			int nb, competence;
			float note;
			//Des scanf pour recuperer les valeurs envoyer par l'utilisateur
			scanf("%u", &nb);
			scanf("%u", &competence);
			scanf("%f", &note);
			//Test pour voir si l'etudiant est enregistrer
			if (nb > p.nbEtudiants)
				printf("Identifiant incorrect");
			//Test pour voir si l'étudiant etudit toujours à l'IUT
			else if (strcmp(p.etudiants[nb].etat, "en cours") != 0)
				printf("Etudiant hors formation");
			else
				NOTE(&p.etudiants[nb], competence, note);
		}

		else if (strcmp(cde, "CURSUS") == 0) {// C3
			int nb;
			scanf("%u", &nb);
			//Test pour voir si l'etudiant est enregistrer
			//Peut-être creer une fonction pour ça
			if (nb > p.nbEtudiants)
				printf("Identifiant incorrect");
			else
				CURSUS(&p.etudiants[nb], nb);
		}

		else if (strcmp(cde, "ETUDIANTS") == 0) // C4
		{
		} // TODO
		else if (strcmp(cde, "DEMISSION") == 0) // C5
		{
		} // TODO
		else if (strcmp(cde, "DEFAILLANCE") == 0) // C6
		{
		} // TODO
		else if (strcmp(cde, "JURY") == 0) // C7
		{
		} // TODO
		else if (strcmp(cde, "BILAN") == 0) // C8
		{
		} // TODO
	} while (strcmp(cde, "EXIT") != 0); // C0
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

//ajoute la note d'un etudiant pour une UE
void NOTE(Etudiant* etudiant, int ue, float note) {
	//Verifie si la note et l'UE donner sont correctes (peut-être le mettre dans le main)
	if (ue < 1 || ue > 6)
		printf("UE incorrecte");
	else if (note < NOTE_MIN || note > NOTE_MAX)
		printf("Note incorrecte");
	else {
		etudiant->notes[etudiant->ans][ue] = note;
		printf("Note enregistree");
	}
}

//permet de voir tout le cursus d'un etudiant donc toutes ses notes depuis la premiere annee
void CURSUS(Etudiant* etudiant, int id) {
	printf("%u %s %s \n", id, etudiant->nom, etudiant->prenom);
	Annee semestre = etudiant->ans;
	Annee s = S1;
	for (; s <= semestre; ++s) {
		for (int i = 0; i < NB_UE; ++i) {
			float note = etudiant->notes[s][i];
			Code c = etudiant->codes[s][i];
			if (note == -1)
				printf("* - ");
			else
				printf("%.1f - ", floorf(note * 10.f) / 10.f);
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
