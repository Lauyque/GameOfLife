#include <stdio.h>
#include <windows.h>
#include <commdlg.h>
//#include <unistd.h> // Pour access()
//#include <errno.h>  // Pour errno

// Mes propes fichiers
#include "grille.h"

void sauvegarderGrilleChaine(GrilleChaine *grilleChaine, const char *nomFichier) {
    if (grilleChaine == NULL || grilleChaine->dernier == NULL) {
        fprintf(stderr, "Erreur : GrilleChaine vide.\n");
        return;
    }

    // Créer le chemin complet vers le dossier "save"
    char cheminComplet[MAX_PATH];
    snprintf(cheminComplet, sizeof(cheminComplet), "save/%s", nomFichier);

    FILE *fichier = fopen(cheminComplet, "w");
    if (!fichier) {
        perror("Erreur ouverture fichier");
        return;
    }

    fprintf(fichier, "%lu\n", grilleChaine->taille);

    Grille *actuelle = grilleChaine->premier;
    while (actuelle != NULL) {
        fprintf(fichier, "%d %d\n", actuelle->tailleX, actuelle->tailleY);
        for (int i = 0; i < actuelle->tailleY; i++) {
            for (int j = 0; j < actuelle->tailleX; j++) {
                fprintf(fichier, "%d ", actuelle->listePointeursLignes[i][j]);
            }
            fprintf(fichier, "\n");
        }
        actuelle = actuelle->suivant;
    }

    fclose(fichier);
    printf("Sauvegarde réussie dans %s\n", cheminComplet);
}

GrilleChaine* chargerGrilleChaine(const char *nomFichier) {
    // Vérification du chemin
    // if (access(nomFichier, F_OK) == -1) {
    //     // Le fichier n'existe pas
    //     perror("Le fichier n'existe pas");
    //     return NULL;
    // }

    FILE *fichier = fopen(nomFichier, "r");
    if (!fichier) {
        perror("Erreur ouverture fichier");
        return NULL;
    }

    size_t taille;
    fscanf(fichier, "%lu", &taille);
    if (taille == 0){
        perror("Pas de taille dans le fichier");
        return NULL;
    }
    //printf("Nombre de tours : %lu\n", taille);

    GrilleChaine *grilleChaine = (GrilleChaine *)malloc(sizeof(GrilleChaine));
    grilleChaine->taille = 0;
    grilleChaine->premier = grilleChaine->dernier = NULL;

    for (size_t t = 0; t < taille; t++) {
        int largeur, hauteur;
        fscanf(fichier, "%d %d", &largeur, &hauteur);
        //printf("Hauteur = %d Largeur = %d\n", hauteur, largeur);

        Grille *nouvelleGrille = (Grille *)malloc(sizeof(Grille));
        nouvelleGrille->tailleX = largeur;
        nouvelleGrille->tailleY = hauteur;

        nouvelleGrille->listePointeursLignes = (int **)malloc(hauteur * sizeof(int *));
        if (nouvelleGrille->listePointeursLignes == NULL) {
            perror("Erreur allocation mémoire");
            fclose(fichier);
            return NULL;
        }
        for (int i = 0; i < hauteur; i++) {
            nouvelleGrille->listePointeursLignes[i] = (int *)malloc(largeur * sizeof(int));
            if (nouvelleGrille->listePointeursLignes[i] == NULL) {
                perror("Erreur allocation mémoire");
                fclose(fichier);
                return NULL;
            }
            for (int j = 0; j < largeur; j++) {
                fscanf(fichier, "%d", &nouvelleGrille->listePointeursLignes[i][j]);
            }
        }

        nouvelleGrille->precedent = grilleChaine->dernier;
        nouvelleGrille->suivant = NULL;
        if (grilleChaine->dernier) {
            grilleChaine->dernier->suivant = nouvelleGrille;
        } else {
            grilleChaine->premier = nouvelleGrille;
        }
        grilleChaine->dernier = nouvelleGrille;
        grilleChaine->taille++;
    }

    fclose(fichier);
    printf("Chargement réussi depuis %s\n", nomFichier);
    return grilleChaine;
}

// Fonction pour ouvrir un explorateur de fichiers et récupérer le chemin du fichier sélectionné
char* ouvrirExplorateurFichiers() {
    OPENFILENAME ofn;
    char filePath[MAX_PATH] = "";

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filePath;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(filePath);
    ofn.lpstrFilter = "All Files\0*.*\0Text Files\0*.TXT\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileName(&ofn) == TRUE) {
        return strdup(filePath);
    } else {
        return NULL;
    }
}