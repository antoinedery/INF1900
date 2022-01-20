/***************************************************************************
* Fichier:		door.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 novembre 2020			   
* Description:  Déclaration de la classe door			   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef DOOR_H
#define DOOR_H

#include <util/delay.h>
#include "port.h"


class Door
{
public:
    enum Tag{
        A = 0xA,
        B = 0xB,
        C = 0xC,
        D = 0xD
    };
    
    Door(const Port& port);
    void close(const Tag& tag);
    void open(const Tag& tag);
   
private:
    void send(const uint32_t& printValue);
    void pulseClock();
    uint32_t returnShift(const Tag& tag);

private:
    Port port_;
    uint32_t previousDisplay_;
};

#endif