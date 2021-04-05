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

CellNoeud* creer_CellNoeud(Noeud* n){
  CellNoeud *cn = (CellNoeud*)malloc(sizeof(CellNoeud));
  cn->nd = n;
  cn->suiv = NULL;
  return cn;
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
  if (!voisin->nd) return;
  CellNoeud* nouv=voisin;
  CellNoeud* temp=noeud->voisins;
  if (!temp){
    temp=nouv;
    afficher_noeud(temp->nd);
    afficher_noeud(voisin->nd);
    return;
  }
  while(temp){
    if (temp->nd->num == nouv->nd->num){
      return;
    }
    temp=temp->suiv;
  }
  temp->suiv=nouv;
}

void ajouter_voisin_noeud(Noeud* n1, Noeud* n2){
  if(!n2) return;
  if(!n1) n1 = n2;
  CellNoeud *temp = n1->voisins;
  CellNoeud* temp2 = n1->voisins;
  while(temp2){
    if (temp2->nd->num==n2->num) return;
    temp2=temp2->suiv;
  }
  n1->voisins = creer_CellNoeud(n2);
  n1->voisins->suiv = temp;
}

int recherche_noeud_liste(CellNoeud* liste_noeud, Noeud* n){
  if (liste_noeud==NULL) return 0;
  if (n==NULL) return 0;
  while(liste_noeud){
    if (liste_noeud->nd->num==n->num) return 1;
    liste_noeud=liste_noeud->suiv;
  }
  return 0;
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
  printf("LISTE DES VOISINS DU RESEAU\n");
  CellNoeud* cour=R->noeuds;
  while(cour->suiv){
    printf("                VOISINS DE ");
    afficher_noeud(cour->nd);
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
    int nb_points=compter_points_chaine(chaine_cour);
    CellPoint* point_cour=chaine_cour->points;
    CellNoeud* noeud=(CellNoeud*)calloc(1,sizeof(CellNoeud));
    CellNoeud* premier_noeud=(CellNoeud*)calloc(1,sizeof(CellNoeud)); // premier noeud de la chaine
    CellNoeud* dernier_noeud=(CellNoeud*)calloc(1,sizeof(CellNoeud)); // dernier noeud de la chaine
    CellNoeud* suiv=(CellNoeud*)calloc(1,sizeof(CellNoeud));
    int i=1;

    while (point_cour){
      noeud->nd=rechercheCreeNoeudListe(res, point_cour->x, point_cour->y);
      if (point_cour->suiv) suiv->nd=rechercheCreeNoeudListe(res, point_cour->suiv->x, point_cour->suiv->y);
      noeud->suiv=suiv;

      if (i==1) premier_noeud->nd=noeud->nd;
      if (i==nb_points) dernier_noeud->nd=noeud->nd;

      // ajout du précédent et du suivant de chaque noeud pour les voisins
      if (suiv && noeud && suiv->nd->num!=noeud->nd->num){
        if(!recherche_noeud_liste(noeud->nd->voisins,suiv->nd))
          ajouter_voisin_noeud(suiv->nd, noeud->nd);
        if(!recherche_noeud_liste(suiv->nd->voisins, noeud->nd))
          ajouter_voisin_noeud(noeud->nd, suiv->nd);
      }
      point_cour=point_cour->suiv;
      i++;
    }
    inserer_com_en_tete(&(res->commodites), premier_noeud->nd, dernier_noeud->nd);
    chaine_cour=chaine_cour->suiv;
  }
  return res;
}

int nbCommodites(Reseau* R){
  int res=0;
  CellCommodite* temp=R->commodites;
  while (temp->suiv){
    res++;
    temp=temp->suiv;
  }
  return res;
}

int nbLiaisons(Reseau* R){
  if(!R) return 0;
  if(!R->nbNoeuds) return 0;
  int res=0;
  CellNoeud* temp=R->noeuds;
  CellNoeud* deja_vu;
  while(temp->nd){
    inserer_noeud_en_tete(&deja_vu, temp->nd->num, temp->nd->x, temp->nd->y);
    CellNoeud* voisin=temp->nd->voisins;
    while(voisin){
      res++;
      voisin=voisin->suiv;
    }
    temp=temp->suiv;
  }
  return res;
}

void ecrireReseau(Reseau* R, FILE* f){
  f=fopen("Reseau1.res", "w");
  if (!f){
    printf("Problème lors de la lecture du fichier\n");
    fclose(f);
    return;
  }
  fprintf(f, "NbNoeuds: %d\nNbLiaisons: %d\nNbCommodites: %d\nGamma: %d\n\n",
          R->nbNoeuds, nbLiaisons(R), nbCommodites(R), R->gamma);
  int i=0;
  CellNoeud* noeud_temp=R->noeuds;
  while (i<R->nbNoeuds){
    fprintf(f, "v %d %.2f %.2f\n", noeud_temp->nd->num, noeud_temp->nd->x, noeud_temp->nd->y);
    noeud_temp=noeud_temp->suiv;
    i++;
  }
  fprintf(f, "\n");
  CellNoeud* cour=R->noeuds;
  CellNoeud* liste_noeud;
  while(cour->suiv){
    Noeud* noeud_cour=cour->nd;
    inserer_noeud_en_tete(&liste_noeud, noeud_cour->num, noeud_cour->x, noeud_cour->y);
    CellNoeud* voisin=cour->nd->voisins;
    while(voisin){
      if(recherche_noeud_liste(cour->nd->voisins,voisin->nd))
        fprintf(f, "l %d %d\n", noeud_cour->num, voisin->nd->num);
      voisin=voisin->suiv;
    }
    cour=cour->suiv;
  }
  fprintf(f, "\n");
  i=0;
  CellCommodite* com_temp=R->commodites;
  while (i<nbCommodites(R)){
    fprintf(f, "k %d %d\n", com_temp->extrA->num, com_temp->extrB->num);
    com_temp=com_temp->suiv;
    i++;
  }
  fclose(f);
}

void afficheReseauSVG(Reseau* R, char* nomInstance){
    int i;
    double maxx=0,maxy=0,minx=1e6,miny=1e6;
    CellNoeud *ncour=R->noeuds;
    SVGwriter svg;
    while (ncour->suiv){
      if (maxx<ncour->nd->x) maxx=ncour->nd->x;
      if (maxy<ncour->nd->y) maxy=ncour->nd->y;
      if (minx>ncour->nd->x) minx=ncour->nd->x;
      if (miny>ncour->nd->y) miny=ncour->nd->y;
      ncour=ncour->suiv;
    }
    SVGinit(&svg,nomInstance,500,500);
    ncour=R->noeuds;
    while (ncour->suiv){
      SVGpoint(&svg,500*(ncour->nd->x-minx)/(maxx-minx),500*(ncour->nd->y-miny)/(maxy-miny));
      ncour=ncour->suiv;
      CellNoeud* vcour=ncour->nd->voisins;
      while (vcour){
        SVGlineRandColor(&svg);
        SVGpoint(&svg,500*(ncour->nd->x-minx)/(maxx-minx),500*(ncour->nd->y-miny)/(maxy-miny));
        SVGline(&svg,500*(vcour->nd->x-minx)/(maxx-minx),500*(vcour->nd->y-miny)/(maxy-miny),500*(ncour->nd->x-minx)/(maxx-minx),500*(ncour->nd->y-miny)/(maxy-miny));
        vcour=vcour->suiv;
      }
      ncour=ncour->suiv;
    }
    SVGfinalize(&svg);
}
