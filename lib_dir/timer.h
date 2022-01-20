/***************************************************************************
* Fichier:		timer.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   
* Description:  Déclaration de la classe timer qui permet de configurer
                une minuterie en mode CTC				   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef TIMER_H
#define TIMER_H

#include <avr/io.h>
#include <avr/interrupt.h>

class Timer
{
public:
    Timer();

    void setDuration(const uint16_t &duration);
    void startTimer(const uint16_t &duration);
    void stopTimer();

private:
};

#endif
