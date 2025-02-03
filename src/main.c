
/**
 * @file main.c
 * @brief Programme principal pour la connexion et l'analyse radar dans le jeu Battle-C.
 *
 * Ce fichier contient la fonction principale qui établit une connexion avec le serveur,
 * récupère les informations du monde et active un radar pour détecter les joueurs à proximité.
 *
 */

#include "battle_c.h" /**< Interface de connexion au serveur */
#include "radar.h"    /**< Gestion du radar */
#include "move.h"     /**< Gestion des déplacements */
#include "print.h"    /**< Fonctions d'affichage */
#include "shoot.h"    /**< Gestion des tirs */
#include "stdio.h"
#include "stdlib.h"
#include <string.h>
#include <math.h>

/**
 * @brief Fonction principale du programme.
 *
 * Cette fonction se connecte au serveur de jeu, récupère les informations du joueur
 * et active le radar pour détecter les autres joueurs dans un rayon donné.
 *
 * @param argc Nombre d'arguments en ligne de commande.
 * @param argv Tableau des arguments en ligne de commande.
 * @return EXIT_SUCCESS (0) en cas de succès, 1 en cas d'échec de connexion.
 */
int main(int argc, char *argv[]) {
    /** @brief Pointeur vers la connexion au serveur */
    BC_Connection *conn = bc_connect("5.135.136.236", 8080);
    if (!conn) {
        printf("Erreur : Impossible de se connecter au serveur\n");
        return 1;
    }

    printf("Connecté au serveur avec succès letsgo !\n");
    fflush(stdout);

    /** @brief Récupération des informations du monde et du joueur */
    bc_get_world_info(conn);
    print_data_current_player(conn);

    /** @brief Coordonnées du joueur */
    float player_x = bc_get_player_data(conn).position.x;
    float player_y = bc_get_player_data(conn).position.y;

    /** @brief Rayon de détection du radar en mètres */
    float detection_radius_meters = 10.0f;

    /** @brief Nombre de joueurs détectés */
    int player_count = 0;

    // Activation du radar
    radar(conn, player_x, player_y, detection_radius_meters, &player_count);

    printf("Fin du radar\n");
    fflush(stdout);

    return EXIT_SUCCESS;
}
