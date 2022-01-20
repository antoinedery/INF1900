/***************************************************************************
* Fichier:		time.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			25 novembre 2020			   
* Description:  Implémentation de la classe Time qui permet de créer un objet
                qui contient des heures et des minutes
***************************************************************************/

#include "time.h"
#include "cast.h"

const uint8_t FIRST_DIGIT_HOUR = 0;
const uint8_t SECOND_DIGIT_HOUR = 1;
const uint8_t FIRST_DIGIT_MINUTE = 2;
const uint8_t SECOND_DIGIT_MINUTE = 3;
const uint8_t FIRST_MINUTE_MAX_VALUE = 5;
const uint8_t MAX_DIGIT_OVERFLOW = 9;
const uint8_t TEN_MULTIPLIER = 10;
const uint8_t HUNDRED_MULTIPLIER = 100;
const uint8_t MAX_HOUR_VALUE = 23;
const uint8_t MAX_MINUTE_VALUE = 59;
const uint8_t NUMBER_DIGITS_HOUR = 4;

const char ERROR[] = "\n\nTemps invalide.\n\n";

/***************************************************************************
* Fonction:     Time::Time
* Description:  Constructeur par défaut de la classe Time
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
Time::Time() 
{}

/***************************************************************************
* Fonction:     Time::trySetValue
* Description:  Permet de récuperer l'heure entrée par l'utilisateur sur le clavier
* Paramètres:   (usart) : Référence vers le usart
                (keypad) : Référence vers le clavier        
* Retour:       Aucun       
****************************************************************************/
bool Time::trySetValue(Usart& usart, Keypad &keyPad)
{

    for (uint8_t i = 0; i < NUMBER_DIGITS_HOUR; i++)
    {
        usart.transmitChar(Cast::intToChar(time_[i] = keyPad.scan()));
        if(time_[i] > MAX_DIGIT_OVERFLOW){
            usart.transmitString(ERROR);
            return false;
        }
    }

    if (   getHour() > MAX_HOUR_VALUE 
        || getMinute() > MAX_MINUTE_VALUE 
        || time_[FIRST_DIGIT_MINUTE] > FIRST_MINUTE_MAX_VALUE)
    {
        usart.transmitString(ERROR);
        return false;
    }

    return true;
}

/***************************************************************************
* Fonction:     Time::getHour
* Description:  Permet de récuperer les deux premiers chiffres du temps (heure)
* Paramètres:   Aucun      
* Retour:       L'heure du temps      
****************************************************************************/
uint8_t Time::getHour()
{
    return time_[FIRST_DIGIT_HOUR] * TEN_MULTIPLIER + time_[SECOND_DIGIT_HOUR];
}

/***************************************************************************
* Fonction:     Time::getMinute
* Description:  Permet de récuperer les deux derniers chiffres du temps (minutes)
* Paramètres:   Aucun      
* Retour:       Les minutes du temps       
****************************************************************************/
uint8_t Time::getMinute()
{
    return time_[FIRST_DIGIT_MINUTE] * TEN_MULTIPLIER + time_[SECOND_DIGIT_MINUTE];
}

/***************************************************************************
* Fonction:     Time::getValue
* Description:  Permet de récuperer l'heure au complet 
* Paramètres:   Aucun      
* Retour:       La valeur complète du temps      
****************************************************************************/
uint16_t Time::getValue()
{
    return getHour() * HUNDRED_MULTIPLIER + getMinute();
}

/***************************************************************************
* Fonction:     Time::printTime
* Description:  Affiche l'heure dans le terminal de SimulIDE 
* Paramètres:   (usart) : Référence vers le usart      
* Retour:       Aucun
****************************************************************************/
void Time::printTime(Usart& usart)
{
    usart.transmitChar(Cast::intToChar(time_[FIRST_DIGIT_HOUR]));
    usart.transmitChar(Cast::intToChar(time_[SECOND_DIGIT_HOUR]));

    usart.transmitChar(':');

    usart.transmitChar(Cast::intToChar(time_[FIRST_DIGIT_MINUTE]));
    usart.transmitChar(Cast::intToChar(time_[SECOND_DIGIT_MINUTE]));
}
