/***************************************************************************
 * Fichier:     interrupt.h			 					   
 * Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
 * Équipe:		03-18					   
 * Section:		01					   
 * Date:        4 décembre 2020				   
 * Description: Implémentation de la classe Interrupt			   
***************************************************************************/

#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <avr/interrupt.h>

class Interrupt
{
public:
    static void setINT1();
};

#endif 
