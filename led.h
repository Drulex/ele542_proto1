/*************************************************************************/
/* Fichier:			 led.h												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#ifndef LED_H
#define LED_H

/*************************************************************************
 * Desc: initialise le module LED
 * Param: aucun
 * Retour: aucun
 ************************************************************************/
void led_init(void);

/*************************************************************************
 * Desc: allume une LED
 * Param: nombre entier correspondant au LED sur le port
 * Retour: aucun
 ************************************************************************/
void led_on(u08 led);

/*************************************************************************
 * Desc: eteint une LED
 * Param: nombre entier correspondant au LED sur le port
 * Retour: aucun
 ************************************************************************/
void led_off(u08 led);

/*************************************************************************
 * Desc: toggle une LED
 * Param: nombre entier correspondant au LED sur le port
 * Retour: aucun
 ************************************************************************/
void led_toggle(u08 led);

#endif
