/***************************************************************************
* Fichier:      motor.cpp			 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   
* Description:  Implémentation de la classe Motor qui permet de configurer un 
                PWM phase correct (8 bits) avec le Timer1 				   
***************************************************************************/

#include "motor.h"

const uint8_t NULL_SPEED = 0;
const uint8_t OVERFLOW_SPEED = 255;
const uint8_t LIMIT_SPEED = 254;

/***************************************************************************
* Fonction:     Motor::Motor
* Description:  Contructeur par défaut de la classe motorPWM - Initialise les registres
* Paramètres:   Aucun                
* Retour:       Aucun         
****************************************************************************/
Motor::Motor()
    : directionLeft_(STATIONNARY),
      directionRight_(STATIONNARY),
      portD_(Port::D)
{
    portD_.setOut((1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6)); //D3, D4 (OC1B), D5 (OC0A) et D6 en output

    //PWM Phase Correct 8 bits + Clear OC1A/OC1B à 1 en upcounting, set en downcounting.
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM10);
    TCCR1B = (1 << CS11); //CLK/8 (from prescaler)
    TCCR1C = 0;

    OCR1A = 0; //D5
    OCR1B = 0; //D4
}

/***************************************************************************
* Fonction:     Motor::getLeftSpeed
* Description:  Méthode qui retourne la vitesse du moteur gauche
* Paramètres:   Aucun               
* Retour:       La vitesse du moteur gauche        
****************************************************************************/
uint8_t Motor::getLeftSpeed() const
{
    return OCR1B;
}

/***************************************************************************
* Fonction:     Motor::getRightSpeed
* Description:  Méthode qui retourne la vitesse du moteur droit
* Paramètres:   Aucun               
* Retour:       La vitesse du moteur droit        
****************************************************************************/
uint8_t Motor::getRightSpeed() const
{
    return OCR1A;
}

/***************************************************************************
* Fonction:     Motor::moveForward
* Description:  Méthode qui fait avancer le robot selon une vitesse
* Paramètres:   (speed) : la vitesse des deux roues (valeur entre 0 et 255)               
* Retour:       Aucun       
****************************************************************************/
void Motor::moveForward(const uint8_t &speed)
{
    setWheels(speed, FORWARD, speed, FORWARD);
}

/***************************************************************************
* Fonction:     Motor::moveBackward
* Description:  Méthode qui fait reculer le robot selon une vitesse
* Paramètres:   (speed) : la vitesse des deux roues (valeur entre 0 et 255)               
* Retour:       Aucun       
****************************************************************************/
void Motor::moveBackward(const uint8_t &speed)
{
    setWheels(speed, BACKWARD, speed, BACKWARD);
}

/***************************************************************************
* Fonction:     Motor::setWheels
* Description:  Méthode qui met en marche les moteurs selon une vitesse et une direction
* Paramètres:   (L_Speed)   : PWM du moteur gauche (valeur entre 0 et 255)
                (L_Direct)  : Direction du moteur gauche
                (R_Speed)   : PWM du moteur droit  (valeur entre 0 et 255)
                (R_Direct)  : Direction du moteur droit                              
* Retour:       Aucun       
****************************************************************************/
void Motor::setWheels(const uint8_t &L_Speed, const Direction &L_Direct,
                      const uint8_t &R_Speed, const Direction &R_Direct)
{
    uint8_t leftSpeed = L_Speed;
    uint8_t rightSpeed = R_Speed;

    if (leftSpeed >= OVERFLOW_SPEED)
        leftSpeed = LIMIT_SPEED;
    if (rightSpeed >= OVERFLOW_SPEED)
        rightSpeed = LIMIT_SPEED;

    directionLeft_ = L_Direct;
    directionRight_ = R_Direct;

    uint8_t wheelsDirection = 0; //D3 et D6 servent à la direction de la roue
    if (L_Direct == STATIONNARY &&
        R_Direct == STATIONNARY)
    {
        rightSpeed = 0;
        leftSpeed = 0;
    }
    else if (L_Direct == STATIONNARY &&
             R_Direct != STATIONNARY)
    {
        leftSpeed = 0;
        wheelsDirection = R_Direct << PORTD6;
    }
    else if (L_Direct != STATIONNARY &&
             R_Direct == STATIONNARY)
    {
        rightSpeed = 0;
        wheelsDirection = L_Direct << PORTD3;
    }
    else
        wheelsDirection = (L_Direct << PORTD3) |
                          (R_Direct << PORTD6);

    portD_.write(wheelsDirection);
    OCR1A = rightSpeed;
    OCR1B = leftSpeed;
}

