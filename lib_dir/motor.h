/***************************************************************************
* Fichier:		motor.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   
* Description:  Déclaration de la classe Motor qui permet de configurer un 
                PWM phase correct (8 bits) avec le Timer1 			   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef MOTOR_H
#define MOTOR_H

#include <avr/io.h>
#include "port.h"

class Motor
{
public:
    //Enum pour la direction du moteur
    enum Direction
    {
        BACKWARD,   // = 0
        FORWARD,    // = 1
        STATIONNARY // = 2
    };

    //Enum pour la direction des rotations
    enum Turn
    {
        RIGHT,
        LEFT
    };

public:
    Motor();

    void setWheels(const uint8_t &L_Speed, const Direction &L_Direct, const uint8_t &R_Speed, const Direction &R_Direct);

    void moveForward(const uint8_t &speed);
    void moveBackward(const uint8_t &speed);
    void turn(const Turn &turn);

    void blockLeftWheel();
    void blockRightWheel();

    uint8_t getLeftSpeed() const;
    uint8_t getRightSpeed() const;

    void invertDirectionLeft();
    void invertDirectionRight();

    void stopMotor();

private:
    Direction directionLeft_;
    Direction directionRight_;
    Port portD_;
};

#endif /* MotorPWM_H */