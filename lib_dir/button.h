/***************************************************************************
* Fichier:		Button.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   		   		   
* Description: 	Déclaration de la classe Button		   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef BUTTON_H
#define BUTTON_H

#include <util/delay.h>
#include <port.h>

class Button
{
public:
    Button(const Port & port, const uint8_t& pin);

    bool isPressed();
    bool isClicked();

private:
    Port port_; //Registre de lecture Pin de l'AVR (Permet a l'utilisateur peut prendre une des broches de A,B,C ou D)
    const uint8_t pin_;
};

#endif
