/***************************************************************************
* Fichier:		state.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			25 novembre 2020			   
* Description:  Déclaration de la classe State qui ajuste l'état d'un des 
                dispositifs (servo ou porte) selon l'opérande
***************************************************************************/

#include "state.h"
#include "cast.h"

const uint8_t NULL_VALUE = 0;
const uint8_t TEN_DIVIDER = 10;
const uint8_t HUNDRED_VALUE = 100;
const uint8_t NUMBER_DIGITS = 9;
const uint8_t THREE_DIGITS_ANGLE = 3;
const uint8_t MAX_ANGLE = 180;
const uint8_t OPEN_DOOR = 0;
const uint8_t CLOSE_DOOR = 1;
const uint8_t DOOR_A = 0xA;
const uint8_t DOOR_B = 0xB;
const uint8_t DOOR_C = 0xC;
const uint8_t DOOR_D = 0xD;
const uint8_t SERVO_E = 0xE;
const uint8_t SERVO_F = 0xF;

const char ERROR[] = "\n\nEtat invalide.\n\n";

/***************************************************************************
* Fonction:     State::State
* Description:  Constructeur par défaut de la classe State
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
State::State()
    : device_(NULL_VALUE),
      value_(NULL_VALUE)
{
}

/***************************************************************************
* Fonction:     State::trySetValue
* Description:  Permet de récuperer le dispositif et l'operande entrée par l'utilisateur sur le clavier
* Paramètres:   (usart) : Référence vers le usart
                (keypad) : Référence vers le clavier        
* Retour:       Bool si les valeurs saisies sont valides       
****************************************************************************/
bool State::trySetValue(Usart& usart, Keypad &keyPad)
{
    uint8_t temp;
    uint8_t j = HUNDRED_VALUE;

    device_ = keyPad.scan();

    //Erreur si valeur entre 0 et 9
    if (device_ < DOOR_A)
    {
        usart.transmitString(ERROR);
        return false;
    }

    usart.transmitChar(Cast::intToChar(device_));

    //Récuperer l'angle du servomoteur
    if (device_ == SERVO_E || device_ == SERVO_F)
    {
        for (uint8_t i = 0; i < THREE_DIGITS_ANGLE; i++)
        {
            usart.transmitChar(Cast::intToChar(temp = keyPad.scan()));
            if (temp <= NUMBER_DIGITS)
            {
                angle_[i] = temp;
                value_ += temp * j;
                j /= TEN_DIVIDER;
            }
            else
            {
                usart.transmitString(ERROR);
                return false;
            }
        }

        if (value_ > MAX_ANGLE)
        {
            usart.transmitString(ERROR);
            return false;
        }
    }
    else
    {
        usart.transmitChar(Cast::intToChar(value_ = keyPad.scan()));

         //Récuperer la valeur de la porte (ouvrir ou fermer)
        if (!(value_ == OPEN_DOOR || value_ == CLOSE_DOOR))
        {
            usart.transmitString(ERROR);
            return false;
        }
    }

    usart.transmitString("\n\n");
    return true;
}

/***************************************************************************
* Fonction:     State::execute
* Description:  Exécute les actions (fermer-ouvrir portes et placer le servo)
* Paramètres:   (door) : Référence vers la porte
                (servo) : Référence vers le servo         
* Retour:       Aucun       
****************************************************************************/
void State::execute(Door &door, Servo &servo)
{
    if (device_ == SERVO_E || device_ == SERVO_F){
        servo.setServo(static_cast<Servo::Tag>(device_), value_);
        return;
    }
    if (value_ == OPEN_DOOR)
        door.open(static_cast<Door::Tag>(device_));
    else
        door.close(static_cast<Door::Tag>(device_));
}

/***************************************************************************
* Fonction:     State::printState
* Description:  Affiche l'action dans le terminal de SimulIDE
* Paramètres:   (usart) : Référence vers le usart    
* Retour:       Aucun       
****************************************************************************/
void State::printState(Usart& usart)
{
    if (device_ == SERVO_E || device_ == SERVO_F)
    {
        usart.transmitString("orienter servomoteur ");

        usart.transmitChar(Cast::intToChar(device_));

        usart.transmitString(" a ");

        for (uint8_t i = 0; i < THREE_DIGITS_ANGLE; i++)
        {
            usart.transmitChar(Cast::intToChar(angle_[i]));
        }

        usart.transmitString(" degres");
    }

    else
    {
        if (value_ == NULL_VALUE)
        {
            usart.transmitString("ouvrir ");
        }
        else
        {
            usart.transmitString("fermer ");
        }
        usart.transmitString("porte ");
        usart.transmitChar(Cast::intToChar(device_));
    }
}
