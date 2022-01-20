/***************************************************************************
* Fichier:		main.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:		    28 novembre 2020				   		   		   
* Description: 	Projet final - INF1900 		   
***************************************************************************/

#define F_CPU 8000000

#include <avr/interrupt.h>
#include "debug.h"
#include "display.h"
#include "interrupt.h"


ISR(INT1_vect)
{
    Clock::stopClock();  
    Display::enableStopSimulation();
}

ISR(TIMER1_COMPA_vect)
{
    Clock::incrementCounter();
}


int main()
{
    Interrupt::setINT1();

    Display display;
    display.startRoutine();
}