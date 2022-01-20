/***************************************************************************
* Fichier:		sonar.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			25 novembre 2020			   
* Description:  Implementation de la classe Sonar			   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef SONAR_H
#define SONAR_H

#include <util/delay.h>
#include "port.h"

class Sonar
{
public:
    Sonar();

    bool objectInRange();

private:
    Port portD_;
};

#endif