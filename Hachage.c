#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "Hachage.h"
#include "Reseau.h"
#include "Chaine.h"
#include "SVGwriter.h"

TableHachage* creer_table_h(int m){
  TableHachage* h=(TableHachage*)malloc(sizeof(TableHachage));
  h->m=m;
  h->nbe=0;
  h->tab=(CellNoeud**)calloc(m,sizeof(CellNoeud*));
  for (int i=0; i<m; i++){
    (h->tab)[i]=NULL;
  }
  return h;
}

void liberer_table_hachage(TableHachage* th){
  if (th==NULL) return;
  if (th->tab==NULL) return;
  for (int i=0; i<th->m; i++){
    if (th->tab[i]!=NULL)
      liberer_liste_noeuds_vides(th->tab[i]);
  }
  free(th->tab);
  free(th);
}

void inserer_noeud_h(TableHachage* t, int i, Noeud* n){
  // on insère le noeud dans la case i de la table de hachage
  CellNoeud* cn = (t->tab)[i];
  Noeud* cour = cn->nd;
  if(!(t->tab)[i]){
    // on insère le noeud à cette case si elle est vide
    (t->tab)[i]->nd=n;
  } else {
     // sinon on l'insère par chainage avec le noeud déjà présent ou celui d'après etc.
    while (cn->suiv) cn=cn->suiv;
    Noeud * nn = n;
    ajouter_noeud_en_tete(&cn,n);
    }
   t->nbe++;
}

int fonctionClef(int x,int y){
 int clef = y+ ((x+y)*(x+y+ 1))/2 ;
  return clef;
}

int fonctionHachage(int clef, int m){
  double a=(sqrt(5)-1)/2;
  double res=floor(m*(clef*a-floor(clef*a)));
  return res;
}

int recherche_noeud_h(TableHachage* H, Noeud* n){
  CellNoeud* liste_noeud= (CellNoeud*)malloc(sizeof(H->tab)*sizeof(CellNoeud));
  for(int i=0;i<H->m;i++){
    liste_noeud= H->tab[i];
    if (liste_noeud==NULL) return 0;
    if (n==NULL) return 0;
    while(liste_noeud){
      if (liste_noeud->nd->num==n->num) return 1;
      liste_noeud=liste_noeud->suiv;
    }
  }
  liberer_liste_noeuds(liste_noeud);
  return 0;
}

Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y) {
  int ind=fonctionHachage(fonctionClef(x,y),H->m);
  CellNoeud* cour=(H->tab)[ind];
  while (cour!=NULL){
    if (cour->nd->x == x && cour->nd->y == y)
      return cour->nd;
    cour=cour->suiv;
  }
  // s'il n'est pas dans la table de hachage
  CellNoeud *cnh=creer_CellNoeud(R->nbNoeuds+1, x, y);

  // ajout du noeud dans le reseau
  ajouter_noeud_en_tete(&(R->noeuds), cnh->nd);
  // ajout du noeud dans la table de hachage
  ajouter_CellNoeud_en_tete(&(H->tab[ind]), cnh);
  /*cnh->suiv=H->tab[ind];
  H->tab[ind]=cnh; // ajout en tete*/

  H->nbe++;
  R->nbNoeuds++;
  return cnh->nd;
}

Reseau* reconstitueReseauHachage(Chaines *C, int m){
  CellNoeud* noeuds=(CellNoeud*)calloc(1,sizeof(CellNoeud));
  Reseau* res=creer_reseau(0, C->gamma, noeuds, NULL);
  CellChaine* chaine_cour=C->chaines;
  TableHachage* h=creer_table_h(m);

  while(chaine_cour){
    int nb_points=compter_points_chaine(chaine_cour);
    CellPoint* point_cour=chaine_cour->points;
    CellNoeud* noeud=creer_CellNoeud(-1, -1, -1);
    CellNoeud* suiv=creer_CellNoeud(-1, -1, -1);
    CellNoeud* premier_noeud=creer_CellNoeud(-1, -1, -1); // premier noeud de la chaine
    CellNoeud* dernier_noeud=creer_CellNoeud(-1, -1, -1); // dernier noeud de la chaine

    int i=1;

    while (point_cour){
      noeud->nd=rechercheCreeNoeudHachage(res, h, point_cour->x, point_cour->y);
      if (point_cour->suiv) suiv->nd=rechercheCreeNoeudHachage(res, h, point_cour->suiv->x, point_cour->suiv->y);
      noeud->suiv=suiv;

      if (i==1) premier_noeud->nd=noeud->nd;
      if (i==nb_points) dernier_noeud->nd=noeud->nd;

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
    inserer_com_en_tete(&(res->commodites), premier_noeud->nd, dernier_noeud->nd);
    chaine_cour=chaine_cour->suiv;
    free(noeud);
    free(suiv);
    free(premier_noeud);
    free(dernier_noeud);
  }
  /*afficher_table(h);
  printf("nb elt table %d\n", h->nbe);
  afficher_reseau(res);
  afficher_voisins_reseau(res);

  printf("\n");
  printf("\033[1;33m");
  printf("NOMBRE DE COMMODITES DU RESEAU : %d\n", nbCommodites(res));
  printf("NOMBRE DE LIAISONS DU RESEAU : %d\n", nbLiaisons(res));
  printf("\033[1;39m");

  FILE* f=fopen("reseau_affiche_table.res", "w");
  if (!f){
    printf("Problème lors de la lecture du fichier\n");
    fclose(f);
    return NULL;
  }
  ecrireReseau(res, f);
  afficheReseauSVG(res, "reseau_affiche_table");*/
  liberer_table_hachage(h);
  return res;
}

void afficher_table(TableHachage* T){
  printf("-----Affichage de la table de hachage-----\n");
  for (int i=0; i<T->m; i++){
    CellNoeud* cour=(T->tab)[i];
    while (cour){
      if (cour) afficher_noeud(cour->nd);
      cour=cour->suiv;
    }
  }
}
