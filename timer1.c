/*************************************************************************/
/* Fichier:			 timer1.c											 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#include "userboard.h"
#include "timer1.h"
#include "uart.h"

/* variables globales */
volatile u08 FLAG_TIMER1;
volatile u16 calibration_counter;
float duty_cycle_gauche;
float duty_cycle_droite;

void init_timer1(void)
{
	DDRD = 0xFF;
	// mode fast pwm avec TOP definit dans ICR1
	TCCR1A = (1 << COM1A1) | (0 << COM1A0) | (1 << COM1B1) | (0 << COM1B0) | (0 << FOC1A) | (0 << FOC1B) | (1 << WGM11) | (0 << WGM10);
	TCCR1B = (0 << ICNC1) | (0 << ICES1) | (0 << 5) | (1 << WGM13) | (1 << WGM12) | ( 0 << CS12) | (1 << CS11) | (0 << CS10);

	// set valeur de top in ICR1 (9999)
	ICR1 = TOP;

	// activation de TIMER1_OVF_vect lorsquon atteint la valeur TOP
	TIMSK = (0 << OCIE2) | (0 << TOIE2) | (0 << TICIE1) | (0 << OCIE1A) | (0 << OCIE1B) | (1 << TOIE1) | (0 << OCIE0) | (0 << TOIE0);

	// initialisation
	FLAG_TIMER1 = 0;
	TCNT1 = 0x00;
	OCR1A = 0;
	OCR1B = 0;
	moteur = 0;
	calibration_counter = 0;
}


/*
 * Routine d'interruption pour TIMER1 OVF
 * Le vecteur est active a toutes les 5ms
 */
ISR(TIMER1_OVF_vect)
{
    // 5ms timer
	FLAG_TIMER1 = 1;

	// mode calibration
	if (FLAG_CAL_READY == 0)
	{
		calibration_counter++;

		// on donne 50ms au moteur pour atteindre le regime permanent
		if(calibration_counter == 10)
		{
			FLAG_CAL_READY = 1;
			calibration_counter = 0;
		}
	}
}

void update_moteur(float duty_cycle_gauche, float duty_cycle_droite)
{
	// arret obligatoire
	if(FLAG_STOP == 1)
	{
		PORTD |= (1 << DIR_D2) | (1 << DIR_D1) | (1 << DIR_G2) | (1 << DIR_G1);
		OCR1A = 0;
		OCR1B = 0;
		FLAG_STOP = 0;
	}

	else
	{
		/******************** moteur gauche *************************/
		// avance
		if(duty_cycle_gauche > 0)
		{
			PORTD |= (1 << DIR_G1);
			PORTD &= ~(1 << DIR_G2);
			OCR1B = (u16)((float) ICR1 * duty_cycle_gauche);
		}

		// recule
		else if(duty_cycle_gauche < 0)
		{
			PORTD |= (1 << DIR_G2);
			PORTD &= ~(1 << DIR_G1);
			OCR1B = (u16)((float) ICR1 * (-1.0) *(duty_cycle_gauche));
		}

		// neutre
		else
		{
			PORTD &= ~(1 << DIR_G2) | ~(1 << DIR_G1);
			OCR1B = 0;
		}

		/******************** moteur droite *************************/
		// avance
		if(duty_cycle_droite > 0.01)
		{
			PORTD |= (1 << DIR_D1);
			PORTD &= ~(1 << DIR_D2);
			OCR1A = (u16)((float) ICR1 * duty_cycle_droite);
		}

		//recule
		else if(duty_cycle_droite < 0.01)
		{
			PORTD |= (1 << DIR_D2);
			PORTD &= ~(1 << DIR_D1);
			OCR1A = (u16)((float) ICR1 * (-1.0) * (duty_cycle_droite));
		}

		// neutre
		else
		{
			PORTD &= ~(1 << DIR_D2) | ~(1 << DIR_D1);
			OCR1A = 0;
		}
	}
}
