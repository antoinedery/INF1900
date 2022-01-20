/***************************************************************************
* Fichier:		cast.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 décembre 2020			   		   		   
* Description: 	Déclaration de la classe statique Cast qui permet de convertir
                un entier en un caractere
***************************************************************************/

#ifndef CAST_H
#define CAST_H

#include <stdint.h>

class Cast
{
public:
    static char intToChar(const uint8_t &value);
};

#endif
