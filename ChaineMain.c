#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"
#include "ArbreQuat.h"

int main(int argc, char const *argv[]) {
  if (argc!=2){
    printf("Il faut un seul fichier d'entrée et un seul entier > 0.\n");
    return 1;
  }

  // test de la fonction lectureChaines
  FILE* fic ;
  fic=fopen(argv[1],"r");
  if (!fic){
    printf("Problème lors de la lecture du fichier\n");
    return 1;
  }
  printf("fichier ouvert avec succès\n\n");
  Chaines** pointeur_chaines=(Chaines**)malloc(sizeof(Chaines*));
  *pointeur_chaines = lectureChaines(fic);

  // test de la fonction ecrireChaines
  FILE* fic2;
  fic2=fopen("ecrireChaines1.cha", "w");
  if (!fic2){
    printf("Problème lors de la lecture du fichier\n");
    fclose(fic2);
    return 1;
  }
  printf("fichier ouvert avec succès\n\n");

  ecrireChaines(*pointeur_chaines, fic2); // le fichier sera fermé dans la fonction
  afficher_liste_chaines(*pointeur_chaines);

  afficheChainesSVG(*pointeur_chaines, "chaines1");
  printf("\033[1;36m");
  printf("LONGUEUR TOTALE DE LA CHAINE:   %f\n", longueurTotale(*pointeur_chaines));
  printf("NOMBRE TOTAL DE POINTS:         %d\n", comptePointsTotal(*pointeur_chaines));
  printf("\033[1;37m");

/*  double a, b, c, d;
  chaineCoordMinMax(*pointeur_chaines, &a, &b, &c, &d);*/

/*  CellNoeud* n1=(CellNoeud*)calloc(1,sizeof(CellNoeud));
  n1->nd=creer_noeud(1, 16.53, 97.38);
  inserer_noeud_en_tete(&n1, 2, 25.23, 97.24);
  inserer_noeud_en_tete(&n1, 3, 20.09, 94.55);
  inserer_noeud_en_tete(&n1, 4, 17.20, 96.29);
  inserer_noeud_en_tete(&n1, 5, 16.30, 97.38);
  inserer_noeud_en_tete(&n1, 6, 21.52, 95.59);
  inserer_noeud_en_tete(&n1, 7, 14.05, 98.12);
  inserer_noeud_en_tete(&n1, 8, 16.47, 94.44);
  inserer_noeud_en_tete(&n1, 9, 22.00, 96.05);
  inserer_noeud_en_tete(&n1, 10, 22.39, 93.37);
  ArbreQuat* aq=creerArbreQuat(a*2, b*2, c*2, d*2);
  CellNoeud* cour=n1;
  while(cour){
    insererNoeudArbre(cour->nd, &aq, aq);
    cour=cour->suiv;
  }
  afficher_aq(aq);
  liberer_aq(aq);
  liberer_liste_noeuds(n1);*/

/*  Reseau* R = creer_reseau(0, 0, NULL, NULL);
  ArbreQuat* aq = creerArbreQuat(a*2, b*2, c*2, d*2);
  printf("là\n");
  rechercheCreeNoeudArbre(R, &aq, aq, 25.23, 97.24);
  rechercheCreeNoeudArbre(R, &aq, aq, 20.09, 94.55);
  rechercheCreeNoeudArbre(R, &aq, aq, 17.20, 96.29);
  rechercheCreeNoeudArbre(R, &aq, aq, 16.30, 97.38);
  rechercheCreeNoeudArbre(R, &aq, aq, 25.23, 97.24);

  afficher_aq(aq);
  afficher_liste_noeuds(R->noeuds);*/


  liberer_liste_chaines(*pointeur_chaines);
  free(pointeur_chaines);
  fclose(fic);
  fclose(fic2);

  return 0;
}
