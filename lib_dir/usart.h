/***************************************************************************
* Fichier:		USART.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   
* Description: 	Déclaration de la classe USART				   
***************************************************************************/

#ifndef USART_H
#define USART_H

#include <stdint.h>
#include <avr/io.h>

class Usart
{
public:
    Usart();
    
    void transmitChar(const uint8_t &data);
    void transmitString(const char *src);

    uint8_t receive();

private:
};

#endif /* USART_H */
