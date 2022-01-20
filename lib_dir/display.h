/***************************************************************************
* Fichier:		display.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			29 novembre 2020			   
* Description:  Déclaration de la classe Display, qui affiche des messages 
                dans le terminal de simulIDE
***************************************************************************/

#ifndef DISPLAY_H
#define DISPLAY_H

#include "usart.h"
#include "keypad.h"
#include "clock.h"
#include "door.h"
#include "servo.h"
#include "lcd_fw.h"
#include "container.h"
#include "can.h"
#include "sonar.h"

class Display 
{

public:
    Display();
    void startRoutine();
    void configurationMode();
    void setSimulationTime();            
    void setStateDevice();  
    void printActions();          
    void programAction();    
    void removeAction();
    void simulationMode();

    static void enableStopSimulation();
    void stopSimulation(Action* action);
    void resetSettings();
    //bool objectIsNear(bool& );

private:
    Door door_;
    Keypad keyPad_;
    LCD lcd_;
    can can_;
    Sonar sonar_;

    uint8_t simHour_;
    uint8_t simMinute_;
    uint8_t simTick_;
    bool simTimeWasSet_;

    Usart usart_;
    Clock clock_;
    Servo servo_;
    Container actionList_;

    static volatile bool simulationIsStopped_;
};

#endif