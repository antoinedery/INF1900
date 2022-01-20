/***************************************************************************
* Fichier:		Led.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   		   		   
* Description: 	Déclaration de la classe Led		   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef LED_H
#define LED_H

#include <port.h>
#include <delay.h>

class Led
{
public:
    enum Color
    {
        RED,
        GREEN,
        AMBER
    };

public:
    enum tag
    {
        MS,
        US
    };

public:
    Led(const Port&, const uint8_t &, const uint8_t &);

    void turnOn(const Color &);
    void timelapse(const Color &, const uint16_t &);
    void turnOff();

private:
    Port port_;
    const uint8_t firstPin_;
    const uint8_t secondPin_;
};

#endif