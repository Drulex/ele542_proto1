/*************************************************************************/
/* Fichier:			 adc.c												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#include "userboard.h"
#include "uart.h"
#include "adc.h"

/* variables globales */
volatile u08 FLAG_CALIBRATION;
volatile u08 FLAG_CAL_READY;

void init_adc(void)
{
	// mode freerun avec traitement par interruption
	ADMUX = (0 << REFS1) | (0 << REFS0) | (0 << ADLAR) | (0 << MUX4) | (0 << MUX3) | (0 << MUX2) | (0 << MUX1) | (0 << MUX0);
	ADCSRA = (1 << ADEN) | (0 << ADSC) | (1 << ADATE) | (0 << ADIF) | (1 << ADIE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	SFIOR = (0 << ADTS2) | (0 << ADTS1) | (0 << ADTS0) | (0 << 4) | (0 << ACME) | (0 << PUD) | (0 << PSR2) | (0 << PSR10);
	ADCSRA |= (1 << ADSC);
	ADMUX = (1 << MUX0);

	// initialisation des variabels a zero
	nb_samples_gauche = 0;
	nb_samples_droite = 0;
	somme_gauche = 0;
	somme_droite = 0;

	FLAG_CALIBRATION = 0;
	FLAG_CAL_READY = 0;

	// pin CAL en mode input
	DDRA = (1 << PA4);
}

/*
 * Routine d'interruption pour ADC
 * Le vecteur est active a chaque fois qu'une conversion est completee
 */
ISR(ADC_vect)
{
	adc_sample = ADC;

	// 0 -> canal gauche 1 -> canal droite
	switch(ADMUX)
	{
		case 1:
			// vitesse negative
			if ((PINA & (1 << DIR_G)) > 0)
			{
				somme_gauche -= adc_sample;
				nb_samples_gauche++;
			}

			// vitesse positive
			else
			{
				somme_gauche += adc_sample;
				nb_samples_gauche++;
			}
			break;

		case 0:
			// vitesse negative
			if ((PINA & (1 << DIR_D)) > 0)
			{
				somme_droite -= adc_sample;
				nb_samples_droite++;
			}

			// vitesse positive
			else
			{
				somme_droite += adc_sample;
				nb_samples_droite++;
			}
			break;
	}
	// toggle au prochain canal
	ADMUX ^= (1  << MUX0);
}

void average_adc_samples(float *avg_gauche, float *avg_droite)
{
	// variables temporaires
	float tmp_somme_gauche, tmp_somme_droite;
	u16 tmp_nb_samples_gauche, tmp_nb_samples_droite;

	// desactivation des interruptions
	cli();

	tmp_somme_gauche = somme_gauche;
	tmp_somme_droite = somme_droite;
	tmp_nb_samples_gauche = nb_samples_gauche;
	tmp_nb_samples_droite = nb_samples_droite;

	// reactivation des interruptions
	sei();

	*avg_gauche = ((float)tmp_somme_gauche) / ((float)tmp_nb_samples_gauche);
	*avg_droite = ((float)tmp_somme_droite) / ((float)tmp_nb_samples_droite);
}



void calibre_moteurs(float *vmpg, float *vmng, float *vzpg, float *vzng, float *vmpd, float *vmnd, float *vzpd, float *vznd)
{
	float vmax_pos_gauche;
	float vmax_neg_gauche;
	float vzero_pos_gauche;
	float vzero_neg_gauche;
	float vmax_pos_droite;
	float vmax_neg_droite;
	float vzero_pos_droite;
	float vzero_neg_droite;

	// duty cycle a zero
	OCR1A = 0;
	OCR1B = 0;

	/********************* CALIBRATION Vmax+ ********************************/
	PORTD |= (1 << DIR_G1)| (0 << DIR_G2) | (1 << DIR_D1)| (0 << DIR_D2);
	PORTA |= (1 << CAL); 

	while(FLAG_CAL_READY != 1)
	{
		; // attente 50ms
	}

	FLAG_CAL_READY = 0;
	reset_adc_variables();

	while(nb_samples_gauche < CAL_SAMPLES && nb_samples_droite < CAL_SAMPLES)
	{
		; // accumulation d'echantillons
	}

	// calcule la moyenne des echantillons
	average_adc_samples(&vmax_pos_gauche, &vmax_pos_droite);


	/********************* CALIBRATION Vzero+ ********************************/
	PORTD |= (1 << DIR_G1)  | (1 << DIR_D1);
	PORTD &= ~(1 << DIR_G2) | ~(1 << DIR_D2) | ~(1 << PWM_G) | ~(1 << PWM_D);
	PORTA &= ~(1 << CAL);


	while(FLAG_CAL_READY != 1)
	{
		; // attente 50ms
	}

	FLAG_CAL_READY = 0;
	reset_adc_variables();

	while(nb_samples_gauche < CAL_SAMPLES && nb_samples_droite < CAL_SAMPLES)
	{
		; // accumulation d'echantillons
	}

	// calcule la moyenne des echantillons
	average_adc_samples(&vzero_pos_gauche, &vzero_pos_droite);


	/********************* CALIBRATION Vmax- ********************************/
	PORTD |= (1 << DIR_G2) | (1 << DIR_D2);
	PORTD &= ~(1 << DIR_G1) | ~(1 << DIR_D1) | ~(1 << PWM_G) | ~(1 << PWM_D);
	PORTA |= (1 << CAL);

	while(FLAG_CAL_READY != 1)
	{
		;	// attente 50ms
	}

	FLAG_CAL_READY = 0;
	reset_adc_variables();

	while(nb_samples_gauche < CAL_SAMPLES && nb_samples_droite < CAL_SAMPLES)
	{
		; // accumulation d'echantillons
	}

	// calcule la moyenne des echantillons
	average_adc_samples(&vmax_neg_gauche, &vmax_neg_droite);


	/********************* CALIBRATION Vzero- ********************************/
	PORTD |= (1 << DIR_G2) |  (1 << DIR_D2);
	PORTD &= ~(1 << DIR_G1) | ~(1 << DIR_D1) | ~(1 << PWM_G) | ~(1 << PWM_D);
	PORTA &= ~(1 << CAL);

	while(FLAG_CAL_READY != 1)
	{
		; // attente 50ms
	}

	FLAG_CAL_READY = 0;
	reset_adc_variables();

	while(nb_samples_gauche < CAL_SAMPLES && nb_samples_droite < CAL_SAMPLES)
	{
		; // accumulation d'echantillons
	}

	// calcule la moyenne des echantillons
	average_adc_samples(&vzero_neg_gauche, &vzero_neg_droite);

	// assignation des valeurs mesurees
	*vmpg = vmax_pos_gauche;
	*vmng = vmax_neg_gauche;
	*vzpg = vzero_pos_gauche;
	*vzng = vzero_neg_gauche;
	*vmpd = vmax_pos_droite;
	*vmnd = vmax_neg_droite;
	*vzpd = vzero_pos_droite;
	*vznd = vzero_neg_droite;
}

void reset_adc_variables(void)
{
	cli();
	somme_gauche = 0;
	somme_droite = 0;
	nb_samples_gauche = 0;
	nb_samples_droite = 0;
	sei();
}
