#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"
#include "Reseau.h"
#include "SVGwriter.h"

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

  ecrireChaines(*pointeur_chaines, fic2); // le fichier sera fermé dans le fonction

  Reseau* R = reconstitueReseauListe(*pointeur_chaines);
  afficher_reseau(R);

  liberer_liste_chaines(*pointeur_chaines);
  free(pointeur_chaines);
  fclose(fic);
  fclose(fic2);

  return 0;
}
