/*************************************************************************/
/* Fichier:			 uart.c												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#include "userboard.h"
#include "uart.h"

/* variables globales */
volatile float param_vitesse;
volatile float param_angle;
volatile char debug_string[TX_BUFFER_SIZE];
volatile u08 FLAG_DEBUG;
volatile u08 FLAG_STOP;

void init_uart(void)
{
	// selection du baudrate
	UBRRH = 0x00;
	UBRRL = UART_BAUD_SELECT;

	// activation des interruptions sur RX complete et activation du receveur et transmetteur uart
	UCSRB = (1 << RXCIE) | (0 << TXCIE) | (0 << UDRIE) | (1 << RXEN) | (1 << TXEN) | (0 << UCSZ2) | (0 << RXB8) | (0 << TXB8);

	// mode asynchrone avec 8 bits data, 1 stop bit et pas de parite
	UCSRC = (1 << URSEL) | (0 << UMSEL) | (0 << UPM1) | (0 << UPM0) | (0 << USBS) | (1 << UCSZ1) | (1 << UCSZ0) | (0 << UCPOL);

	// initialisations
	uart_counter = TX_BUFFER_SIZE;
	FLAG_DEBUG = 0;
	FLAG_STOP = 0;
	rx_state = 1;
}

/*
 * Routine d'interruption pour UART TX
 * Le vecteur est active a chaque fois que le bit UDRIE dans UCSRB est a 1
 */
ISR(USART_UDRE_vect)
{
	// debut de trame
	if(uart_counter == TX_BUFFER_SIZE)
	{
		UDR = 0xFE;
		uart_counter = 0;
	}

	// pas debut de trame -> on incremente le pointeur jusqua la fin de trame
	else
	{
		if(debug_string[uart_counter] != '\0')
		{
			UDR = debug_string[uart_counter];
			uart_counter++;
		}

		// fin de trame -> on desactive le vecteur UDRE jusqua la prochaine trame debug
		else
		{
			UDR = 0xFF;
			UCSRB &= ~(1 << UDRIE);
			uart_counter = TX_BUFFER_SIZE;
			FLAG_DEBUG = 0;
		}
	}

}

/*
 * Routine d'interruption pour UART RX
 * Le vecteur est active lorsquon recoit une donnee dans UDR
 */
ISR(USART_RXC_vect)
{
	volatile u08 uart_data = UDR;
	
	// reset watchdog
	wdt_reset();

	// renvoie l'echo si on n'est pas mode DEBUG
	if (FLAG_DEBUG == 0)
	{
		UDR = uart_data;
	}

	// reception des commandes
	switch (rx_state)
	{
		case 1:
			// mode normal
			if(uart_data == 0xF1)
			{
				rx_state = 2;
				break;
			}

			// sinon arret obligatoire
			else
			{
				param_vitesse = 100;
				FLAG_STOP = 1;
				break;
			}

		case 2:
			// lecture commande vitesse
			param_vitesse = (float)uart_data;
			rx_state = 3;
			break;

		case 3:
			// lecture commande d'angle
			param_angle = (float)uart_data;
			rx_state = 1;

			// on clignote LED1 pour commande recue
			PORTB ^= _BV(1);
			break;
	}
}

void toggle_tx(void)
{
	FLAG_DEBUG = 1;
	UCSRB |= (1 << UDRIE);
}
