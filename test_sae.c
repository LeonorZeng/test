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
typedef enum { ADM, AJ , AJB, ADC, ADS} Code; //code entre parenthese ¨¤ coter des notes

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


int Verifie_id(Promotion* promo, int id); //Test pour voir si l'etudiant est enregistrer

void inscrire(Promotion* p, const char* nom, const char* prenom); //initialise une nouvelle valeur etudiant
void etudiants(const Promotion* p); //fait defiler la liste des etudiants
void fnote(Etudiant* etudiant, int ue, float note);//ajoute la note d'un etudiant pour une UE
void cursus(Etudiant* etudiant, int id); //permet de voir tout le cursus d'un etudiant donc toutes ses notes depuis la premiere annee
void demission(Promotion* p, int id); //change le statut d'un etudiant ?demission
void defaillance(Promotion* p, int id); //change le statut d'un etudiant ?defaillance
void jury(Promotion* p, Annee s);

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
			inscrire(&p, nom, prenom);
		}

		else if (strcmp(cde, "NOTE") == 0) { // C2
			int nb, competence;
			float note;
			scanf("%u", &nb);
			scanf("%u", &competence);
			scanf("%f", &note);
			//Test pour voir si l'etudiant est enregistrer
			if (nb-1 > p.nbEtudiants || nb == p.nbEtudiants+1)
				printf("Identifiant incorrect\n");
			//Test pour voir si l'etudiant etudit toujours a l'IUT
			else if (strcmp(p.etudiants[nb-1].statut, "en cours") != 0)
				printf("Etudiant hors formation\n");
			else
				fnote(&p.etudiants[nb-1], competence, note);
		}

		else if (strcmp(cde, "CURSUS") == 0) {// C3
			int nb;
			scanf("%u", &nb);
			if (Verifie_id(&p, nb))
				cursus(&p.etudiants[nb-1], nb);
		}

		else if (strcmp(cde, "ETUDIANTS") == 0) { // C4
			etudiants(&p);
		}

		else if (strcmp(cde, "DEMISSION") == 0) { // C5
			int id;
			scanf("%d", &id);
			demission(&p, id);
		}

		else if (strcmp(cde, "DEFAILLANCE") == 0) { // C6
			int id;
			scanf("%d", &id);
			defaillance(&p, id);
		}

		else if (strcmp(cde, "JURY") == 0){ // C7

		} // TODO

		else if (strcmp(cde, "BILAN") == 0){ // C8

		} // TODO

	} while (strcmp(cde, "EXIT") != 0); // C0
}

