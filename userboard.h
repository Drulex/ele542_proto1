/*************************************************************************/
/* Fichier:			 userboard.h										 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#ifndef USERBOARD_H
#define USERBOARD_H


/* fichier includes */
//#include "main.h"
#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/wdt.h>

/* macros et constantes*/
#define ROTATION (Pi / 90)


/* PORTD defines */
#define DIR_G1 PORTD2
#define DIR_G2 PORTD3
#define DIR_D1 PORTD6
#define DIR_D2 PORTD7
#define PWM_G  PORTD4
#define PWM_D  PORTD5

/* PORTA defines */
#define DIR_G PORTA2
#define DIR_D PORTA3
#define CAL   PORTA4
#define SW6  (PINA & (1 << PA6))
#define SW7  (PINA & (1 << PA7))

/* types redefinis */
typedef unsigned char u08;
typedef char s08;
typedef unsigned short u16;
typedef	short s16;

#endif
