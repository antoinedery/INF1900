/***************************************************************************
 * Fichier:     interrupt.cpp			 					   
 * Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
 * Équipe:		03-18					   
 * Section:		01					   
 * Date:        4 décembre 2020				   
 * Description: Implémentation de la classe Interrupt			   
***************************************************************************/

#include "interrupt.h"

/***************************************************************************
* Méthode:      Interrupt::setINT1
* Description:  Méthode ajuste les registres pour l'interruption du port INT1 (D3)
* Paramètres:   Aucun              
* Retour:       Aucun   
****************************************************************************/
void Interrupt::setINT1()
{
    cli ();
    //on enable les interruption int0 int1
    EIMSK |= (1 << INT1);
    //On ajuste le bounton en front descendant
    EICRA |= (1 << ISC11) | (0 << ISC10);
    sei ();
}
