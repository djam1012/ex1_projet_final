#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Reseau.h"
#include "ArbreQuat.h"
#include "Chaine.h"

void chaineCoordMinMax(Chaines* C, double* xmin, double* ymin, double* xmax, double* ymax){
  CellChaine* chaine_cour=C->chaines;
  *xmin=(C->chaines->points->x);
  *ymin=(C->chaines->points->y);
  *xmax=0;
  *ymax=0;

  while (chaine_cour){
    CellPoint* point_cour=chaine_cour->points;
    while(point_cour){
      // coordonnées min
      if (point_cour->x<=(*xmin))
        *xmin=(point_cour->x);
      if (point_cour->y<=(*ymin))
        *ymin=(point_cour->y);

      // coordonnées max
      if (point_cour->x>=(*xmax))
        *xmax=(point_cour->x);
      if (point_cour->y>=(*ymax))
        *ymax=(point_cour->y);

      point_cour=point_cour->suiv;
    }
    chaine_cour=chaine_cour->suiv;
  }
}

ArbreQuat* creerArbreQuat(double xc, double yc, double coteX, double coteY){
  ArbreQuat* aq=(ArbreQuat*)malloc(sizeof(ArbreQuat));
  aq->xc=xc;
  aq->yc=yc;
  aq->coteX=coteX;
  aq->coteY=coteY;
  aq->noeud= NULL;
  aq->so=NULL;
  aq->se=NULL;
  aq->no=NULL;
  aq->ne=NULL;
  return aq;
}

void liberer_aq(ArbreQuat* aq){
  if (aq){
    liberer_aq(aq->so);
    liberer_aq(aq->se);
    liberer_aq(aq->no);
    liberer_aq(aq->ne);
    free(aq);
  }
}

void insererNoeudArbre(Noeud* n, ArbreQuat** a, ArbreQuat* parent){
  if(!(*a)){
    *a = creerArbreQuat(0, 0, parent->coteX/2, parent->coteY/2);
    (*a)->noeud = n; // on y place le noeud à ajouter
    double x = n->x;
    double y = n->y;
    double xc = parent->xc;
    double yc = parent->yc;
    double CX = (*a)->coteX;
    double CY = (*a)->coteY;

    if( x < xc && y < yc ){
        (*a)->xc = xc - ( CX / 2 );
        (*a)->yc = yc - ( CY / 2 );
    }

    else if ( x >= xc && y < yc ){
        (*a)->xc = xc + ( CX / 2 ) ;
        (*a)->yc = yc - ( CY / 2 );
    }

    else if( x < xc && y >= yc ){
        (*a)->xc = xc - ( CX / 2 );
        (*a)->yc = yc + ( CY / 2 );
    }

    else if( x >= xc && y >= yc ){
        (*a)->xc = xc + ( CX / 2 );
        (*a)->yc = yc + ( CY / 2 );
    }
  }

  else if ((*a)->noeud){ // Cas d'une feuille donc noeud présent
    if ((*a)->noeud->x==n->x&&(*a)->noeud->y==n->y) return;
    Noeud *ntmp = (*a)->noeud; // On stocke ce neoud
    (*a)->noeud = NULL; // On transforme la feuille en noeud interne "sans noeud"
    insererNoeudArbre(ntmp, a, parent); // on insere le neoud précédemment stocké un niveau en dessous dans l'arbre
    insererNoeudArbre(n, a, parent); // on inserer le noeud à inserer initialement au même niveau de l'arbre que le noeud présent précédemment en feuille
  }

  else if (!(*a)->noeud){
    double x = n->x;
    double y = n->y;
    double xc = (*a)->xc;
    double yc = (*a)->yc;

    if( x < xc && y < yc ){ // cas sud ouest insert so
        insererNoeudArbre(n, &((*a)->so), *a);
    }

    else if ( x >= xc && y < yc ){ // cas  sud est insert se
        insererNoeudArbre(n, &((*a)->se), *a);
    }

    else if ( x < xc && y >= yc ){ // cas nord ouest insert no
        insererNoeudArbre(n, &((*a)->no), *a);
    }

    else if( x >= xc && y >= yc ){ // cas nord est insert ne
        insererNoeudArbre(n, &((*a)->ne), *a);
    }
  }
}

void afficher_aq(ArbreQuat *a){
  if(!a) return;
  if(a->noeud) afficher_noeud(a->noeud);
  else printf("est VIDE\n");

  printf("_______|SUD OUEST\n\t");
  afficher_aq(a->so);
  printf("_______|SUD EST\n\t");
  afficher_aq(a->se);
  printf("_______|NORD OUEST\n\t");
  afficher_aq(a->no);
  printf("_______|NORD EST\n\t");
  afficher_aq(a->ne);
  printf("\n");
}