/***************************************************************************
* Fonction:     Motor::blockLeftWheel
* Description:  Méthode qui rend stationnaire la roue gauche
* Paramètres:   Aucun                            
* Retour:       Aucun       
****************************************************************************/
void Motor::blockLeftWheel()
{
    OCR1B = 0;
    directionLeft_ = STATIONNARY;
}

/***************************************************************************
* Fonction:     Motor::blockRightWheel
* Description:  Méthode qui rend stationnaire la roue droite
* Paramètres:   Aucun                            
* Retour:       Aucun       
****************************************************************************/
void Motor::blockRightWheel()
{
    OCR1A = 0;
    directionRight_ = STATIONNARY;
}

/***************************************************************************
* Fonction:     Motor::invertDirectionLeft
* Description:  Méthode qui inverse la direction du moteur gauche
* Paramètres:   Aucun      
* Retour:       Aucun        
****************************************************************************/
void Motor::invertDirectionLeft()
{
    if (directionLeft_ == FORWARD)
    {
        directionLeft_ = BACKWARD;
    }
    else
    {
        directionLeft_ = FORWARD;
    }
    portD_.write((directionLeft_ << PORTD3) | (directionRight_ << PORTD6));
}

/***************************************************************************
* Fonction:     Motor::invertDirectionLeft
* Description:  Méthode qui inverse la direction du moteur gauche
* Paramètres:   Aucun      
* Retour:       Aucun        
****************************************************************************/
void Motor::invertDirectionRight()
{
    if (directionRight_ == FORWARD)
    {
        directionRight_ = BACKWARD;
    }
    else
    {
        directionRight_ = FORWARD;
    }
    portD_.write((directionLeft_ << PORTD3) | (directionRight_ << PORTD6));
}

/***********************************************************************
* Fonction:     Motor::turn
* Description:  Méthode qui faire tourner le robot à gauche ou à droite
* Paramètres:   (turn)  : La rotation (LEFT ou RIGHT)      
* Retour:       Aucun       
***********************************************************************/
void Motor::turn(const Turn &turn)
{
    //Verifie si le robot est presentement entrain d'avancer
    if (directionRight_ == FORWARD && directionLeft_ == FORWARD)
    {
        switch (turn)
        {
        case RIGHT:
            blockRightWheel();
            break;
        case LEFT:
            blockLeftWheel();
        }
    }
    //Verifie si le robot est presentemnt entrain de reculer
    else if (directionRight_ == BACKWARD &&
             directionLeft_ == BACKWARD)
    {
        switch (turn)
        {
        case RIGHT:
            blockLeftWheel();
            break;
        case LEFT:
            blockRightWheel();
        }
    }
    //Verifie si le robot est presentement entrain de tourner a droite ou a gauche
    //lorsque la roue droite est stationnaire
    else if (directionRight_ == STATIONNARY &&
             directionLeft_ != STATIONNARY)
    {
        switch (turn)
        {
        case RIGHT:
            if (directionLeft_ == BACKWARD)
                setWheels(NULL_SPEED, STATIONNARY, //roue gauche
                          OCR1B, BACKWARD);        //roue droite
            break;
        case LEFT:
            if (directionLeft_ == FORWARD)
                setWheels(NULL_SPEED, STATIONNARY,
                          OCR1B, FORWARD);
        }
    }

    //Verifie si le robot est presentement entrain de tourner a droite ou a gauche
    else if (directionRight_ != STATIONNARY &&
             directionLeft_ == STATIONNARY)
    {
        switch (turn)
        {
        case RIGHT:
            if (directionRight_ == FORWARD)
                setWheels(OCR1A, FORWARD,
                          NULL_SPEED, STATIONNARY);
            break;
        case LEFT:
            if (directionRight_ == BACKWARD)
                setWheels(OCR1A, BACKWARD,
                          NULL_SPEED, STATIONNARY);
        }
    }
    //Si les deux roues sont stationnaires
    else
    {
        switch (turn)
        {
        case RIGHT:
            setWheels(LIMIT_SPEED, FORWARD,
                      NULL_SPEED, STATIONNARY);
            break;
        case LEFT:
            setWheels(NULL_SPEED, STATIONNARY,
                      LIMIT_SPEED, FORWARD);
        }
    }
}

/***************************************************************************
* Fonction:     Motor::stopMotor
* Description:  Méthode qui arrête les deux moteurs
* Paramètres:   Aucun      
* Retour:       Aucun        
****************************************************************************/
void Motor::stopMotor()
{
    directionLeft_ = STATIONNARY;
    directionRight_ = STATIONNARY;
    OCR1A = 0;
    OCR1B = 0;
}
