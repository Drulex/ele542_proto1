/*************************************************************************/
/* Fichier:			 i2c.c												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/

#include "i2c.h"
#include "userboard.h"

u16 i2c_counter = 0;

/*
 * traitement par interruption 10 kHz sans addresse slave prescaler = 1
 */
void init_i2c(void)
{
	// 198
	TWBR = 0xC6;
	TWSR = ~((1 << TWPS1) | (1 << TWPS0));
	TWDR = 0xFF;
	TWCR = (0 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC) | (1 << TWEN) | (0 << TWIE);
}

/******************************************************************************
 * Insérer dans le buffer out
 *****************************************************************************/
void putDataOutBuf(u08 data){

	CircularBufferOutEnd++;
	i2c_counter++;
	CircularBufferOutEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferOut[CircularBufferOutEnd] = data;

}


/******************************************************************************
 * Retirer du buffer out
 *****************************************************************************/
u08 getDataOutBuf(void){

	CircularBufferOutIndex++;
	CircularBufferOutIndex %= CIRCULAR_BUFFER_SIZE;
	i2c_counter--;
	return (u08)CircularBufferOut[CircularBufferOutIndex];

}


/******************************************************************************
 * Insérer dans le buffer in
 *****************************************************************************/
void putDataInBuf(u08 * ptr){

	CircularBufferInEnd++;
	CircularBufferInEnd %= CIRCULAR_BUFFER_SIZE;
	CircularBufferIn[CircularBufferInEnd] = ptr;

}


/******************************************************************************
 * Retirer du buffer in
 *****************************************************************************/
u08 * getDataInBuf(void){

	CircularBufferInIndex++;
	CircularBufferInIndex %= CIRCULAR_BUFFER_SIZE;
	return CircularBufferIn[CircularBufferInIndex];

}


/******************************************************************************
 * Écrire sur le bus twi
 *****************************************************************************/
void twiWrite(u08 address, u08 registre, u08 data){

	cli();
	/*
		Insérer ici le code pour lancer une écriture dur le bus twi
	*/
	sei();

}

/******************************************************************************
 * lire sur le bus
 *****************************************************************************/
void twiRead(u08 address, u08 registre, u08 *ptr){

	cli();
	/*
		Insérer ici le code pour lancer une écriture dur le bus twi
	*/	
	sei();
}


/******************************************************************************
 *
 *****************************************************************************/
ISR(TWI_vect) {

	u08 status  = TWSR & 0xF8;
		
	switch (status) {
		case	0x08: /* Start Condition */
		case	0x10: /* Restart Condition */
			
			/* 
				Si  nous avons un start ou un restart condition alors il faut envoyer l'addr 
				qui est dans le buffer Out et Activer le bus sans start/stop 
			*/
			
			break;

		case	0x18: /* Address Write Ack */
		case	0x28: /* Data Write Ack */
		case	0x30: /* Date Write NoAck */
			
			/* 
				Si  nous avons un data ou une addr d'écrit sur le bus, ensuite il peut y avoir un autre data, 
				un stop ou un restart. Il faut donc lire le buffer pour savoir quoi faire et configure 
				le bus en conséquence 
			*/
			
			break;

		case	0x50: /* Data Read Ack */
		case	0x58: /* Data Read NoAck */

			/* 
				Une lecture à été effectué sur le bus, il faut donc la récupérer 
			*/

		case	0x40: /* Address Read Ack */

			/* 
				Puisqu'il n'y a pas de break dans les deux case 0x50 et 0x58, quand nous sommes ici
				nous avons soit lue la donnée ou envoyé l'addr à lire, il peut donc y avoir un stop, un
				start ou encore il faut placer le bus en mode lecture 
			*/
	
			break;

		case	0x48: /* Address Read NoAck */
		case	0x20: /* Address Write NoAck */

			/* 
				Ici l'un des deux sonars n'a pas répondu, il faut donc tout stoper ou faire un restart
			    pour la prochaine trame qui peut être dans le buffer 
			*/
			
			break;

		default : 
			/*
				Cette partie de code ne devrait pas être utile :)
			*/
			break;
	}
}
