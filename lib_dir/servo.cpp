/***************************************************************************
* Fichier:		servo.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 novembre 2020			   
* Description:  Implémentation de la classe Servo qui permet de positionner
                le bras du servomoteur selon l'angle voulu
***************************************************************************/

#include "servo.h"

const uint8_t NULL_VALUE = 0;
const uint8_t ADDITION_ONE = 1;
const float DIVIDER_FREQUENCY_DEGRES = 180.0; //180.0 degrés
const float MULTIPLIER_FREQUENCY = 125;       //187.5 / 1.5 ms

/***************************************************************************
* Méthode:      Servo::Servo
* Description:  Constructeur par défaut de la classe Servo
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
Servo::Servo()
    : portD_(Port::D)
{
    TCCR2A = (1 << WGM21) | (1 << WGM20) | (1 << COM2A1) | (1 << COM2B1);
    setServo(E, NULL_VALUE);
    setServo(F, NULL_VALUE);
}

/***************************************************************************
* Fonction:     Servo::setServo
* Description:  Place la bras du servomoteur (E ou F) a l'angle passé en paramètre
* Paramètres:   (servo) : Le servomoteur E ou F
                (degrees) : L'angle en degrees (0 à 180)          
* Retour:       Aucun       
****************************************************************************/
void Servo::setServo(const Tag &servo, const uint8_t &degrees)
{
    TCNT2 = NULL_VALUE;

    if (servo == E)
    {
        portD_.setOut((1 << DDD6)); //permet d'éviter les problèmes que les 2 servos changent en même temps
        TCCR2B = (1 << CS22);
        OCR2B = ((degrees / DIVIDER_FREQUENCY_DEGRES) + ADDITION_ONE) * MULTIPLIER_FREQUENCY; //Assigne une valeur entre 0 et 255 pour que le "duty cycle" soit entre 1 et 2 ms.
        do
        {
        } while (TCNT2 < OCR2B);

    }
    else
    {
        portD_.setOut((1 << DDD7)); //permet d'éviter les problèmes que les 2 servos changent en même temps
        TCCR2B = (1 << CS22);
        OCR2A = ((degrees / DIVIDER_FREQUENCY_DEGRES) + ADDITION_ONE) * MULTIPLIER_FREQUENCY;
        do
        {
        } while (TCNT2 < OCR2A);

    }
    stopServo();
}

/***************************************************************************
* Fonction:     Servo::stopServo
* Description:  Remet le servo moteur à sa position initiale, soit à 90 degrés.
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Servo::stopServo()
{
    TCCR2B = NULL_VALUE;
    portD_.writeZero(PORTD6);
    portD_.writeZero(PORTD7);
    portD_.setIn((1 << DDD6) | (1 << DDD7));        //Évite les bogues sur SimulIDE
}