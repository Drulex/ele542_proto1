/*************************************************************************/
/* Fichier:			 led.c												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#include "userboard.h"
#include "led.h"



void led_init(void)
{
	// toutes les pins du PORTB sont utilisees en mode output
	DDRB = 0xff;
	PORTB = 0xff;
}


void led_on(u08 led)
{
	PORTB &= ~_BV(led);
}


void led_off(u08 led)
{
	PORTB |= _BV(led);
}


void led_toggle(u08 led)
{
	PORTB ^= _BV(led);
}