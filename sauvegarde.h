#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H

void sauvegarder_grille(int** grille, int largeur, int hauteur, const char* nom_fichier);
void charger_grille(int** grille, int largeur, int hauteur, const char* nom_fichier);

#endif // SAUVEGARDE_H