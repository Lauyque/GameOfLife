# GameOfLife

## 📕 Traduction en Français
GameOfLife = Jeu de la Vie 

## 🤯 Description
Le Jeu de la Vie est un automate cellulaire imaginé par le mathématicien britannique John Horton Conway en 1970. C'est un jeu à zéro joueur, ce qui signifie que son évolution est déterminée par son état initial, sans nécessiter d'intervention supplémentaire.

## 💻 Développement
Ce projet implémente le Jeu de la Vie en C, avec une grille de jeu représentée par une structure de données personnalisée et une interface graphique avec SDL2.

## 🤲 Structure du Projet
- `main.c` : Point d'entrée du programme, contenant la logique principale du jeu.
- `grille.h` : Contient les définitions et prototypes pour la gestion de la grille.
- `grille.c` : Contient les implémentations des fonctions pour manipuler la grille.
- `caseVie.c` : Contient le principe du jeux avec les différentes règles.
- `menu.c` : Contient le lanceur graphique du jeux et le choix de la taille de la grille.
- `jeu.c` : Contient l'interface graphique pour le jeu.
- `sauvegarde.c` : Contient toutes les fonctions pour sauvegarder la grille et permettre son importation.
- `sauvegarde.h` : Contient les prototypes des fonctions définies dans `sauvegarde.c`.


## Fichiers Importants

### grille.h
```cpp
// Prototype de la grille qui permet de créer la liste avec la taille en YxZ
struct GrilleChaine {
    size_t taille;
    struct Grille *dernier;
    struct Grille *premier;
};

struct Grille {
    struct Grille *precedent;
    struct Grille *suivant;
    int tailleX;
    int tailleY;
    int **listePointeursLignes;
};
```
Ce type personnalisé, appelé double liste chaînée, permet de suivre l'historique de toutes les grilles calculées au cours de la vie du jeu.

## ©️ Compilation et Exécution
Pour compiler et exécuter le programme, utilisez les commandes suivantes :  
(Vous devez posseder un compilateur C et l'utilitaire "make" 😉)  
- `make all` : Permet de compiler le code
- `make run` : Permet de lancer le code compilé
- `make copy_dll` : Permet de compier le dll de SDL2 (librairie graphique) dans le dossier build
- `make clean` : Supprimer les fichiers compilés (.o ,.exe et le dll)
- `make build_and_run` : Cette commande va nettoyer les fichiers générés précédemment, compiler le programme et l'exécuter.

## 👽 Auteurs
LE DOHER Loïc  
ZAMBON Ronan  
POISSONNIER Matteo

## Licence
Ce projet est sous licence MIT. Voir le fichier LICENSE pour plus de détails.