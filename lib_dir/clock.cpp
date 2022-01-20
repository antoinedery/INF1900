/***************************************************************************
* Fichier:		clock.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			20 novembre 2020			   
* Description:  Implementation de la classe Clock qui configurer le timer1
                en mode CTC et compte le nombre de fois que l'horloge s'est
                incremente pour la faire rouler 23h59			   
***************************************************************************/

#include "clock.h"

const uint8_t READ_PORT_A0 = 0;
const uint8_t RIGHT_BIT_SHIFT = 2;
const uint8_t DELAY_10MS = 20;
const uint8_t DELAY_10US = 5;
const uint8_t NULL_VALUE = 0;
const uint8_t TICKS_ONE_HOUR = 60;
const uint16_t ONE_HERTZ = 3906;      // 7812 inst. par seconde / 2
const uint16_t TICKS_24_HOURS = 2878; // (23*60)+59 = 1439. Mutliplié par 2 pour tenir compte du front descendant sur D5
const float CLOCK_MULTIPLIER = 60.0;
const float MAX_8_BITS = 255.0;

volatile uint16_t Clock::counter_ = 0;
volatile bool Clock::reachedEndTick_ = false;
volatile uint8_t Clock::previousVoltage = 0;
volatile uint8_t Clock::currentVoltage = 0;

/***************************************************************************
* Méthode:      Clock::Clock
* Description:  Constructeur par défaut de la classe Clock
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
Clock::Clock()
    : portD_(Port::D),
      can_()
{
    portD_.setOut((1 << DDD5) | (1 << DDD0)); //D5 (OC1A) et D0 en output
    portD_.writeOne(PORTD0);

    cli();
    TCCR1A = (1 << COM1A0);
    TIMSK1 = (1 << OCIE1A);
    TCNT1 = NULL_VALUE;
    sei();
}

/***************************************************************************
* Fonction:     Clock::setClock
* Description:  Affiche le temps passé en paramètre sur le cadran
* Paramètres:   (hour) : le nombre d'heures à afficher sur l'horloge
                (minute) : le nombre de minutes à afficher sur l'horloge           
* Retour:       Aucun       
****************************************************************************/
void Clock::setClock(const uint16_t &hour, const uint16_t &minute)
{
    uint16_t timeSetup = (hour * TICKS_ONE_HOUR) + minute;

    for (uint16_t i = 0; i < 2; i++)        //Deux boucles pour éviter les bogues dans SimulIDE
    {
        //Réinitialiser l'horloge
        portD_.writeOne(PORTD0);
        _delay_ms(DELAY_10MS);
        portD_.writeZero(PORTD0);
        _delay_ms(DELAY_10MS);

        for (uint16_t i = 0; i < timeSetup; i++)
        {
            portD_.writeOne(PORTD5);
            _delay_us(DELAY_10US);
            portD_.writeZero(PORTD5);
            _delay_us(DELAY_10US);
        }
    }
}

/***************************************************************************
* Fonction:     Clock::startClock
* Description:  Ajuste les registres nécessaires au Timer1 et le met en marche
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Clock::startClock()
{
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10);
    setClockSpeed();
}

/***************************************************************************
* Fonction:     Clock::catchNewVoltage
* Description:  Ajuste les registres nécessaires au Timer1 et le met en marche
* Paramètres:   (newVoltage) : le nouveau voltage           
* Retour:       Aucun       
****************************************************************************/
void Clock::catchNewVoltage(const uint8_t &newVoltage)
{
    currentVoltage = newVoltage;
}

/***************************************************************************
* Fonction:     Clock::incrementCounter
* Description:  Incrémente un compteur jusqu'à l'équivalent de 23h59
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Clock::incrementCounter()
{
    if (previousVoltage != currentVoltage)
    {
        previousVoltage = currentVoltage;
        setClockSpeed();
    }

    if (++counter_ >= TICKS_24_HOURS)
    {
        stopClock();
        reachedEndTick_ = true;
    }
}

/***************************************************************************
* Méthode:      Clock::getClockCounter
* Description:  Méthode qui retourne la valeur du compteur
* Paramètres:   Aucun             
* Retour:       La valeur du compteur      
****************************************************************************/
uint16_t Clock::getClockCounter()
{
    return counter_;
}

/***************************************************************************
* Méthode:      Clock::isCounting
* Description:  Vérifie si l'horloge a parcouru 23h59 depuis son depart
* Paramètres:   Aucun             
* Retour:       Booléen qui verifie si le compteur a parcouru 23h59 depuis son depart
****************************************************************************/
bool Clock::isComplete()
{
    return reachedEndTick_;
}

/***************************************************************************
* Fonction:     Clock::setClockSpeed
* Description:  Ajuste la vitesse du cadran selon la valeur lue par le convertisseur analogique-numérique
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Clock::setClockSpeed()
{
    if (previousVoltage == NULL_VALUE) //Évite de faire 255/0 avec la fonction utilisée dans le else
        OCR1A = ONE_HERTZ;
    else
        OCR1A = (MAX_8_BITS / (previousVoltage * CLOCK_MULTIPLIER)) * ONE_HERTZ;
}

/***************************************************************************
* Méthode:      Clock::resetClock
* Description:  Méthode qui reinitialise le compteur à 0 et qui réinitialise
                l'horloge à 88:88
* Paramètres:   Aucun             
* Retour:       Aucun  
****************************************************************************/
void Clock::resetClock()
{
    portD_.writeOne(PORTD0);
    counter_ = NULL_VALUE;
    reachedEndTick_ = false;
}

/***************************************************************************
* Fonction:     Clock::stopClock
* Description:  Arrête le cadran en mettant le registre TCCR1B en mode "normal"
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Clock::stopClock()
{
    TCCR1B = NULL_VALUE;
    OCR1A = NULL_VALUE;
}