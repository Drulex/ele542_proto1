/*************************************************************************/
/* Fichier:			 uart.h												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#ifndef UART_H
#define UART_H

/* macros et constantes */
//#define F_CPU ((uint32_t)16000000)
#define BAUD_RATE ((uint32_t)9600)
#define UART_BAUD_SELECT (uint8_t)(F_CPU/(BAUD_RATE << 4) - 1)

/* taille buffer de trames debug */
#define TX_BUFFER_SIZE 256

/* variable contenant les trames debug */
extern volatile char debug_string[TX_BUFFER_SIZE];

/* variables locales */
volatile u16 uart_counter, rx_state;

/* variables drapeaux */
extern volatile u08 FLAG_DEBUG;
extern volatile u08 FLAG_STOP;


/* variables de commande recue */
extern volatile float param_vitesse;
extern volatile float param_angle;

/*************************************************************************
 * Desc: initialise le peripherique UART
 * Param: aucun
 * Retour: aucun
 ************************************************************************/
void init_uart(void);

/*************************************************************************
 * Desc: cause une interruption de type USART_UDRE_vect
 * Param: aucun
 * Retour: aucun
 ************************************************************************/
void toggle_tx(void);

#endif
