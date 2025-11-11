#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#pragma warning(disable: 4996 6031)

enum {
	MAX_ETUDIANTS = 100,
	MAX_CHAR = 31, NB_SEMESTRES = 6,
	NB_ANNEE = 3, NB_UE = 6, UE_VALIDE_MIN = 4
};
const float NOTE_INCONNUE = -1.f;

typedef enum { S1, S2, B1, S3, S4, B2, S5, S6, B3 } Annee; //pour definir chaque semestre et annee
typedef enum { ADM, AJ, AJB, ADC, ADS } Code; //code entre parenthese a coter des notes

typedef struct {
	char prenom[MAX_CHAR];
	char nom[MAX_CHAR];
	float notes[NB_SEMESTRES + NB_ANNEE][NB_UE];
	Code codes[NB_SEMESTRES + NB_ANNEE][NB_UE];
	char statut[MAX_CHAR];
	Annee ans;
} Etudiant;

typedef struct {
	int nbEtudiants;
	Etudiant etudiants[MAX_ETUDIANTS];
} Promotion;


void affiche_annee(Annee semestre, int essay) {
	if (semestre == B1 || semestre == B2 || semestre == B3){
		printf("b (%d) ", essay);
		for (int i = 1; i < semestre / 2; ++i)
			if (i == semestre / 2 || i == semestre / 2 - 1)
				printf("B%d - (%d) ", i, essay);
	}
	else
		for (int i = 0; i < semestre; ++i) {
			if (i <= S2)
				printf("S%d - (%d) ", i + 1, essay);
			else if (B1 < i && i <= S4)
				printf("S%d - (%d) ", i, essay);
			else if (B2 < i && i <= S6)
				printf("S%d - (%d) ", i - 1, essay);

			if (i % 2 == 0) //pour S2 par exemple +1 = B1 et B1 +1 = S3 pour prochaine boucle
				++i;
		}
}

int main() {
	affiche_annee(B1, 1);
}
