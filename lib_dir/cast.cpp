/***************************************************************************
* Fichier:		cast.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 décembre 2020			   		   		   
* Description: 	Implémentation de la classe statique Cast qui permet de convertir
                un entier en un caractere (utile pour la transmission USART)
***************************************************************************/

#include "cast.h"

/***************************************************************************
* Fonction:     Cast::intToChar
* Description:  Permet de convertir une valeur numérique en caractère
* Paramètres:   (value) : la valeur numérique à convertir          
* Retour:       Aucun       
****************************************************************************/
char Cast::intToChar(const uint8_t &value)
{
    switch (value)
    {
    case 0:
        return '0';
    case 1:
        return '1';
    case 2:
        return '2';
    case 3:
        return '3';
    case 4:
        return '4';
    case 5:
        return '5';
    case 6:
        return '6';
    case 7:
        return '7';
    case 8:
        return '8';
    case 9:
        return '9';
    case 0xA:
        return 'A';
    case 0xB:
        return 'B';
    case 0xC:
        return 'C';
    case 0xD:
        return 'D';
    case 0xE:
        return 'E';
    case 0xF:
        return 'F';
    }
    return 0;
}