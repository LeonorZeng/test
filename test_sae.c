#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#pragma warning(disable: 4996 6031)

//cr¨¦er les constantes utilisees dans le programme
enum {MAX_ETUDIANTS = 100,
	MAX_CHAR = 31, NB_SEMESTRES = 6,
	NB_ANNEE = 3, NB_UE = 6, UE_VALIDE_MIN = 4
};
const float NOTE_MIN = 0.f, NOTE_MAX = 20.f, NOTE_MOY = 10.f, NOTE_LIMITE = 8.f, NOTE_INCONNUE = -1.f;

//definition de type utilise dans le programme
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

//prototype des fonctions 
int Verifie_id(int nbEtudiants, int id); //Test si l'etudiant est enregistrer

void inscrire(Promotion* p, const char* nom, const char* prenom); //initialise un nouveau etudiant
void etudiants(const Promotion* p); //affiche la liste de tous les etudiants
void fnote(Etudiant* etudiant, int ue, float note);//ajout une note pour un UE d'un etudiant
void cursus(const Etudiant* etudiant, int id); //affiche toutes les notes d'un etudiant depuis la 1ere annee
void demission(Promotion* p, int id); //change le statut d'un etudiant a demission
void defaillance(Promotion* p, int id); //change le statut d'un etudiant a defaillance
void jury(Promotion* p, Annee s,int impair); //juge les notes des etudiants et permet le passage au semestre suivant
void bilan(const Promotion* p, int annee);

int main() {
	Promotion p;
	p.nbEtudiants = 0;

	//initialisation variable utile pour la suite
	int nb, competence, semestre;
	char nom[MAX_CHAR], prenom[MAX_CHAR];
	float note;

	//boucle qui appelle les fonctions
	char cde[MAX_CHAR] = " ";
	do {
		scanf("%s", cde);
		if (strcmp(cde, "INSCRIRE") == 0) { // C1 
			scanf("%s %s", nom, prenom);
			inscrire(&p, nom, prenom);
		}

		else if (strcmp(cde, "NOTE") == 0) { // C2
			scanf("%d", &nb);
			scanf("%d", &competence);
			scanf("%f", &note);

			//Test pour voir si l'etudiant est enregistrer
			if (nb - 1 > p.nbEtudiants || nb == p.nbEtudiants + 1)
				printf("Identifiant incorrect\n");
			//Test pour voir si l'etudiant etudit toujours a l'IUT
			else if (strcmp(p.etudiants[nb - 1].statut, "en cours") != 0)
				printf("Etudiant hors formation\n");
			else
				fnote(&p.etudiants[nb - 1], competence, note);// -1 car tableau commence a 0 et numero etudiant a 1
		}

		else if (strcmp(cde, "CURSUS") == 0) {// C3
			scanf("%d", &nb);

			//Test pour si l'etudiant enregistr¨¦
			if (Verifie_id(p.nbEtudiants, nb))
				cursus(&p.etudiants[nb - 1], nb);
		}

		else if (strcmp(cde, "ETUDIANTS") == 0) { // C4
			etudiants(&p);
		}

		else if (strcmp(cde, "DEMISSION") == 0) { // C5
			scanf("%d", &nb);

			//Test pour si l'etudiant enregistr¨¦ et en formation
			if (Verifie_id(p.nbEtudiants, nb))
				demission(&p, nb - 1);
		}

		else if (strcmp(cde, "DEFAILLANCE") == 0) { // C6
			scanf("%d", &nb);

			//Test pour si l'etudiant enregistr¨¦ et en formation
			if (Verifie_id(p.nbEtudiants, nb))
					defaillance(&p, nb - 1);
		}

		else if (strcmp(cde, "JURY") == 0) { // C7
			scanf("%d", &semestre);
			Annee s = S1;

			// On v¨¦rifie que le semestre rensigner est entre 1~6
			if (semestre < 1 || semestre > 6)
				printf("Semestre incorrect\n");
			else {
				//pour avoir le semestre qui correspond dans le tableau
				if (semestre < NB_UE - 3)
					s = semestre - 1;
				else if (semestre < NB_UE - 1)
					s = semestre;
				else if (semestre <= NB_UE)
					s = semestre + 1;
				jury(&p, s, semestre % 2);
			}
		}

		else if (strcmp(cde, "BILAN") == 0) { // C8
			int annee;
			scanf("%d", &annee);
			bilan(&p, annee);
		} // TODO

	} while (strcmp(cde, "EXIT") != 0); // C0
}

//verifie que l'identifiant utilisateur est correct
int Verifie_id(int nbEtudiants, int id) {
	if (id < 1 || id > nbEtudiants) {
		printf("Identifiant incorrect\n");
		return 0;
	}

	else
		return 1;

}

