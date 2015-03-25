/*************************************************************************/
/* Fichier:			 adc.h												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#ifndef ADC_H
#define ADC_H

/* nombre d'echantillons pour la calibration */
#define CAL_SAMPLES 25

/* variables locales */
volatile u16 adc_sample;
volatile s16 somme_gauche;
volatile s16 somme_droite;
volatile u08 nb_samples_gauche;
volatile u08 nb_samples_droite;

/* variables globales */
extern volatile u08 FLAG_CAL_READY;

/*************************************************************************
 * Desc: initialise le peripherique ADC
 * Param: aucun
 * Retour: aucun
 ************************************************************************/
void init_adc(void);

/*************************************************************************
 * Desc: calcule la moyenne des echantillons mesures
 * Param: pointeur vers la moyenne du moteur gauche et droite
 * Retour: aucun
 ************************************************************************/
void average_adc_samples(float *avg_gauche, float *avg_droite);

/*************************************************************************
 * Desc: performe la calibration des moteurs et mesures les valeurs min/max
 * Param: pointeurs vers les varibles de calibrations (voir main.h)
 * Retour: aucun
 ************************************************************************/
void calibre_moteurs(float *vmpg, float *vmng, float *vzpg, float *vzng, float *vmpd, float *vmnd, float *vzpd, float *vznd);

/*************************************************************************
 * Desc: reinitialise a zero les variables du ADC
 * Param: aucun
 * Retour: aucun
 ************************************************************************/
void reset_adc_variables(void);

#endif