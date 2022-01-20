/***************************************************************************
* Fichier:		Led.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   		   		   
* Description: 	Implementation de la classe Led		   
***************************************************************************/

#include <led.h>

const uint8_t ONE_CYCLE = 1;
const uint8_t CYCLE_DIVIDER = 2;

/***************************************************************************
* Méthode:      Led::Led
* Description:  Constructeur par paramètres de la classe Led
* Paramètres:   (port)     :   port qui sera utiliser pour le branchement de la led 
                (firstPin) :   Broche associe a la couleur rouge
                (secondPin):   Broche associe a la couleur verte           
* Retour:       Aucun         
****************************************************************************/
Led::Led(const Port& port, const uint8_t &firstPin, const uint8_t &secondPin)
    : port_(port),
      firstPin_(firstPin),
      secondPin_(secondPin)
{
    port_.setOut(_BV(firstPin_) | _BV(secondPin_));
}

/***************************************************************************
* Méthode:      Led::turnOn
* Description:  Méthode qui allume la couleur choisit. La couleur ambre 
                n'allume pas si la methode n'est pas dans une boucle
* Paramètres:   (color)   : couleur choisi            
* Retour:       Aucun   
****************************************************************************/
void Led::turnOn(const Color &color)
{
    switch (color)
    {
    case AMBER:
        timelapse(Led::RED, ONE_CYCLE);
        timelapse(Led::GREEN, ONE_CYCLE);
        return;
    case RED:
        port_.switchValues(firstPin_, secondPin_);
        break;
    case GREEN:
        port_.switchValues(secondPin_, firstPin_);
    }
}

/***************************************************************************
* Méthode:      Led::timelapse
* Description:  Méthode qui allume la couleur choisie pour une periode donnee
* Paramètres:   (color) : couleur choisi
*               (cycle) : nombre de cycles qui executent un delai de 5ms
* Retour:       Aucun  
****************************************************************************/
void Led::timelapse(const Color &color, const uint16_t &cycles)
{
    if (color == Led::AMBER)
    {
        uint16_t i = 0, dividedCycles = cycles / CYCLE_DIVIDER; //cyclesx2x5ms
        for (; i < dividedCycles; i++)
            turnOn(Led::AMBER);
        return;
    }
    turnOn(color);
    Delay::inMS(cycles); //cycles x 5ms
}

/***************************************************************************
* Méthode:      Led::turnOff()
* Description:  Méthode qui eteint la del (met la sortie du port a zero)
* Paramètres:   Aucun
* Retour:       Aucun   
****************************************************************************/
void Led::turnOff()
{
    port_.clear();
}
