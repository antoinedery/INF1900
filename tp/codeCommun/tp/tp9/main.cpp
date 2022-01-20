/***************************************************************************
* Fichier:		main.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:		    10 novembre 2020				   		   		   
* Description: 	Faire bouger un robot selon une trajectorie predefinie		   
***************************************************************************/

#define F_CPU 8000000
#include "led.h"
#include "motor.h"
#include "file.h"
#include "compass.h"
#include "debug.h"

//Variables globales
const uint16_t CYCLE_SETTER = 5;
const uint16_t ADDRESS_ZERO = 0;
const uint16_t TURN_FINISHED = 400;
const uint16_t ADDRESS_BEGIN = 2;
const uint8_t  ZERO_ITERATION = 0;
const uint8_t  MIDDLE_EDGE = 128;
const uint8_t  TIME_MULTIPLIER = 25;
const float    POURCENTAGE_MULTIPLIER = 100/255.0;


//Variables d'interprétation du byte code
const uint8_t DBT = 0x01;  //Debut
const uint8_t ATT = 0x02;  //Attendre
const uint8_t DAL = 0x44;  //Allumer la DEL bicolore
const uint8_t DET = 0x45;  //Eteindre la DEL bicolore
const uint8_t MON = 0x18;  //Allumer la matrice de DEL
const uint8_t MOF = 0x19;  //Eteindre la matrice de DEL
const uint8_t MAR1 = 0x60; //Arreter les moteurs
const uint8_t MAR2 = 0x61; //Arreter les moteurs
const uint8_t MAV = 0x62;  //Avancer
const uint8_t MRE = 0x63;  //Reculer
const uint8_t TRD = 0x64;  //Tourner a droite
const uint8_t TRG = 0x65;  //Tourner a gauche
const uint8_t DBC = 0xC0;  //Debut de boucle
const uint8_t FBC = 0xC1;  //Fin de boucle
const uint8_t FIN = 0xFF;  //Fin

int main()
{
    //Initialisation des classes utilisées
    Port matrix4x4(Port::C, Port::OUT);
    Led led(Port::A, PORTA0, PORTA1);
    Compass compass(Port::B, Compass::NORTH); // On assume que la boussole est toujours branchee sur les 6 premieres broches du portB
    Memory eeprom;
    Usart usart;
    File byteCode(eeprom, usart);
    Delay delay;
    Motor robot;

    //Initialisation des variables utilisées
    uint8_t opCode;
    uint8_t operand;
    uint8_t nLoops = ZERO_ITERATION;
    uint16_t address = ADDRESS_BEGIN;
    uint16_t oldAddress = ADDRESS_ZERO;
    bool poleWasSet = false;

    //Sauvegarde des donnees en memoire
    byteCode.save();

    //Recuperer l'adresse de l'instruction apres DBT
    while (opCode != DBT)
        eeprom.read(opCode, address++);

    //Execution des commandes
    while (opCode != FIN)
    {
        eeprom.read(opCode, ++address);
        eeprom.read(operand, ++address);
        switch (opCode)
        {
        case (ATT):
            DEBUG_PRINT("Attente de %u ms\n", operand * TIME_MULTIPLIER);
            delay.inMS(operand * CYCLE_SETTER);
            break;

        case (DAL):
            if (operand < MIDDLE_EDGE)
            {
                led.turnOn(Led::GREEN);
                DEBUG_PRINT("Allumer la DEL verte%c\n", ' ');
            }
            else
            {
                led.turnOn(Led::RED);
                DEBUG_PRINT("Allumer la DEL rouge%c\n", ' ');
            }
            break;

        case (DET):
            led.turnOff();
            DEBUG_PRINT("Eteindre la DEL%c\n", ' ');
            break;

        case (MON):
            matrix4x4.write(operand);
            DEBUG_PRINT("Allumer la matrice 4x4%c\n", ' ');
            break;

        case (MOF):
            matrix4x4.clear();
            DEBUG_PRINT("Eteindre la matrice 4x4%c\n", ' ');
            break;

        case (MAR1):
            robot.stopMotor();
            DEBUG_PRINT("Eteindre les moteurs%c\n", ' ');
            break;

        case (MAR2):
            robot.stopMotor();
            DEBUG_PRINT("Eteindre les moteurs%c\n", ' ');
            break;

        case (MAV):
            robot.moveForward(operand);
            DEBUG_PRINT("Avancer a une vitesse de %u %% \n", 
                        (uint8_t)(operand * POURCENTAGE_MULTIPLIER));
            if (!poleWasSet)
            {
                compass.turnOn();
                poleWasSet = true;
            }
            break;

        case (MRE):
            robot.moveBackward(operand);
            DEBUG_PRINT("Reculer a une vitesse de %u %% \n", 
                        (uint8_t)(operand * POURCENTAGE_MULTIPLIER));
            if (!poleWasSet)
            {
                compass.turnOn();
                poleWasSet = true;
            }
            break;

        case (TRD):
            robot.turn(Motor::RIGHT);
            DEBUG_PRINT("Tourner a droite%c\n", ' ');
            delay.inMS(TURN_FINISHED); //2 secondes pour une virage complet
            compass.rightShift();
            break;

        case (TRG):
            robot.turn(Motor::LEFT);
            DEBUG_PRINT("Tourner a gauche%c\n", ' ');
            delay.inMS(TURN_FINISHED); //2 secondes pour une virage complet
            compass.leftShift();
            break;

        case (DBC):
            oldAddress = address;
            nLoops = operand;
            DEBUG_PRINT("Iterations restantes: %u\n", operand + 1);
            break;

        case (FBC):
            if (nLoops > ZERO_ITERATION)
            {
                address = oldAddress;
                DEBUG_PRINT("Iterations restantes: %u\n", nLoops);;
                nLoops--;
            }
            DEBUG_PRINT("Fin de la boucle%c\n", ' ');
        }
    }
    compass.turnOff(); //Eteint la boussole
    eeprom.clear();    //Vidage de la memoire pour les utilisations futures
    DEBUG_PRINT("Choregraphie terminee%c\n", ' ');
}
