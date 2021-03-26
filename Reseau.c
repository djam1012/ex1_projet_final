#include "Reseau.h"
#include "Chaine.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Noeud * creer_noeud(int num, double x, double y){
  Noeud * n = (Noeud *) malloc(sizeof(Noeud));
  n->num=num;
  n->x=x;
  n->y=y;
  n->voisins=NULL;
  return n;
}

void inserer_noeud_en_tete(Reseau** R, Noeud* n){
  CellNoeud * nouv=R->noeuds->nd;
  if (R->noeuds->nd==NULL){
    R->noeuds->nd=nouv;
  } else {
    nouv->suiv=R->noeuds->nd;
    R->noeuds->nd=nouv;
  }
}

CellCommodite creer_commodite(Noeud* extrA, Noeud* extrB){
  CellCommodite* com = (CellCommodite *) malloc(sizeof(CellCommodite));
  com->extrA=extrA;
  com->extrB=extrB;
  com->suiv=NULL;
}

void inserer_com_en_tete(Reseau** R, CellCommodite* com){
  CellCommodite* nouv=R->commodites;
  if (R->commodites==NULL){
    R->commodites=nouv;
  } else {
    nouv->suiv=R->commodites;
    R->commodites=nouv;
  }
}

Reseau * creer_reseau(int nbNoeuds, int gamma, CellNoeud* noeuds, CellCommodite* commodites){
  Reseau * r= (Reseau *) malloc(sizeof(Reseau));
  r->gamma=gamma;
  r->nbNoeuds=nbNoeuds;
  r->noeuds=noeuds;
  r->commodites=commodites;
  return r;
}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
  CellNoeud* cell_noeud = R->noeuds;
  Noeud* nouv;
  CellCommodite com_nouv;
  int found=0;
  while (cell_noeud->suiv != NULL){
    if (cell_noeud->nd->x == x && cell_noeud->nd->y == y){
      found++;
      return cell_noeud;
    }
    cell_noeud=cell_noeud->suiv;
  }
  nouv=creer_noeud(R->nbNoeuds+1,x,y);
  R->nbNoeuds++;
  inserer_noeud_en_tete(*R,nouv);
  return nouv;
}

Reseau* reconstitueReseauListe(Chaines *C){
  Reseau res=creer_reseau(0, C->gamma, NULL, NULL);
  CellChaine chaine_cour=C->chaines;
  while(chaine_cour){
    CellPoint point_cour=chaine_cour->points;
    while (point_cour){
      rechercheCreeNoeudListe(res, point_cour->x, point_cour->y);
    }
  }
  return res;
}

void afficher_noeud(Noeud * n){
  printf("Noeud numéro %d: (%lf, %lf)\n", n->num, n->x, n->y);
}

void afficher_commodite(CellCommmodite * n){
  printf("Commodité de noeuds:\n");
  printf("\t");
  afficher_noeud(n->extrA);
  printf("\n");
  printf("\t");
  afficher_noeud(n->extrB);
}

void afficher_reseau(Reseau * r){
  printf("Voici le réseau de %d noeuds et de gamma %d:\n", r->nbNoeuds, r->gamma);
  printf("\n");
  printf("La liste des noeuds du réseau est:\n");
  CellNoeud noeud_cour=r->noeuds; // on crée un noeud courant pour ne pas fausser la liste de noeuds
  while (r->noeuds->suiv != NULL){
    afficher_noeud(noeud_cour);
    noeud_cour=noeud_cour->suiv;
  }
  printf("\n");
  printf("La liste des commodités du réseau est:\n");
  CellCommodite com_cour=r->commodites;
  while (com_cour->suiv != NULL){
    afficher_commodite(com_cour);
    com_cour=com_cour->suiv;
  }
}

int main(){

  return 0;
}
