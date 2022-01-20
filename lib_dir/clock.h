/***************************************************************************
* Fichier:		clock.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			20 novembre 2020			   
* Description:  Déclaration de la classe Clock qui configurer le timer1
                en mode CTC et compte le nombre de fois que l'horloge s'est
                incremente pour la faire rouler 23h59	 			   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef CLOCK_H
#define CLOCK_H

#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "port.h"
#include "can.h"

class Clock
{
public:
    Clock();

    void setClock(const uint16_t &hour, const uint16_t &minute);

    uint16_t getClockCounter();

    static void startClock();
    static void incrementCounter();
    static void setClockSpeed();
    static void stopClock();

    bool isComplete();

    void catchNewVoltage(const uint8_t& voltage);

    void resetClock();

private:
    Port portD_;
    can can_;
    static volatile uint16_t counter_;
    static volatile bool reachedEndTick_;

    static volatile uint8_t previousVoltage;
    static volatile uint8_t currentVoltage;
};

#endif