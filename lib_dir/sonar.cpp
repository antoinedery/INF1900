/***************************************************************************
* Fichier:		sonar.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			25 novembre 2020			   
* Description:  Implementation de la classe Sonar			   
***************************************************************************/

#include "sonar.h"

const float CONVERT_TO_CM = 27.36; //À 2V, compteur = 5472 --> 5472/200 cm = 27.36
const uint8_t DELAY_10US = 10;
const uint8_t DELAY_1US = 1;
const uint8_t NULL_VALUE = 0;
const uint8_t DISTANCE_MAX = 200; //Si un objet à < 200 cm, arrêter l'horloge
const uint16_t TIME_NO_OBJECT_US = 36000;

/***************************************************************************
* Méthode:      Sonar::Sonar
* Description:  Constructeur par défaut de la classe Sonar
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
Sonar::Sonar()
    : portD_(Port::D)
{
    portD_.setOut(1 << PORTD2); //A7 en output (Trig)
    portD_.setIn(1 << PORTD1);  //A6 en input (Echo) A6
}

/***************************************************************************
* Fonction:     Sonar::objectInRange()
* Description:  Permet de trigger, calculer le temps de détection d'un objet
                et retourne un booléen si l'objet est à moins de 2 mètres
* Paramètres:   Aucun 
* Retour:       Un booléen qui vérifie si un objet est à moins de 2 metres
****************************************************************************/
bool Sonar::objectInRange()
{
    //Trigger
    PORTD |= (1 << PORTD2);
    _delay_us(DELAY_10US);
    PORTD &= ~(1 << PORTD2);

    //Attendre le Echo
    uint16_t counterEcho = NULL_VALUE;
    while (!(PIND & (1 << PORTD1)))
        ;

    while (PIND & (1 << PORTD1))
    {
        counterEcho++;
        _delay_us(DELAY_1US);
        if (counterEcho > TIME_NO_OBJECT_US)
        {
            counterEcho = NULL_VALUE;
            break;
        }
    }

    //Conversion en cm
    counterEcho /= CONVERT_TO_CM;
    if (counterEcho < DISTANCE_MAX)
        return true;
    return false;
}