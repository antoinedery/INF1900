/***************************************************************************
* Fichier:		Delay.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   		   		   
* Description: 	Implementation de la classe delay		   
***************************************************************************/

#include "delay.h"

const double DELAY_MS = 0.01;
const uint8_t DELAY_US = 1;

/***************************************************************************
* Méthode:      Delay::delayMS
* Description:  Methode qui permet de creer un delai pour un temps voulu en 
*               en fonction des millisecondes
* Paramètres:   (cycles)   :   nombres de cycles qui executent le delai               
* Retour:       Aucun      
* Note :        Les intervalles de temps possibles sont de 10 ms a 655 350 ms  
****************************************************************************/
void Delay::inMS(const uint16_t &cycles)
{
    for (uint16_t i = 0; i < cycles; i++)
    {
        _delay_ms(DELAY_MS);
    }
}

/***************************************************************************
* Méthode:      Delay::delayUS
* Description:  Methode qui permet de creer un delai pour un temps voulu en 
*               en fonction des microsecondes
* Paramètres:   (cycles)   :   nombres de cycles qui executent le delai               
* Retour:       Aucun         
****************************************************************************/
void Delay::inUS(const uint16_t &cycles)
{
    for (uint16_t i = 0; i < cycles; i++)
    {
        _delay_us(DELAY_US);
    }
}
