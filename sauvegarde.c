// On a besoin de 2 fonction :
//     - La première permet d"importer un fichier de sauvegarde (utilisé dans le fichier menu pour le menu)
//         - Dans ce fichier nous avons besoin de :
//             - Sauvegarder toute les datas des 2 structure : GrilleChaine et Grille (à voir leur détailles dans le fichier "grille.h")
//             Grille chaine est unique. Par contre Grille est multiple et son nombre est dans grilleChaine. Donc tu peut retrouver le nombre de grille à sauvegarder directement via "grille->taille"
//             - C'est tout, les 2 structures ont toues les données utiles
//     - La deuxième fonction permet de sauvegarder à n'importe quel moment les jeu en cours (utilisé dans le fichier jeu)
//         - Doit suavegarde les 2 structures