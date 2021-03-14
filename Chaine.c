#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"

CellPoint* creer_point(double x, double y){
  CellPoint* nouv=(CellPoint*)malloc(sizeof(CellPoint));
  nouv->x=x;
  nouv->y=y;
  nouv->suiv=NULL;
  return nouv;
}

CellChaine* creer_chaine(int numero, CellPoint* liste_points){
  CellChaine* nouv=(CellChaine*)malloc(sizeof(CellChaine));
  nouv->numero=numero;
  nouv->points=liste_points;
  nouv->suiv=NULL;
  return nouv;
}

void inserer_point_en_tete(CellPoint** liste_points, double x, double y){
  CellPoint* nouv=creer_point(x,y);
  if (*liste_points==NULL){
    *liste_points=nouv;
  } else {
    nouv->suiv=*liste_points;
    *liste_points=nouv;
  }
}

void inserer_chaine_en_tete(CellChaine** liste_chaine, int numero, CellPoint* liste_points){
  CellChaine* nouv=(CellChaine*)malloc(sizeof(CellChaine));
  nouv->numero=numero;
  nouv->points=liste_points;
  if (*liste_chaine==NULL){
    *liste_chaine=nouv;
  } else {
    nouv->suiv=*liste_chaine;
    *liste_chaine=nouv;
  }
}

