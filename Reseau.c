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

void inserer_noeud_en_tete(CellNoeud** liste_noeud, int num, double x, double y){
  CellNoeud* nouv=(CellNoeud*)malloc(sizeof(CellNoeud));
  nouv->nd=creer_noeud(num, x, y);
  if (*liste_noeud==NULL){
    *liste_noeud=nouv;
  } else {
    nouv->suiv=*liste_noeud;
    *liste_noeud=nouv;
  }
}

void ajouter_voisin_en_queue(Noeud* noeud, CellNoeud* voisin){
  if (!noeud) return;
  if (!voisin) return;
  CellNoeud* nouv=voisin;
  CellNoeud* temp=noeud->voisins;
  while(temp){
    if (temp->nd->x == nouv->nd->x && temp->nd->y == nouv->nd->y){
      return;
    }
    temp=temp->suiv;
  }
  nouv->suiv=temp;
  temp=nouv;
}

CellCommodite* creer_commodite(Noeud* extrA, Noeud* extrB){
  CellCommodite* com = (CellCommodite *) malloc(sizeof(CellCommodite));
  com->extrA=extrA;
  com->extrB=extrB;
  com->suiv=NULL;
}

void inserer_com_en_tete(CellCommodite** liste_com, Noeud* extrA, Noeud* extrB){
  CellCommodite* nouv=creer_commodite(extrA, extrB);
  if (*liste_com==NULL){
    *liste_com=nouv;
  } else {
    nouv->suiv=*liste_com;
    *liste_com=nouv;
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

void afficher_noeud(Noeud* n){
  if (!n){
    printf("noeud vide\n");
    return;
  }
  printf("Noeud numéro %d: (%.2f, %.2f)\n", n->num, n->x, n->y);
}

void afficher_liste_noeuds(CellNoeud* cn){
  if (!cn){
    printf("liste noeuds vide\n");
    return;
  }
  CellNoeud* temp=cn;
  while(temp->nd){
    afficher_noeud(temp->nd);
    if(!temp->suiv) return;
    temp=temp->suiv;
  }
}

void afficher_voisins(Noeud* n){
  if (!n){
    printf("noeud vide\n");
    return;
  }
  CellNoeud* temp=n->voisins;
  while(temp){
    afficher_noeud(temp->nd);
    temp=temp->suiv;
  }
}

void afficher_voisins_reseau(Reseau* R){
  CellNoeud* cour=R->noeuds;
  int i=0;
  while(cour){
    afficher_noeud(cour->nd);
    printf("A POUR VOISINS :\n");
    afficher_voisins(cour->nd);
    cour=cour->suiv;
  }
}

void afficher_commodite(CellCommodite* c){
  if (!c){
    printf("commodite vide\n");
    return;
  }
  printf("Commodité de noeuds:\n");
  printf("\t");
  afficher_noeud(c->extrA);
  printf("\t");
  afficher_noeud(c->extrB);
  printf("\n");
}

void afficher_reseau(Reseau* r){
  if (!r){
    printf("reseau vide\n");
    return;
  }
  printf("Voici le réseau de %d noeuds et de gamma %d:\n", r->nbNoeuds, r->gamma);
  printf("\n");
  printf("La liste des noeuds du réseau est:\n");
  CellNoeud* noeud_cour=r->noeuds; // on crée un noeud courant pour ne pas fausser la liste de noeuds
  while (noeud_cour->suiv != NULL){
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
  printf("\n");
}

Noeud* rechercheCreeNoeudListe(Reseau *R, double x, double y){
  CellNoeud* cell_noeud = R->noeuds;
  while (cell_noeud->nd){
    // si le noeud y est déjà
    if (cell_noeud->nd->x == x && cell_noeud->nd->y == y){
      return cell_noeud->nd;
    }
    cell_noeud=cell_noeud->suiv;
  }
  // sinon
  inserer_noeud_en_tete(&(R->noeuds),R->nbNoeuds+1,x,y);
  R->nbNoeuds++;
  return R->noeuds->nd;
}

Reseau* reconstitueReseauListe(Chaines *C){
  CellNoeud* noeuds=(CellNoeud*)calloc(1,sizeof(CellNoeud));
  CellCommodite** commodites=(CellCommodite**)calloc(1,sizeof(CellCommodite*));
  *commodites=(CellCommodite*)calloc(1,sizeof(CellCommodite));
  Reseau* res=creer_reseau(0, C->gamma, noeuds, *commodites);
  CellChaine* chaine_cour=C->chaines;
  while(chaine_cour){
    CellPoint* point_cour=chaine_cour->points;
    CellNoeud* noeud=(CellNoeud*)calloc(1,sizeof(CellNoeud));
    CellNoeud* noeud_prec=(CellNoeud*)calloc(1,sizeof(CellNoeud));
    CellNoeud* premier_noeud=(CellNoeud*)calloc(1,sizeof(CellNoeud)); // premier noeud de la chaine
    CellNoeud* dernier_noeud=(CellNoeud*)calloc(1,sizeof(CellNoeud)); // dernier noeud de la chaine
    int i=1;
    while (point_cour){
      int nb_noeuds=res->nbNoeuds;
      noeud->nd=rechercheCreeNoeudListe(res, point_cour->x, point_cour->y);
      if (i==1) premier_noeud->nd=noeud->nd;
      if (i==compter_points_chaine(chaine_cour)) dernier_noeud->nd=noeud->nd;
      // ajout du précédent et du suivant de chaque noeud pour les voisins
      if (noeud && noeud_prec){
        ajouter_voisin_en_queue(noeud->nd, noeud_prec);
        ajouter_voisin_en_queue(noeud_prec->nd, noeud);
      }
      if (noeud && noeud->suiv){
        ajouter_voisin_en_queue(noeud->nd, noeud->suiv);
        ajouter_voisin_en_queue(noeud->suiv->nd, noeud);
      }
      noeud_prec=noeud;
      point_cour=point_cour->suiv;
      i++;
    }
    inserer_com_en_tete(&(res->commodites), premier_noeud->nd, dernier_noeud->nd);
    chaine_cour=chaine_cour->suiv;
  }
  return res;
}
