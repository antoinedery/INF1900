/***************************************************************************
* Fichier:		Keypad.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			17 novembre 2020				   
* Description:  Délaration de la classe Keypad			   
***************************************************************************/

#ifndef KEYPAD_H
#define KEYPAD_H

#include "port.h"
#include "button.h"

class Keypad
{
public:
    Keypad(const Port& port);
    uint8_t scan();

private:
    Port port_;
    Button button_;
};

#endif
