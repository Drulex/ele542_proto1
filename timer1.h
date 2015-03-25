/*************************************************************************/
/* Fichier:			 timer1.h											 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#ifndef TIMER1_H
#define TIMER1_H

/* valeur de TOP = 9999 */
#define TOP 0x270F

/* variables drapeaux */
extern volatile u08 FLAG_TIMER1;
extern volatile u08 FLAG_CALIBRATION;
extern volatile u08 FLAG_CAL_READY;

/* variables pour duty_cycle des moteurs */
extern float duty_cycle_gauche;
extern float duty_cycle_droite;

/* variables locales */
u16 moteur;

/*************************************************************************
 * Desc: initialise le peripherique TIMER1
 * Param: aucun
 * Retour: aucun
 ************************************************************************/
void init_timer1(void);

/*************************************************************************
 * Desc: configure les pins de direction
 * Param: variables de duty_cycle pour chaque moteur
 * Retour: aucun
 ************************************************************************/
void update_moteur(float duty_cycle_gauche, float duty_cycle_droite);

#endif