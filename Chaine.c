#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Chaine.h"
#include "SVGwriter.h"
#include <math.h>

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
  printf("Gamma: %d, Nombre de chaines: %d, Liste des chaines:\n", liste_chaines->gamma, liste_chaines->nbChaines);
  afficher_chaine(liste_chaines->chaines);
}

void liberer_liste_chaines(Chaines* liste_chaines){
  if (!liste_chaines){
    printf("Les chaines sont vides.\n");
    return;
  }
  liberer_chaine(liste_chaines->chaines);
  free(liste_chaines);
}

int compter_points_chaine(CellChaine* liste_chaine){
  int nb_points=0;
  if (!liste_chaine){
    printf("La liste de chaines est vide.\n");
    return 0;
  }
  CellPoint* points_cour=liste_chaine->points;
  while(points_cour){
    nb_points++;
    if(liste_chaine->points->suiv==NULL) return 0;
    points_cour=points_cour->suiv;
  }
  return nb_points;
}

Chaines* lectureChaines(FILE *f){
  // on alloue les structures nécessaires
  Chaines* liste_chaines=(Chaines*)malloc(sizeof(Chaines));
  CellChaine** chaine=(CellChaine**)malloc(sizeof(CellChaine*));

  // on crée les variables temporaires permettant de lire chaque donnée du fichier
  char x[10]; // abcisse
  char y[10]; // ordonnée
  char numero[10]; // numero de la chaine
  char gamma[10]; // gamma
  char nbChaines[10]; // nombre de chaines
  char ligne[256]; // ligne à lire dans le fichier
  int num_ligne=0; // numero de la ligne
  int premier_point=0; // teste si le point à insérer est le premier, alors il crée un nouveau point
  int premiere_chaine=0; // teste si la chaine à insérer est la première

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
      CellPoint** point=(CellPoint**)calloc(1,sizeof(CellPoint*));

      while(ligne){ // lecture de 1 ligne

        if (feof(f)) break; // condition pour sortir de la boucle car ça ne marche pas lorsqu'on le met pas (boucle infinie)
        for (int c=0; c<10; c++){// on les réinitialise quand on passe au point suivant
            x[c]='\0';
            y[c]='\0';
        }


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
          for (int c=0; c<10; c++){// on réinitialise quand on passe au numero suivant
              numero[c]='\0';
          }
          strncat(numero, &ligne[0], 10);
          if (premiere_chaine==0){
            *chaine=creer_chaine(atoi(numero), *point);
            premiere_chaine=1;
          } else inserer_chaine_en_tete(chaine, atoi(numero), *point);
          break;
        }
        if (ligne[j]=='\0') break; // ou lorsque la fin de la ligne est atteinte
      }
      free(point);
    }
    if(feof(f)) break;
  }
  liste_chaines->chaines=*chaine;
  free(chaine);
  return liste_chaines;
}

void ecrireChaines(Chaines *C, FILE *f){

  // on vérifie que la structure ou les chaines qu'elle contient ne sont pas vides
  if (!C&&!C->chaines){
    printf("La chaine est vide, on ne peut pas la parcourir.\n");
    return;
  }
  // on écrit le gamma et le nombre de chaines
  fprintf(f,"NbChain: %d\nGamma: %d\n\n",C->nbChaines,C->gamma);

  // on écrit le numero des chaines, leur nombre de points puis le leurs points
  CellChaine* chaine_cour=C->chaines;
  while(chaine_cour){
    fprintf(f, "%d %d", chaine_cour->numero, compter_points_chaine(chaine_cour));
    CellPoint* point_cour=chaine_cour->points;
    while(point_cour){
      fprintf(f, " %.2f %.2f", point_cour->x, point_cour->y); // on laisse un espace en plus au debut pour ajouter le nombre de points
      point_cour=point_cour->suiv;
    }
    fprintf(f, "\n");
    if(C->chaines->suiv==NULL){
      return;
    }
    chaine_cour=chaine_cour->suiv;
  }

}

void afficheChainesSVG(Chaines *C, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellChaine *ccour;
    CellPoint *pcour;
    double precx,precy;
    SVGwriter svg;
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        while (pcour!=NULL){
            if (maxx<pcour->x) maxx=pcour->x;
            if (maxy<pcour->y) maxy=pcour->y;
            if (minx>pcour->x) minx=pcour->x;
            if (miny>pcour->y) miny=pcour->y;
            pcour=pcour->suiv;
        }
    ccour=ccour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ccour=C->chaines;
    while (ccour!=NULL){
        pcour=ccour->points;
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
        precx=pcour->x;
        precy=pcour->y;
        pcour=pcour->suiv;
        while (pcour!=NULL){
            SVGline(&svg,500*(precx-minx)/(maxx-minx),500*(precy-miny)/(maxy-miny),500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            SVGpoint(&svg,500*(pcour->x-minx)/(maxx-minx),500*(pcour->y-miny)/(maxy-miny));
            precx=pcour->x;
            precy=pcour->y;
            pcour=pcour->suiv;
        }
        ccour=ccour->suiv;
    }
    SVGfinalize(&svg);
}

double longueurChaine (CellChaine * c ){
  CellPoint * p = c->points->suiv;
  CellPoint* prec= c->points;
  double l=0;
  while(p->suiv != NULL){
    l=l+ sqrt(((p->x)-(prec->x))*((p->x)-(prec->x))+ ((p->y)-(prec->y))*((p->y)-(prec->y)));
    prec=p;
    p=p->suiv;
  }
  l=l+ sqrt(((p->x)-(c->points->x))*((p->x)-(c->points->x))+ ((p->y)-(c->points->y))*((p->y)-(c->points->y)));
  return l;
}

double longueurTotale(Chaines *C){
  int i=0;
  double S = 0;

  CellChaine * p = C->chaines ;

  for(i=0;i<C->nbChaines;i++){
    S=S+longueurChaine(p);
    p=p->suiv;
  }

  return S;
}

int comptePointsTotal(Chaines * C){
  int i=0;
  int S=0;
  CellChaine * p = C->chaines ;
  for(i=0;i<C->nbChaines;i++){
    S=S+compter_points_chaine(p);
    p=p->suiv;

  }

  return S;
}