//verifie que l'identifiant utilisateur est correct
int Verifie_id(Promotion* promo, int id) {
	if ((id < 1 || id > promo->nbEtudiants)){
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

//affiche les semestres et annees
void affiche_annee(Annee semestre) {
	switch (semestre) {
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
}

//initialise une nouvelle valeur etudiant
void inscrire(Promotion* p, const char* nom, const char* prenom) {
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
	strcpy(e->statut, "en cours");


	Init_tabNotes(p, p->nbEtudiants);


	p->nbEtudiants++;
	printf("Inscription enregistree (%d)\n", p->nbEtudiants);
}

//fait defiler la liste des etudiants
void etudiants(const Promotion* p) {
	for (int i = 0; i < p->nbEtudiants; ++i) {
		const Etudiant* e = &p->etudiants[i];
		printf("%d - %s %s - ", i + 1, e->nom, e->prenom);
		affiche_annee(e->ans);
		printf("%s\n", e->statut );
	}
}

//ajoute la note d'un etudiant pour une UE
void fnote(Etudiant* etudiant, int ue, float note) {
	//Verifie si la note et l'UE donner sont correctes
	if (ue < 1 || ue > 6)
		printf("UE incorrecte\n");
	else if (note < NOTE_MIN || note > NOTE_MAX)
		printf("Note incorrecte\n");
	else {
		etudiant->notes[etudiant->ans][ue-1] = note;
		printf("Note enregistree\n");
		//test pour savoir quel code assigner a la note
		if (note >= 10)
			etudiant->codes[etudiant->ans][ue-1] = ADM;
		else if (note < 10)
			etudiant->codes[etudiant->ans][ue-1] = AJ;
	}
}

//permet de voir le cursus d'un etudiant donc toutes ses notes depuis la premiere annee
void cursus(Etudiant* etudiant, int id) {
	printf("%u %s %s \n", id, etudiant->nom, etudiant->prenom);
	Annee semestre = etudiant->ans;
	//boucle pour afficher tous les semestres et annees
	for (Annee s = S1; s <= semestre; ++s) {
		//pour afficher semestre car Annee type enum
		affiche_annee(semestre);
		//parcours les notes par ue et annee
		for (int i = 0; i < NB_UE; ++i) {
			float note = etudiant->notes[s][i];
			Code c = etudiant->codes[s][i];
			if (note == -1)
				printf("* ");
			else
				printf("%.1f ", floorf(note * 10.f) / 10.f);
			//pour afficher c car Code type enum
			switch (c) {
			case ADM:
				printf("(ADM) - ");
				break;
			case AJ:
				printf("(AJ) - ");
				break;
			case AJB:
				printf("(AJB) - ");
				break;
			case ADC:
				printf("(ADC) - ");
				break;
			case ADS:
				printf("(ADS) - ");
				break;
			default:
				printf("(*) - ");
			}
		}
	}
	printf("%s \n", etudiant->statut);
}

//change le statut d'un etudiant a demission
void demission(Promotion* p, int id) {
	if (id < 1 || id > p->nbEtudiants) {
		printf("Identifiant incorrect\n");
		return;
	}

	Etudiant* e = &p->etudiants[id - 1]; // -1 car tableau commence a 0 et numero etudiant a 1

	if (strcmp(e->statut, "en cours") != 0) {
		printf("Etudiant hors formation\n");
		return;
	}

	strcpy(e->statut, "demission");
	printf("Demission enregistree\n");
}

//change le statut d'un etudiant a defaillance
void defaillance(Promotion* p, int id) {
	if (id < 1 || id > p->nbEtudiants) {
		printf("Identifiant incorrect\n");
		return;
	}

	Etudiant* e = &p->etudiants[id - 1]; //pointeur etudiant

	if (strcmp(e->statut, "en cours") != 0) {
		printf("Etudiant hors formation\n");
		return;
	}

	strcpy(e->statut, "defaillance");
	printf("Defaillance enregistree\n");
}

//valide les dettes des annees precedentes s'il y a dette s= annee ou il y a la dette
void rattrape_note_de_l_annne_d_avant(Code* c[], Annee s, int ue) {
	if (c[s - 2][ue] == AJ)
		c[s - 2][ue] == ADS;
	else if (c[s - 1][ue] == AJ)
		c[s - 1][ue] == ADS;
	else if (c[s][ue] == AJ)
		c[s][ue] == ADS;
}

//ajoute les codes des notes
int noteJury(Etudiant* e, Annee s, int ue, float rcue) {
	
	if (rcue >= 10){
		e->codes[s][ue] = ADM;
		//fait des tests pour les notes < 10 et > 8 qui pourront etre valider
		if (e->codes[s-2][ue] == AJ && rcue > 10)
			e->codes[s - 2][ue] == ADC;
		else if (e->codes[s - 1][ue] == AJ && rcue > 10)
			e->codes[s - 1][ue] == ADC;
		return 1;
	}
	else if (8 <= rcue && rcue < 10)
		e->codes[s][ue] = AJ;
	else if (rcue < 8) {
		e->codes[s][ue] = AJB;
		//change le statut de l'etudiant car avec une note AJB pas de passage a l'annee d'apres
		strcpy(e->statut, "ajourne");  
	}
	return 0;
}

//jury pour 1ere annee
void jury_1ereAnne(Etudiant *e, int* compte_ADM) {
	float rcue;

	for (int i = 0; i < NB_UE; ++i) { //i = ue
		rcue = (e->notes[S1][i] + e->notes[S2][i]) / 2; //calcul note de l'ue pour l'annee
		e->notes[B1][i] = rcue;//affectation de la note

		//incremente le compteur de ADM et affecte les codes aux notes
		*compte_ADM +=noteJury(e, B1, i, rcue);

	if (*compte_ADM >= 4 && strcmp(e->statut, "ajourne") != 0)
		e->ans = S3;
	}
}

//jury pour 2eme annee
void jury_2emeAnne(Etudiant* e, int *compte_ADM) {	
	int compte_valideB1 = 0;
	float rcue;

	for (int i = 0; i < NB_UE; ++i) {
		rcue = (e->notes[S3][i] + e->notes[S4][i]) / 2; //calcul note de l'ue pour l'annee
		e->notes[B2][i] = rcue; //affectation de la note

		*compte_ADM += noteJury(e, B2, i, rcue);
		if (rcue > 10) //pour remplir les dettes de l'annee passe si elles existent
			rattrape_note_de_l_annne_d_avant(e->codes, B1, i);
		if (e->codes[B1][i]!= AJ) //incremente le compteur de note valide de B1
			++compte_valideB1;

	if ((*compte_ADM >= 4 && strcmp(e->statut, "ajourne") != 0)&&compte_valideB1==6)
		e->ans = S5;
	}
}

//jury pour 3eme annee
void jury_3emeAnne(Etudiant* e, int* compte_ADM) {
	int compte_ADM = 0;
	int compte_valideB2 = 0;
	float rcue;


	for (int i = 0; i < NB_UE; ++i) {
		rcue = (e->notes[S3][i] + e->notes[S4][i]) / 2;
		e->notes[B2][i] = rcue;
		*compte_ADM += noteJury(e, B2, i, rcue);
		if (rcue > 10) //pour remplir les dettes de l'annee passe si elles existent
			rattrape_note_de_l_annne_d_avant(e->codes, B2, i);
		if (e->codes[B2][i] != AJ)
			++compte_valideB2;

	if (*compte_ADM == 6 && compte_valideB2 == 6)
		strcpy(e->statut, "diplome");
	}

}

//jury pour les semestres paires
void jury_paire(Promotion* p, Annee semestre) {
	for (int i = 0; i < p->nbEtudiants; ++i) {
		Etudiant *e = &p->etudiants[i];
		int compte_ADM = 0;
		if (strcmp(e->statut, "en cours") == 0 && e->ans == semestre){
			switch (semestre) {
			case S2:
				jury_1ereAnne(e, &compte_ADM);
				break;
			case S4:
				jury_2emeAnne(e, &compte_ADM);
				break;
			case S6:
				jury_3emeAnne(e, &compte_ADM);
				break;
			}	
		}
	}
}
