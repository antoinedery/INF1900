/***************************************************************************
* Fichier:		time.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			25 novembre 2020			   
* Description:  Déclaration de la classe Time qui permet de créer un objet
                qui contient des heures et des minutes
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef TIME_H
#define TIME_H

#include "usart.h"
#include "keypad.h"

class Time
{
public:
    Time();

    bool trySetValue(Usart &usart, Keypad &keypad);
    uint8_t getHour();
    uint8_t getMinute();
    uint16_t getValue();

    void printTime(Usart &usart);

private:
    uint8_t time_[4];
};
#endif
