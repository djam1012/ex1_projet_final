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

void inserer_noeud_en_tete(CellNoeud* liste_noeud, int num, double x, double y){
  Noeud* temp=creer_noeud(num, x, y);
  CellNoeud* nouv;
  nouv->nd=temp;
  nouv->suiv=NULL;
  if (liste_noeud==NULL){
    liste_noeud=nouv;
  } else {
    nouv->suiv=liste_noeud;
    liste_noeud=nouv;
  }
}

void ajouter_voisin_en_tete(Noeud* noeud, CellNoeud* voisin){
  CellNoeud* nouv=voisin;
  if (noeud->voisins==NULL){
    noeud->voisins=nouv;
  } else {
    nouv->suiv=noeud->voisins;
    noeud->voisins=nouv;
  }
}

CellCommodite* creer_commodite(Noeud* extrA, Noeud* extrB){
  CellCommodite* com = (CellCommodite *) malloc(sizeof(CellCommodite));
  com->extrA=extrA;
  com->extrB=extrB;
  com->suiv=NULL;
}

void inserer_com_en_tete(CellCommodite* liste_com, Noeud* extrA, Noeud* extrB){
  CellCommodite* nouv=creer_commodite(extrA, extrB);
  if (liste_com==NULL){
    liste_com=nouv;
  } else {
    nouv->suiv=liste_com;
    liste_com=nouv;
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
  while (cell_noeud->suiv != NULL){
    if (cell_noeud->nd->x == x && cell_noeud->nd->y == y){
      afficher_noeud(cell_noeud->nd);
      return cell_noeud->nd;
    }
    cell_noeud=cell_noeud->suiv;
  }
  nouv=creer_noeud(R->nbNoeuds+1,x,y);
  R->nbNoeuds++;
  inserer_noeud_en_tete(cell_noeud,nouv->num,x,y);
  return cell_noeud->nd;
}

Reseau* reconstitueReseauListe(Chaines *C){
  Reseau* res=creer_reseau(0, C->gamma, NULL, NULL);
  CellChaine* chaine_cour=C->chaines;
  while(chaine_cour){
    CellPoint* point_cour=chaine_cour->points;
    CellNoeud* noeud;
    CellNoeud* noeud_prec=NULL;
    CellNoeud* premier_noeud; // premier noeud de la chaine
    CellNoeud* dernier_noeud; // dernier noeud de la chaine
    int i=0;
    while (point_cour){
      int nb_noeuds=res->nbNoeuds;
      if (!res->noeuds->nd) printf("res->noeuds est null\n");
      noeud->nd=rechercheCreeNoeudListe(res, point_cour->x, point_cour->y);
      if (i==0) premier_noeud=noeud;
      if (i==longueurChaine(chaine_cour)) dernier_noeud=noeud;
      if (res->nbNoeuds>nb_noeuds && nb_noeuds!=0 && noeud && noeud_prec)
        ajouter_voisin_en_tete(noeud->nd, noeud_prec);
      noeud_prec=noeud;
      point_cour=point_cour->suiv;
      i++;
    }
    inserer_com_en_tete(res->commodites, premier_noeud->nd, dernier_noeud->nd);
    chaine_cour=chaine_cour->suiv;
  }
  return res;
}

void afficher_noeud(Noeud* n){
  printf("Noeud numéro %d: (%lf, %lf)\n", n->num, n->x, n->y);
}

void afficher_commodite(CellCommodite* n){
  printf("Commodité de noeuds:\n");
  printf("\t");
  afficher_noeud(n->extrA);
  printf("\n");
  printf("\t");
  afficher_noeud(n->extrB);
}

void afficher_reseau(Reseau* r){
  printf("Voici le réseau de %d noeuds et de gamma %d:\n", r->nbNoeuds, r->gamma);
  printf("\n");
  printf("La liste des noeuds du réseau est:\n");
  CellNoeud* noeud_cour=r->noeuds; // on crée un noeud courant pour ne pas fausser la liste de noeuds
  while (r->noeuds->suiv != NULL){
    afficher_noeud(noeud_cour->nd);
    noeud_cour=noeud_cour->suiv;
  }
  printf("\n");
  printf("La liste des commodités du réseau est:\n");
  CellCommodite* com_cour=r->commodites;
  while (com_cour->suiv != NULL){
    afficher_commodite(com_cour);
    com_cour=com_cour->suiv;
  }
}
