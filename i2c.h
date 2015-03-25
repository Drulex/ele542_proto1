//*************************************************************************/
/* Fichier:			 i2c.h												 */
/* Auteurs:			 Alexandru Jora et Theodor Daniil					 */
/* Date de revision: 16 Mars 2015										 */
/*************************************************************************/
#ifndef I2C_H
#define I2C_H

#include "userboard.h"

/* taille du buffer circulaire */
#define CIRCULAR_BUFFER_SIZE 32

/*************************************************************************
 * Desc: initialisation du peripherique i2c
 * Param: aucun
 * Retour: aucun
 ************************************************************************/
void init_i2c(void);

/*************************************************************************
 * Desc: insere dans le buffer out
 * Param: byte a envoyer
 * Retour: aucun
 ************************************************************************/
void putDataOutBuf(u08 data);

/*************************************************************************
 * Desc: recupere du buffer out
 * Param: aucun
 * Retour: byte lu du buffer
 ************************************************************************/
u08 getDataOutBuf(void);

/*************************************************************************
 * Desc: insere dans le buffer in
 * Param: pointeur vers la donnee
 * Retour: aucun
 ************************************************************************/
void putDataInBuf(u08 * ptr);

/*************************************************************************
 * Desc: retire du buffer in
 * Param: aucun
 * Retour: pointer vers donnees lues
 ************************************************************************/
u08 * getDataInBuf(void);

/*************************************************************************
 * Desc: ecrit sur le bus twi
 * Param: addresse, registre et donnee
 * Retour: aucun
 ************************************************************************/
void twiWrite(u08 address, u08 registre, u08 data);

/*************************************************************************
 * Desc: lit sur le bus twi
 * Param: addresse, registre et pointeur vers donnees
 * Retour: aucun
 ************************************************************************/
void twiRead(u08 address, u08 registre, u08 *ptr);

/* variables locales */
u08 CircularBufferOut[CIRCULAR_BUFFER_SIZE];
u08 * CircularBufferIn[CIRCULAR_BUFFER_SIZE];
u08 CircularBufferOutEnd;
u08 CircularBufferOutIndex;
u08 CircularBufferInEnd;
u08 CircularBufferInIndex;

#endif
