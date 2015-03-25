/*************************************************************************/
/* Fichier:			 main.h												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include "userboard.h"

/* variables vitesse moteur */
float vitesse_gauche = 0.0;
float vitesse_droite = 0.0;

/* variables de la commande recue */
float vitesse_d;
float angle_d;

/* variable d'etat du robot */
u08 etat_robot = 0;

/* variables de calibration
 * [v]itesse [m]ax|[z]ero [n]egatif|[p]ositif [d]roite|[g]auche
 */
float vmpg = 0.0;
float vmng = 0.0;
float vzpg = 0.0;
float vzng = 0.0;
float vmpd = 0.0;
float vmnd = 0.0;
float vzpd = 0.0;
float vznd = 0.0;

/*************************************************************************
 * Desc: transforme la valeur de la commande de degres a radians
 * Param: angle en degre
 * Retour: angle en radians
 ************************************************************************/
float degree_to_rad(float angle);

#endif
