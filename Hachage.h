#ifndef __HACHAGE_H__
#define __HACHAGE_H__
#include "Chaine.h"
#include "Reseau.h"
#include "math.h"

/*  Structure table de hachage  */
typedef struct tableHachage{
  int nbe;    // nombre d'élements dans la table de hachage
  int m;      // taille de la table de hachage
  CellNoeud** tab;          /* tableau de pointeurs vers une liste de noeuds */
} TableHachage;




int fonctionHachage(int clef, int m);
int fonctionClef(int x,int y);
TableHachage* creer_table_h(int m);
Noeud * creer_noeud_h(int num, double x, double y);
void inserer_noeud_en_tete_h(CellNoeud** liste_noeud, int num, double x, double y);
void inserer_noeud_h(TableHachage* t,int num,double x,double y);
Noeud* rechercheCreeNoeudHachage(Reseau* R, TableHachage* H, double x, double y) ;
Reseau* reconstitueReseauHachage(Chaines *C, int m);
void liberer_liste_noeuds_h(CellNoeud* noeuds);
void liberer_table_hachage(TableHachage* th);
int recherche_noeud_h(TableHachage* H, Noeud* n);
void liberer_noeud_h(Noeud* n);
void afficher_table(TableHachage* H);

#endif