//initilialise le tableau de note d'un etudiant avec la valeur -1
void Init_tabNotes(Promotion* promo, int id) {
	Etudiant* etu = &promo->etudiants[id];
	for (Annee s = S1; s <= B3; ++s) {
		for (int i = 0; i < NB_UE; ++i) {
			etu->notes[s][i] = NOTE_INCONNUE;
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
void inscrire(Promotion* p, const char *nom, const char *prenom) {
	for (int i = 0; i < p->nbEtudiants; ++i)
		if (strcmp(p->etudiants[i].nom, nom) == 0 && strcmp(p->etudiants[i].prenom, prenom) == 0) {
			printf("Nom incorrect\n");
			return;
		}

	Etudiant* e = &p->etudiants[p->nbEtudiants]; //pointeur etudiant

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
		if (e->ans== B1 || e->ans == B2 || e->ans == B3)
			affiche_annee(e->ans-1);
		else
			affiche_annee(e->ans);
		printf("%s\n", e->statut);
	}
}

//ajoute la note d'un etudiant pour une UE
void fnote(Etudiant* etudiant, int ue, float note) {
	assert(etudiant->ans >= S1 && B3 >= etudiant->ans);
	//Verifie si la note et l'UE donner sont correctes
	if (ue < 1 || ue > NB_UE)
		printf("UE incorrecte\n");
	else if (note < NOTE_MIN || note > NOTE_MAX)
		printf("Note incorrecte\n");

	else {
		etudiant->notes[etudiant->ans][ue - 1] = note;
		printf("Note enregistree\n");
		//test pour savoir quel code assigner a la note
		if (note >= NOTE_MOY)
			etudiant->codes[etudiant->ans][ue - 1] = ADM;
		else if (note < NOTE_MOY)
			etudiant->codes[etudiant->ans][ue - 1] = AJ;
	}
}

//permet de voir le cursus d'un etudiant donc toutes ses notes depuis la premiere annee
void cursus(const Etudiant* etudiant, int id) {
	printf("%u %s %s\n", id, etudiant->nom, etudiant->prenom);
	Annee semestre = etudiant->ans;
	//boucle pour afficher tous les semestres et annees
	for (Annee s = S1; s <= semestre; ++s) {
		//pour afficher semestre car Annee type enum
		affiche_annee(s);
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
		if (s != semestre)
			printf("\n");
	}
	printf("%s\n", etudiant->statut);
}

//change le statut d'un etudiant a demission
void demission(Promotion* p, int id) {
	Etudiant* e = &p->etudiants[id]; //pointeur etudiant

	if (strcmp(e->statut, "en cours") == 0) {
		strcpy(e->statut, "demission");
		printf("Demission enregistree\n");
	}

	else 
		printf("Etudiant hors formation\n");
}

//change le statut d'un etudiant a defaillance
void defaillance(Promotion* p, int id) {

	Etudiant* e = &p->etudiants[id]; //pointeur etudiant

	if (strcmp(e->statut, "en cours") == 0) {
		strcpy(e->statut, "defaillance");
		printf("Defaillance enregistree\n");
	}

	else 
		printf("Etudiant hors formation\n");
	
}

int codeJury(Etudiant* e, Annee s, int ue, float rcue) {

	if (rcue >= NOTE_MOY) {
		e->codes[s][ue] = ADM;
		//fait des tests pour les notes < 10 et > 8 qui pourront etre valider
		if (e->codes[s - 2][ue] == AJ)
			e->codes[s - 2][ue] = ADC;
		else if (e->codes[s - 1][ue] == AJ)
			e->codes[s - 1][ue] = ADC;
		return 1;
	}
	else if (NOTE_LIMITE <= rcue && rcue < NOTE_MOY)
		e->codes[s][ue] = AJ;

	else if (rcue < NOTE_LIMITE) {
		e->codes[s][ue] = AJB;
		//change le statut de l'etudiant car avec une note AJB pas de passage a l'annee d'apres
		strcpy(e->statut, "ajourne");
	}
	return 0;
}

//jury pour 1ere annee
void jury_1ereAnne(Etudiant* e, int* compte_valide) {
	float rcue;

	for (int i = 0; i < NB_UE; ++i) { //i = ue
		rcue = (e->notes[S1][i] + e->notes[S2][i]) / 2; //calcul note de l'ue pour l'annee
		e->notes[e->ans][i] = rcue;//affectation de la note

		//incremente le compteur de ADM et affecte les codes aux notes
		*compte_valide += codeJury(e, e->ans, i, rcue);
	}
	if (*compte_valide >= UE_VALIDE_MIN && strcmp(e->statut, "ajourne") != 0)
		++e->ans; //passage en S3
	else
		strcpy(e->statut, "ajourne");
}


int jury_nemeAnne(Etudiant* e, Annee B, int* compte_valide) {
	int cpt_valide_annee_prec = 0; //compte_valide pour annee precedente
	float rcue;

	for (int i = 0; i < NB_UE; ++i) {
		rcue = (e->notes[B - 2][i] + e->notes[B - 1][i]) / 2;
		e->notes[B][i] = rcue;
		*compte_valide += codeJury(e, B, i, rcue);

		if (rcue > NOTE_MOY && e->codes[B - 3][i] == AJ) { //pour remplir les dettes de l'annee passe si elles existent 
			e->codes[B - 3][i] = ADS;
			if (e->codes[B - 4][i] == AJ)
				e->codes[B - 4][i] = ADS;
			if (e->codes[B - 5][i] == AJ)
				e->codes[B - 5][i] = ADS;
		}

		if (e->codes[B - 3][i] != AJ)
			++cpt_valide_annee_prec;

	}
	if (*compte_valide >= UE_VALIDE_MIN && strcmp(e->statut, "ajourne") != 0 && cpt_valide_annee_prec == NB_UE)
		return 1;
	else{
		strcpy(e->statut, "ajourne");
		return 0;
	}
}


void jury_pair(Etudiant* e, Annee semestre) {
	int compte_valide = 0; //compte le nb de note valider
	++e->ans; // ans = B.. quelque chose

	switch (semestre) {
	case S2:
		jury_1ereAnne(e, &compte_valide);
		break;
	case S4:
		if (jury_nemeAnne(e, e->ans, &compte_valide))
			++e->ans;
		break;
	case S6:
		if (jury_nemeAnne(e, e->ans, &compte_valide) && compte_valide == NB_UE)
			strcpy(e->statut, "diplome");
		else
			strcpy(e->statut, "ajourne");
		break;
	}
}

int verifie_si_noteManquante(const Promotion* p, const Annee s, int* cpt_etu) {
	for (int i = 0; i < p->nbEtudiants; ++i)//parcours tous les etudiants

		//regarde si l'etudiant en formation et s'il est du semestre demander
		if (strcmp(p->etudiants[i].statut, "en cours") == 0 && p->etudiants[i].ans == s) {
			for (int ue = 0; ue < NB_UE; ++ue) // et regarder les ue 

				if (p->etudiants[i].notes[s][ue] == NOTE_INCONNUE) {
					//pour voir s'il y a une note manquante
					printf("Des notes sont manquantes\n");
					return 0;
				}
			++ * cpt_etu;
		}
	return 1;
}

void jury(Promotion* p, Annee s, int impair) {
	//initialise des variables dont on a besoin
	int cpt_etu = 0;
	if (verifie_si_noteManquante(p, s, &cpt_etu) != 0) {
		printf("Semestre termine pour %d etudiant(s)\n", cpt_etu);

		for (int i = 0; i < p->nbEtudiants; ++i) { //parcours tous les etudiants
			Etudiant* e = &p->etudiants[i];

			if (strcmp(e->statut, "en cours") == 0 && e->ans == s) {
				// Si le semestre est impair ¡ú appelle jury_impair
				if (impair)
					e->ans += 1;

				// Si le semestre est pair ¡ú appelle jury_pair
				else if (!impair)
					jury_pair(e, s);
			}
		}
	}
}

void bilan(const Promotion* p, int annee) {
	if (annee < 1 || annee > 3) {
		printf("Annee incorrecte\n");
		return;
	}

	int dem = 0, def = 0, encours = 0, aj = 0, passe = 0;
	printf("BILAN %d\n", annee);
	//parcours des etudiants
	for (int i = 0; i < p->nbEtudiants; ++i) {
		const Etudiant* e = &p->etudiants[i];

		// On détermine à quelle année appartient l’étudiant
		int anneeEtu = 0;
		if (e->ans <= B1) 
			anneeEtu = 1;
		else if (e->ans <= B2) 
			anneeEtu = 2;
		else anneeEtu = 3;

		// S’il n’a pas encore atteint cette année, on l’ignore
		if (anneeEtu >= annee) {

			// Maintenant on compte selon le statut
			if (strcmp(e->statut, "demission") == 0)
				if (anneeEtu == annee) 
					dem++;
			else if (strcmp(e->statut, "defaillance") == 0)
				if (anneeEtu == annee) 
					def++;
			else if (strcmp(e->statut, "ajourne") == 0)
				if (anneeEtu == annee) 
					aj++;
			else if (strcmp(e->statut, "diplome") == 0) 
				if (annee == 3) 
					passe++;
			else if (strcmp(e->statut, "en cours") == 0) {
				if (anneeEtu == annee)
					encours++;
				else if (anneeEtu > annee)
					passe++;
			}
		}
	}
	// On affiche le résultat final
	printf("%d demission(s)\n", dem);
	printf("%d defaillance(s)\n", def);
	printf("%d en cours\n", encours);
	printf("%d ajourne(s)\n", aj);
	printf("%d passe(s)\n", passe);
}
