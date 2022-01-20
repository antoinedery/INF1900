/***************************************************************************
* Fichier:		action.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 décembre 2020			   		   		   
* Description: 	Implémentation de la classe Action		   
***************************************************************************/

#include "action.h"

const uint8_t CLOCK_TICKS_ONE_HOUR = 60;
const uint8_t HOURS_FULL_DAY = 24;
const uint16_t TICKS_FULL_DAY = 1440; //24*60 = 1440 ticks d'horloge
const uint8_t DOUBLE_TICKS = 2;
const uint8_t NULL_VALUE = 0;

/***************************************************************************
* Fonction:     Action::Action
* Description:  Constructeur par défaut de la classe Action
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
Action::Action()
    : clockTicks_(NULL_VALUE),
      insertPriority_(NULL_VALUE)
{
}

/***************************************************************************
* Fonction:     Action::setValues
* Description:  Permet de définir les valeurs de l'heure et l'état d'une action
* Paramètres:   (time) : l'heure de l'action
                (state) : État de l'action (dispositif et operande)         
* Retour:       Aucun       
****************************************************************************/
void Action::setValues(const Time &time, const State &state)
{
    time_ = time;
    state_ = state;
}

/***************************************************************************
* Fonction:     Action::getTime
* Description:  Permet de retourner l'heure d'une action
* Paramètres:   Aucun
* Retour:       Aucun       
****************************************************************************/
uint16_t Action::getTime()
{
    return time_.getValue();
}

/***************************************************************************
* Fonction:     Action::printAction
* Description:  Permet d'afficher l'action dans le terminal de SimulIDE
* Paramètres:   (usart) : référence vers le usart
* Retour:       Aucun       
****************************************************************************/
void Action::printAction(Usart &usart)
{
    time_.printTime(usart);
    usart.transmitString(" - ");
    state_.printState(usart);
}

/***************************************************************************
* Fonction:     Action::setClockTicks
* Description:  Permet de calculer le nombre de "ticks" entre l'action et l'heure de départ
* Paramètres:   (simHour) : Heure de départ de la simulation
                (simMin) : Minute de départ de la simulation
* Retour:       Aucun       
****************************************************************************/
void Action::setClockTicks(const uint8_t &simHour, const uint8_t &simMin)
{
    uint8_t actHour = time_.getHour();
    uint8_t actMin = time_.getMinute();

    clockTicks_ = NULL_VALUE;

    if (actHour != simHour)
    {
        if (actHour > simHour)
            clockTicks_ += (actHour - simHour) * CLOCK_TICKS_ONE_HOUR;
        else
            clockTicks_ += ((HOURS_FULL_DAY - simHour) + actHour) * CLOCK_TICKS_ONE_HOUR;
    }

    if (actMin != simMin)
    {
        if (actMin > simMin)
            clockTicks_ += actMin - simMin;
        else
        {
            if (clockTicks_ == NULL_VALUE)
                clockTicks_ = TICKS_FULL_DAY; //Ex : sim = 14:20 et Action = 14:10 alors 23:50 entre les deux
            clockTicks_ -= simMin - actMin;
        }
    }

    if (clockTicks_ == NULL_VALUE)
        return;

    clockTicks_ = (clockTicks_ * DOUBLE_TICKS) - 1; //-1 ici pour synchroniser les actions précisément avec l'heure
}

/***************************************************************************
* Fonction:     Action::getClockTicks
* Description:  Permet de retourner le nombre de "ticks" entre l'action et l'heure de départ
* Paramètres:   Aucun
* Retour:       Aucun       
****************************************************************************/
uint16_t Action::getClockTicks()
{
    return clockTicks_;
}

/***************************************************************************
* Fonction:     Action::executeAction
* Description:  Permet d'exécution l'action prévue
* Paramètres:   (door) : référence vers la porte
                (servo) : référence vers le servo moteur
* Retour:       Aucun       
****************************************************************************/
void Action::executeAction(Door &door, Servo &servo)
{
    state_.execute(door, servo);
}

/***************************************************************************
* Fonction:     Action::setInsertPriority
* Description:  Permet de mettre une priorité à une action
* Paramètres:   (index) : l'index de l'action
* Retour:       Aucun       
****************************************************************************/
void Action::setInsertPriority(const uint8_t &index)
{
    insertPriority_ = index;
}

/***************************************************************************
* Fonction:     Action::getInsertPriority
* Description:  Permet de récuperer la priorité d'une action
* Paramètres:   Aucun
* Retour:       La priorité de l'action       
****************************************************************************/
uint8_t Action::getInsertPriority()
{
    return insertPriority_;
}
