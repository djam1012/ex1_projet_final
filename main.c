#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "Reseau.h"
#include "Chaine.h"
#include "Hachage.h"
#include "ArbreQuat.h"
#include "SVGwriter.h"

Chaines* generationAleatoire(int nbChaines, int nbPointsChaine, int xmax, int ymax){
  srand(time(NULL));
  Chaines* C=(Chaines*)malloc(sizeof(Chaines));
  C->gamma=0; C->nbChaines=nbChaines;
  int i=0;
  while (i<nbChaines){
    CellPoint* points;
    int nbPtsRand=rand()%nbPointsChaine+1;
    int x, y;
    int j=0;
    while (j<nbPtsRand){
      j++;
      x=(double)(rand()%(xmax+1));
      y=(double)(rand()%(ymax+1));
      inserer_point_en_tete(&points, x, y);
    }
    CellChaine* chaine=creer_chaine(i, points);
    ajouter_chaine_en_tete(&(C->chaines), &chaine);
    points=NULL;
    i++;
  }
  afficher_liste_chaines(C);
  return C;
}

int main(int argc, char const *argv[]) {
  if (argc!=2){
    printf("Il faut un seul fichier d'entrée.\n");
    return 1;
  }

  FILE* entree ;
  entree=fopen(argv[1],"r");
  if (!entree){
    printf("Problème lors de la lecture du fichier \n");
    return 1;
  }

  printf("fichier ouvert avec succès\n\n");

  Chaines* C = lectureChaines(entree);
  FILE *sortie=fopen("temps_cpu.txt","w");
  if (!sortie){
    printf("Problème lors de l'ouverture du fichier \n");
    return 1;
  }
  clock_t temps_initial;
  clock_t temps_final;
  double temps_cpu;
  /*Reseau* R;


  printf("Recherche dans une liste : \n");
  fprintf(sortie,"Recherche dans une liste : \n");
  temps_initial=clock();
  R=reconstitueReseauListe(C);
  temps_final=clock();
  temps_cpu=((double)(temps_final-temps_initial));
  printf("%f\n",temps_cpu);
  fprintf(sortie,"%f\n",temps_cpu);
  int taille=R->nbNoeuds+30;
  liberer_reseau(R);

  int i;
  printf("Recherche dans une table de hachage de tailles différentes : \n");
  fprintf(sortie,"Recherche dans une table de hachage de tailles différentes : \n");
  for(i=1;i<=taille;i++){
    temps_initial=clock();
    R=reconstitueReseauHachage(C,i);
    temps_final=clock();
    temps_cpu=((double)(temps_final-temps_initial));
    printf("%f\t%d\n",temps_cpu,i);
    fprintf(sortie,"%f\t%d\n",temps_cpu,i);
    liberer_reseau(R);
  }

  printf("Recherche dans un arbre quaternaire : \n");
  fprintf(sortie,"Recherche dans un arbre quaternaire : \n");
  temps_initial=clock();
  R=reconstitueReseauArbre(C);
  temps_final=clock();
  temps_cpu=((double)(temps_final-temps_initial));
  printf("%f\n",temps_cpu);
  fprintf(sortie,"%f\n",temps_cpu);
  liberer_reseau(R);*/


  fclose(entree);
  fclose(sortie);
  liberer_liste_chaines(C);

  Chaines* random_cha=generationAleatoire(200, 10, 3000, 6000);
  liberer_liste_chaines(random_cha);
  return 0;
}