Noeud* rechercheCreeNoeudArbre(Reseau* R, ArbreQuat** a, ArbreQuat* parent, double x, double y){
  if (!(*a)){
    Noeud* nouv = creer_noeud(R->nbNoeuds+1, x, y);
    insererNoeudArbre(nouv, a, parent);
    ajouter_noeud_en_tete(&(R->noeuds), nouv);
    R->nbNoeuds += 1;

    return nouv;
  }
  else if ((*a)->noeud){
    if ((*a)->noeud->x == x && (*a)->noeud->y == y){
      return (*a)->noeud;
    }
    Noeud* nouv = creer_noeud(R->nbNoeuds+1, x, y);
    insererNoeudArbre(nouv, a, parent);
    ajouter_noeud_en_tete(&(R->noeuds), nouv);
    R->nbNoeuds += 1;
    return nouv;
  }
  else if (*a && !(*a)->noeud){
    // sud-ouest
    if (x < (*a)->xc && y < (*a)->yc)
      rechercheCreeNoeudArbre(R, &((*a)->so), parent, x, y);
    // sud-est
    if (x >= (*a)->xc && y < (*a)->yc)
      rechercheCreeNoeudArbre(R, &((*a)->se), parent, x, y);
    // nord-ouest
    if (x < (*a)->xc && y >= (*a)->yc)
      rechercheCreeNoeudArbre(R, &((*a)->no), parent, x, y);
    // nord-est
    if (x >= (*a)->xc && y >= (*a)->yc)
      rechercheCreeNoeudArbre(R, &((*a)->ne), parent, x, y);
  }
}

Reseau* reconstitueReseauArbre(Chaines* C){
  CellNoeud* noeuds=(CellNoeud*)calloc(1,sizeof(CellNoeud));
  Reseau* res=creer_reseau(0, C->gamma, noeuds, NULL);
  CellChaine* chaine_cour=C->chaines;
  double xmin, ymin, xmax, ymax;
  chaineCoordMinMax(C, &xmin, &ymin, &xmax, &ymax);
  ArbreQuat *parent = creerArbreQuat(xmin*2, ymin*2, xmax*2, ymax*2);

  while(chaine_cour){
    int nb_points=compter_points_chaine(chaine_cour);
    CellPoint* point_cour=chaine_cour->points;
    CellNoeud* noeud=creer_CellNoeud(-1, -1, -1);
    CellNoeud* suiv=creer_CellNoeud(-1, -1, -1);
    CellNoeud* premier_noeud; // premier noeud de la chaine
    CellNoeud* dernier_noeud; // dernier noeud de la chaine

    int i=1;

    while (point_cour){
      noeud->nd=rechercheCreeNoeudArbre(res, &parent, parent, point_cour->x, point_cour->y);
      if (point_cour->suiv) suiv->nd=rechercheCreeNoeudArbre(res, &parent, parent, point_cour->suiv->x, point_cour->suiv->y);
      noeud->suiv=suiv;

      if (i==1) premier_noeud=noeud;
      if (i==nb_points) dernier_noeud=noeud;

      // ajout du précédent et du suivant de chaque noeud pour les voisins
      if (suiv && noeud && suiv->nd->num!=noeud->nd->num){
        if(!recherche_noeud_liste(noeud->nd->voisins,suiv->nd))
          lier_voisin(suiv, noeud);
        if(!recherche_noeud_liste(suiv->nd->voisins, noeud->nd))
          lier_voisin(noeud, suiv);
      }
      point_cour=point_cour->suiv;
      i++;
    }
    afficher_noeud(premier_noeud->nd);
    afficher_noeud(dernier_noeud->nd);
    inserer_com_en_tete(&(res->commodites), premier_noeud->nd, dernier_noeud->nd);
    chaine_cour=chaine_cour->suiv;
    free(noeud);
    free(suiv);
  }
  //afficher_aq(parent);
  afficher_reseau(res);
  afficher_voisins_reseau(res);

  printf("\n");
  printf("\033[1;33m");
  printf("NOMBRE DE COMMODITES DU RESEAU : %d\n", nbCommodites(res));
  printf("NOMBRE DE LIAISONS DU RESEAU : %d\n", nbLiaisons(res));
  printf("\033[1;39m");

  FILE* f=fopen("reseau_affiche_arbre.res", "w");
  if (!f){
    printf("Problème lors de la lecture du fichier\n");
    fclose(f);
    return NULL;
  }
  ecrireReseau(res, f);
  afficheReseauSVG(res, "reseau_affiche_arbre");
  liberer_aq(parent);
  return res;
}
