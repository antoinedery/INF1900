/***************************************************************************
* Fichier:		action.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 décembre 2020			   		   		   
* Description: 	Déclaration de la classe Action		   
***************************************************************************/

#ifndef ACTION_H
#define ACTION_H

#include "time.h"
#include "state.h"
#include <stdint.h>

class Action
{
public:
    Action();

    void setValues(const Time &time, const State &state);

    uint16_t getTime();

    void printAction(Usart &usart);

    void setClockTicks(const uint8_t &simHour, const uint8_t &simMin);
    uint16_t getClockTicks();

    void executeAction(Door &door, Servo &servo);

    void setInsertPriority(const uint8_t &index);
    uint8_t getInsertPriority();

private:
    Time time_;
    State state_;
    uint16_t clockTicks_;
    uint8_t insertPriority_;
};

#endif
