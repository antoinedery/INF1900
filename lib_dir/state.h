/***************************************************************************
* Fichier:		state.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			25 novembre 2020			   
* Description:  Implémentation de la classe State qui ajuste l'état d'un des 
                dispositifs (servo ou porte) selon l'opérande
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef STATE_H
#define STATE_H

#include "usart.h"
#include "keypad.h"
#include "door.h"
#include "servo.h"

class State
{
public:
    State();

    bool trySetValue(Usart &usart, Keypad &keypad);
    void execute(Door &door, Servo &servo);
    void printState(Usart &usart);

private:
    // device_ memorise la composante choisi
    uint8_t device_;
    
    // La valeur de la variable value_ 
    // depend de la variable device_. Value 
    // est utiliser ici pour sauvegarder 
    // l'angle sur 16 bit, ou pour memoriser 
    // si la porte est ouverte/fermer (0/1)
    uint16_t value_; 
    uint8_t angle_[3];
};

#endif
