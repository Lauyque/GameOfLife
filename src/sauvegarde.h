#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

//#include "grille.h"

void sauvegarderGrilleChaine(GrilleChaine *grilleChaine, const char *nomFichier);
GrilleChaine* chargerGrilleChaine(const char *nomFichier);
char* ouvrirExplorateurFichiers();

#endif // SAUVEGARDE_H
