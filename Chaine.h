#ifndef __CHAINE_H__
#define __CHAINE_H__
#include <stdio.h>
#include "SVGwriter.h"

/* Liste chainee de points */
typedef struct cellPoint{
  double x,y;                   /* Coordonnees du point */
  struct cellPoint *suiv;       /* Cellule suivante dans la liste */
} CellPoint;

/* Celllule d une liste (chainee) de chaines */
typedef struct cellChaine{
  int numero;                   /* Numero de la chaine */
  CellPoint *points;            /* Liste des points de la chaine */
  struct cellChaine *suiv;      /* Cellule suivante dans la liste */
} CellChaine;

/* L'ensemble des chaines */
typedef struct {
  int gamma;                    /* Nombre maximal de fibres par cable */
  int nbChaines;                /* Nombre de chaines */
  CellChaine *chaines;          /* La liste chainee des chaines */
} Chaines;

CellPoint* creer_point(double x, double y);
CellChaine* creer_chaine(int numero, CellPoint* liste_points);
void inserer_point_en_tete(CellPoint** liste_points, double x, double y);
void inserer_chaine_en_tete(CellChaine** liste_chaine, int numero, CellPoint* liste_points);
void liberer_points(CellPoint* points);
void liberer_chaine(CellChaine* chaine);
void liberer_liste_chaines(Chaines* liste_chaines);
void afficher_points(CellPoint* liste_points);
void afficher_chaine(CellChaine* liste_chaine);
void afficher_liste_chaines(Chaines* liste_chaines);
int compter_points_chaine(CellChaine* liste_chaine);

Chaines* lectureChaines(FILE *f);
void ecrireChaines(Chaines *C, FILE *f);
void afficheChainesSVG(Chaines *C, char* nomInstance);
double longueurChaine (CellChaine * c );
double longueurTotale(Chaines *C);
int comptePointsTotal(Chaines *C);


#endif
