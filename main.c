/*************************************************************************/
/* Fichier:			 main.c												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#include "main.h"
#include "userboard.h"
#include "uart.h"
#include "timer1.h"
#include "moteur.h"
#include "adc.h"
#include "i2c.h"

int main(void)
{
	DDRD = 0xFF;
	DDRA = 0x00;
	PORTA = 0x00;
	DDRB = 0xff;
	PORTB = 0xff;

	sei();

	init_uart();
	init_adc();
	init_timer1();
	init_i2c();
	calibre_moteurs(&vmpg, &vmng, &vzpg, &vzng, &vmpd, &vmnd, &vzpd, &vznd);

	// enable watchdog with 1s limit
	wdt_enable(WDTO_1S);

	// message debug
	sprintf(debug_string, "Robot initialise et pret au combat\n\r");
	toggle_tx();

	while(1)
	{
		switch(etat_robot)
		{
			// robot en attente
			case 0:
				PORTB &= ~_BV(6);
				PORTB |= _BV(7);
				FLAG_STOP = 1;
				PORTD |= (1 << DIR_D2) | (1 << DIR_D1) | (1 << DIR_G2) | (1 << DIR_G1);
				OCR1A = 0;
				OCR1B = 0;

				// mise en marche
				if(!SW7)
				{
					etat_robot = 1;
					PORTB &= ~_BV(7);
					FLAG_STOP = 0;
					break;
				}

			case 1:
				PORTB |= _BV(6);
				// arret d'urgence
				if(!SW6)
				{
					etat_robot = 0;
					break;
				}

				// fonctionnement normal
				else
				{
					
					// toutes les 5ms
					if(FLAG_TIMER1 == 1)
					{

						// faire la moyenne des echantillons
						average_adc_samples(&vitesse_gauche, &vitesse_droite);
						reset_adc_variables();

						// correction moteur
						if(vitesse_gauche > 0.0)
							vitesse_gauche = (float)((vitesse_gauche - vzpg) / (vmpg - vzpg));

						if(vitesse_gauche < 0.0)
							vitesse_gauche = (float)(-((-vitesse_gauche - vzng) / (vmng - vzng)));

						if(vitesse_droite > 0.0)
							vitesse_droite = (float)((vitesse_droite - vzpd) / (vmpd - vzpd));

						if(vitesse_droite < 0.0)
							vitesse_droite = (float)(-((-vitesse_droite - vznd) / (vmnd - vznd)));

						// conversion des commandes de teleguidage
						vitesse_d = (float)((param_vitesse - 100.0) / 100.0);
						angle_d = degree_to_rad(param_angle);

						// calculer les duty cycles
						CalculPWM(vitesse_d, angle_d, vitesse_gauche, vitesse_droite, &duty_cycle_gauche, &duty_cycle_droite);

						// envoyer commandes au robot
						update_moteur(duty_cycle_gauche, duty_cycle_droite);

						FLAG_TIMER1 = 0;
					}
				}
		}

	}
	return 0;
}

float degree_to_rad(float angle)
{
	return (float)(angle * ROTATION);
}
