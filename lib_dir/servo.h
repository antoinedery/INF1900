/***************************************************************************
* Fichier:		servo.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 novembre 2020			   
* Description:  Déclaration de la classe Servo qui configure le timer2
                en PWM pour faire fonctionner les servomoteurs			   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef SERVO_H
#define SERVO_H

#include <avr/io.h>
#include <util/delay.h>
#include "port.h"

class Servo
{
public:
    enum Tag
    {
        E = 0xE,
        F = 0xF
    };

    Servo();
    void setServo(const Tag &servo, const uint8_t &degrees);
    void stopServo();

private:
    Port portD_;
};

#endif