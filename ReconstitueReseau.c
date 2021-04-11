#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"

void methode_liste(Chaines* C){
  Reseau* R = reconstitueReseauListe(C);
  afficher_reseau(R);

  printf("\n");
  printf("\033[1;33m");
  printf("NOMBRE DE COMMODITES DU RESEAU : %d\n", nbCommodites(R));
  printf("NOMBRE DE LIAISONS DU RESEAU : %d\n", nbLiaisons(R));
  printf("\033[0;37m");


  ecrireReseau(R, NULL);
  afficheReseauSVG(R, "reseau_affiche");
  liberer_reseau(R);
}

void methode_table(Chaines* C){

  return;
}

void methode_arbre(Chaines* C){

  return;
}

int main(int argc, char const *argv[]) {
  if (argc!=2){
    printf("Il faut un seul fichier d'entrée.\n");
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

  ecrireChaines(*pointeur_chaines, fic2); // le fichier sera fermé dans le fonction

  //afficher_liste_chaines(*pointeur_chaines);

  printf("\033[1;39m");
  printf("Choisissez la méthode de stockage de votre réseau, 1 2 ou 3:\n");
  printf("-1 : par liste chaînée\n");
  printf("-2 : par table de hachage\n");
  printf("-3 : par arbre\n");
  printf("\033[0;37m");
  int val;
  int lire=scanf("%d", &val);

  switch(val){
    case 1:
      methode_liste(*pointeur_chaines);
      break;
    case 2:
      methode_table(*pointeur_chaines);
      break;
    case 3:
      methode_arbre(*pointeur_chaines);
      break;
    default:
      printf("Votre choix est invalide\n");
      break;
  }

  printf("-------------------------------------------------\n");
  liberer_liste_chaines(*pointeur_chaines);
  free(pointeur_chaines);
  fclose(fic);
  fclose(fic2);

  return 0;
}
