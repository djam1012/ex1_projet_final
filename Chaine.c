#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"

void inserer_point_en_tete(CellPoint* liste_points, double x, double y){
  CellPoint* nouv=(CellPoint*)malloc(sizeof(CellPoint));
  nouv->x=x;
  nouv->y=y;
  if (liste_points==NULL){
    liste_points=nouv;
  } else {
    nouv->suiv=liste_points;
    liste_points=nouv;
  }
}

void inserer_chaine_en_tete(CellChaine* liste_chaine, int numero, CellPoint* liste_points){
  CellChaine* nouv=(CellChaine*)malloc(sizeof(CellChaine));
  nouv->numero=numero;
  nouv->points=liste_points;
  if (liste_chaine==NULL){
    liste_chaine=nouv;
  } else {
    nouv->suiv=liste_chaine;
    liste_chaine=nouv;
  }
}

void liberer_points(CellPoint* points){
  if (!points){
    printf("Impossible de libérer une liste de points vide.\n");
    return;
  }
  CellPoint* cour;
  if(points!=NULL){
      cour=points->suiv;
      free(points);
      points=cour;
      liberer_points(points);
  }
}

void liberer_chaine(CellChaine* chaine){
  if (!chaine){
    printf("Impossible de libérer une liste de chaines vide.\n");
    return;
  }
  CellChaine* cour;
  if(chaine!=NULL){
      cour=chaine->suiv;
      liberer_points(chaine->points);
      free(chaine);
      chaine=cour;
      liberer_chaine(chaine);
  }
}

void liberer_liste_chaines(Chaines* liste_chaines){
  if (!liste_chaines){
    printf("Impossible de libérer une liste de liste de chaines vide.\n");
    return;
  }
  liberer_chaine(liste_chaines->chaines);
}

Chaines* lectureChaines(FILE *f){

  // on alloue les structures nécessaires
  Chaines* liste_chaines=(Chaines*)malloc(sizeof(Chaines));
  CellChaine* chaine=(CellChaine*)malloc(sizeof(CellChaine));

  // on crée les variables temporaires permettant de lire chaque donnée du fichier
  char x[10]; // abcisse
  char y[10]; // ordonnée
  char gamma[10];
  char nbChaines[10];
  char ligne[256]; // ligne à lire dans le fichier
  int num_ligne=0;

  while(f){
    for (int c=0; c<10; c++){
      gamma[c]='\0';
      nbChaines[c]='\0'; // on les réinitialise
    }

    CellPoint* point=(CellPoint*)malloc(sizeof(CellPoint));
    char* temp=fgets(ligne, 256, f); // on se place su la ligne courante

    if (ligne[0]=='N'){
      strncat(nbChaines, &ligne[9], 1);
      liste_chaines->nbChaines=atoi(nbChaines);
      num_ligne++;
    }
    else if (ligne[0]=='G'){
      strncat(gamma, &ligne[7], 1);
      liste_chaines->gamma=atoi(gamma);
      num_ligne++;
    }
    else if (num_ligne==2){
      num_ligne=5;
      continue; // on ignore le saut de ligne sinon la boucle s'arrête
    }
    else {
      int a_o=0; // 0 = on lit une abcisse, 1 = on lit une ordonnée
      int nb_pts=atoi(&ligne[2]); // nombre de coordonnées à lire dans la ligne
      int j=4; // caractère à partir duquel on commence à lire des points
      int lire_nb_pts=0; // savoir si on passe à la chaine suivante si on atteint ce nombre


      //printf("NB POINTS VAUT %d\n", nb_pts);
      while(ligne){ // lecture de 1 ligne
        if (feof(f)) return NULL; // condition_sortie doit etre egal a la somme de tous les nb de pts de chaque ligne moins le nombre de chaines
        for (int c=0; c<10; c++){
            x[c]='\0';
            y[c]='\0'; // on les réinitialise quand on passe au point suivant
        }
        int lire_pts=0; // on lit chaque caractère de l'abcisse ou de l'ordonnée
        while(ligne[j]!=' '&&ligne[j]!='\0'&&ligne[j]!='\n'){
          //printf("j1 vaut %d\n", j);
          strncat(x, &ligne[j], 1);
          //printf("  %c\n", ligne[j]);
          j++;
        }
        if (ligne[j]!='\0')j++;
        //printf("j2 vaut %d\n", j);

        while(ligne[j]!=' '&&ligne[j]!='\0'&&ligne[j]!='\n'){
          //printf("j3 vaut %d\n", j);
          strncat(y, &ligne[j], 1);
          //printf("  %c\n", ligne[j]);
          j++;
        }
        if (ligne[j]!='\0')j++;
        //printf("j4 vaut %d\n", j);
        printf("          %s\n", x);
        printf("          %s\n", y);
        printf("\n");
        lire_nb_pts++;
        //printf("LIRE NB POINTS VAUT %d\n", lire_nb_pts);
        if (lire_nb_pts==nb_pts) break; // on peut couper la boucle lorsqu'on a lu le nombre de points qu'on voulait
        if (ligne[j]=='\0') break;

        inserer_point_en_tete(point, atof(x), atof(y));
      }
      char numero[10];
      strncat(numero, &ligne[0], 10);
      inserer_chaine_en_tete(chaine, atoi(numero), point);
    }
  }
  liste_chaines->chaines=chaine;
  return liste_chaines;
}

int main(int argc, char const *argv[]) {
  FILE* fic ;
  fic=fopen(argv[1],"r");
  if (!fic){
    printf("Problème lors de la lecture du fichier\n");
    return 1;
  }
  printf("fichier lu avec succès\n");
  Chaines* ch1 = lectureChaines(fic);
  fclose(fic);
  liberer_liste_chaines(ch1);
  return 0;
}
