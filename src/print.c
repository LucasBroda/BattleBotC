/**
* @file print.c
 * @brief Affichage des informations du joueur dans le jeu Battle-C.
 *
 * Ce fichier contient la fonction permettant d'afficher les informations du joueur
 * actuel, y compris son ID, sa santé, son armure, son score et sa position.
 *
 */

#include "print.h"  /**< Déclarations des fonctions d'affichage */
#include "stdio.h"

/**
 * @brief Affiche les données du joueur actuel.
 *
 * Cette fonction récupère les informations du joueur connecté et les affiche
 * sur la console. Elle comprend l'ID, la vie, l'armure, le score et la position
 * du joueur.
 *
 * @param connection Pointeur vers la connexion au serveur de jeu.
 */
void print_data_current_player(BC_Connection *connection) {
    /** @brief Structure contenant les informations du joueur */
    BC_PlayerData player = bc_get_player_data(connection);

    printf("ID : %d\n", player.id);
    printf("Vie du joueur : %d\n", player.health);
    printf("Armure du joueur : %d\n", player.armor);
    printf("Score : %d\n", player.score);
    printf("Position x: %.2f\n", player.position.x);
    printf("Position y: %.2f\n", player.position.y);

    fflush(stdout);
}
