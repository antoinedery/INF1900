/***************************************************************************
* Fichier:      timer.cpp			 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   
* Description:  Implémentation de la classe timer qui permet de configurer
                une minuterie en mode CTC	
***************************************************************************/

#include "timer.h"

/***************************************************************************
* Fonction:     Timer::Timer
* Description:  Constructeur par défaut de la classe Timer
* Paramètres:   Aucun               
* Retour:       Aucun    
****************************************************************************/
Timer::Timer()
{
}

/***************************************************************************
* Fonction:     Timer::setDuration
* Description:  Méthode qui met à jour la durée de la minuterie
* Paramètres:   (duration) : la duree de la minuterie (en ms)              
* Retour:       Aucun    
****************************************************************************/
void Timer::setDuration(const uint16_t &duration)
{
    OCR1A = duration*8; // (8'000'000/1'024)/1000 = 7.8 inst/ms = 8 ms
}

/***************************************************************************
* Fonction:     Timer::startTimer
* Description:  Méthode qui part la minuterie
* Paramètres:   (duration) : la duree de la minuterie (en ms)              
* Retour:       Aucun    
****************************************************************************/
void Timer::startTimer(const uint16_t &duration)
{
    TCNT1 = 0;
    OCR1A = duration * 8;                               // (8'000'000/1'024)/1'000 = 7.8 inst/ms
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // Mode CTC, Division de l'horloge par 1024
    TIMSK1 = (1 << OCIE1A);
}

/***************************************************************************
* Fonction:     Timer::stopTimer
* Description:  Méthode qui arrête la minuterie
* Paramètres:   Aucun              
* Retour:       Aucun    
****************************************************************************/
void Timer::stopTimer()
{
    TCCR1B = 0;
}