void liberer_points(CellPoint* points){
  if (!points){
    printf("La liste de points est vide.\n");
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
    printf("La liste de chaines est vide.\n");
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

void afficher_points(CellPoint* liste_points){
  if (!liste_points){
    printf("Liste de points vide, impossible de l'afficher.\n");
    return;
  }
  CellPoint* cour=liste_points;
  while(cour){
    printf("Point de coodonnées %.2f et %.2f.\n", cour->x, cour->y);
    if(liste_points->suiv==NULL) return;
    cour=cour->suiv;
  }
  printf("\n");
}

void afficher_chaine(CellChaine* liste_chaine){
  if (!liste_chaine){
    printf("La liste de chaines est vide.\n");
    return;
  }
  CellChaine* cour=liste_chaine;
  while(cour){
    printf("Chaine numéro: %d avec les points:\n", cour->numero);
    afficher_points(cour->points);
    if(liste_chaine->suiv==NULL) return;
    cour=cour->suiv;
  }
  printf("\n");
}

void afficher_liste_chaines(Chaines* liste_chaines){
  if (!liste_chaines){
    printf("Les chaines sont vides.\n");
    return;
  }
  printf("Gamma: %d Nombre de chaines: %d Liste des chaines:\n", liste_chaines->gamma, liste_chaines->nbChaines);
  afficher_chaine(liste_chaines->chaines);
}

void liberer_liste_chaines(Chaines* liste_chaines){
  if (!liste_chaines){
    printf("Impossible de libérer une liste de liste de chaines vide.\n");
    return;
  }
  liberer_chaine(liste_chaines->chaines);
  free(liste_chaines);
}

Chaines* lectureChaines(FILE *f){
  // on alloue les structures nécessaires
  Chaines* liste_chaines=(Chaines*)malloc(sizeof(Chaines));
  CellChaine** chaine=(CellChaine**)malloc(sizeof(CellChaine*));

  // on crée les variables temporaires permettant de lire chaque donnée du fichier
  char x[10]; // abcisse
  char y[10]; // ordonnée
  char gamma[10]; // gamma
  char nbChaines[10]; // nombre de chaines
  char ligne[256]; // ligne à lire dans le fichier
  int num_ligne=0; // numero de la ligne
  int premier_point=0; // teste si le point à insérer est le premier, alors il crée un nouveau point

  while(f){
    for (int c=0; c<10; c++){
      gamma[c]='\0';
      nbChaines[c]='\0'; // on les réinitialise
    }

    char* temp=fgets(ligne, 256, f); // on se place su la ligne courante

    if (ligne[0]=='N'){ // on enregistre le nombre de chaines
      strncat(nbChaines, &ligne[9], 1);
      liste_chaines->nbChaines=atoi(nbChaines);
      num_ligne++;
    }
    else if (ligne[0]=='G'){ // on enregistre gamma
      strncat(gamma, &ligne[7], 1);
      liste_chaines->gamma=atoi(gamma);
      num_ligne++;
    }
    else if (num_ligne==2){
      num_ligne=5;
      continue; // on ignore le saut de ligne de la ligne 2 sinon la boucle s'arrête
    }
    else {
      int nb_pts=atoi(&ligne[2]); // nombre de coordonnées à lire dans la ligne
      int j=4; // caractère à partir duquel on commence à lire des points
      int lire_nb_pts=0; // on passe à la chaine suivante si on atteint le nombre de points à lire
      CellPoint** point=(CellPoint**)malloc(sizeof(CellPoint*));

      while(ligne){ // lecture de 1 ligne

        if (feof(f)) break; // condition pour sortir de la boucle car ça ne marche pas lorsqu'on le met pas (boucle infinie)
        for (int c=0; c<10; c++){// on les réinitialise quand on passe au point suivant
            x[c]='\0';
            y[c]='\0';
        }
        int lire_pts=0; // on lit chaque caractère de l'abcisse ou de l'ordonnée

        while(ligne[j]!=' '&&ligne[j]!='\0'&&ligne[j]!='\n'){
          strncat(x, &ligne[j], 1);
          j++;
        }
        if (ligne[j]!='\0')j++; // on incrémente encore une fois pour ignorer les espaces

        while(ligne[j]!=' '&&ligne[j]!='\0'&&ligne[j]!='\n'){
          strncat(y, &ligne[j], 1);
          j++;
        }
        if (ligne[j]!='\0')j++;

        if (premier_point==0){
          *point=creer_point(atof(x), atof(y));
          premier_point=1;
        } else inserer_point_en_tete(point, atof(x), atof(y));

        lire_nb_pts++;
        if (lire_nb_pts==nb_pts) { // on peut couper la boucle lorsqu'on a lu le nombre de points qu'on voulait
          inserer_chaine_en_tete(chaine, atoi(&ligne[0]), *point);
          break;
        }
        if (ligne[j]=='\0') break; // ou lorsque la fin de la ligne est atteinte mais cette seule condition n'est pas efficace

      }
      char numero[10];
      strncat(numero, &ligne[0], 10);
    }
    if(feof(f)) break;
  }
  liste_chaines->chaines=*chaine;
  return liste_chaines;
}

int main(int argc, char const *argv[]) {
  FILE* fic ;
  fic=fopen(argv[1],"r");
  if (!fic){
    printf("Problème lors de la lecture du fichier\n");
    return 1;
  }
  printf("fichier ouvert avec succès\n\n");
  Chaines* ch1 = lectureChaines(fic);
  afficher_liste_chaines(ch1);
  fclose(fic);
  liberer_liste_chaines(ch1);

  /*CellPoint** pointeur_liste_points;
  pointeur_liste_points=(CellPoint**)malloc(sizeof(CellPoint*));
  CellPoint* liste_points=creer_point(9.88,71.19);
  *pointeur_liste_points=liste_points;
  inserer_point_en_tete(pointeur_liste_points, 45.2, 2.125);
  inserer_point_en_tete(pointeur_liste_points, 456.2, 2.4852);
  afficher_points(*pointeur_liste_points);


  CellPoint** pointeur_liste_points_2;
  pointeur_liste_points_2=(CellPoint**)malloc(sizeof(CellPoint*));
  CellPoint* liste_points_2=creer_point(3.33,25.8);
  *pointeur_liste_points_2=liste_points_2;
  inserer_point_en_tete(pointeur_liste_points_2, 52.8, 2.0);
  inserer_point_en_tete(pointeur_liste_points_2, 17.2, 12.9);
  afficher_points(*pointeur_liste_points_2);

  CellChaine** pointeur_liste_chaine;
  pointeur_liste_chaine=(CellChaine**)malloc(sizeof(CellChaine*));
  CellChaine* liste_chaine=creer_chaine(15, *pointeur_liste_points_2);
  *pointeur_liste_chaine=liste_chaine;
  afficher_chaine(*pointeur_liste_chaine);
  inserer_chaine_en_tete(pointeur_liste_chaine, 10, *pointeur_liste_points);
  afficher_chaine(*pointeur_liste_chaine);

  Chaines* chaine

  liberer_chaine(*pointeur_liste_chaine);
  free(pointeur_liste_chaine);
  //liberer_points(*pointeur_liste_points);
  free(pointeur_liste_points);
  //liberer_points(*pointeur_liste_points_2);
  free(pointeur_liste_points_2);*/

  return 0;
}
