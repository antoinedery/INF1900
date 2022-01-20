/***************************************************************************
* Fichier:		Delay.h 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   		   		   
* Description: 	Déclaration de la classe delay		   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>
#include <util/delay.h>

class Delay
{
public:
    static void inMS(const uint16_t & cycles);
    static void inUS(const uint16_t & cycles);
};

#endif
