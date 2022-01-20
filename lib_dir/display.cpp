/***************************************************************************
* Fichier:		display.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			29 novembre 2020			   
* Description:  Implémentation de la classe Display, qui affiche des messages 
                dans le terminal de simulIDE
***************************************************************************/

#include "display.h"
#include "time.h"
#include "state.h"
#include "cast.h"

const char MAIN_MENU[] = "1-Definir l'heure de depart\n2-Definir l'etat d'un dispositif\n3-Afficher les actions programmees\n4-Programmer une action\n5-Supprimer une action programmee\n6-Demarrer la simulation\n\nChoisissez une option : ";
const char SET_CLOCK[] = "\n\nEntrez l'heure et les minutes de depart de la simulation HHMM : ";
const char SET_STATE[] = "\n\nEntrez l'identifiant d'un dispositif suivi de sa valeur de configuration (A|B|C|D)(0|1) ou (E|F)(000-180) : ";
const char PROGRAM_ACTION[] = "\n\nEntrez une action a programmer. HHMM (A|B|C|D)(0|1) ou (E|F)(000-180) ";
const char REMOVE_ACTION[] = "\n\nEntrez le numero d'une action a supprimer. ";
const char SIM_TIME_WAS_SET[] = "\n\nL'heure de la simulation a deja ete initialise.\n\n";
const char SIM_TIME_WAS_NOT_SET[] = "\n\nL'heure de la simulation n'a pas ete initialise.\n\n";
const char SIMULATION_END[] = "\n\nFin de la simulation.\n\n";
const char SIMULATION_STOP[] = "\n\nSimulation interrompue.\n\n";
const char ERROR[] = "\n\nSaisie invalide\n\n";

const uint8_t FIRST_VALUE_MENU = 1;
const uint8_t LAST_VALUE_MENU = 6;
const uint8_t TWO_DIGITS = 2;
const uint8_t MAX_VALUE_DIGIT = 9;
const uint8_t TEN_MULTIPLIER = 10;
const uint8_t INDEX_ZERO = 0;
const uint8_t INDEX_ONE = 1;
const uint8_t NULL_VALUE = 0;
const uint16_t END_SIM_DELAY = 2000;

const uint8_t READ_PORT_A0 = 0;
const uint8_t RIGHT_BIT_SHIFT = 2;
const uint16_t TEMP_TIME = -1;

volatile bool Display::simulationIsStopped_ = false;

/***************************************************************************
* Fonction:     Display::Display
* Description:  Constructeur par défaut de la classe Display
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
Display::Display()
    : door_(Port::A),
      keyPad_(Port::C),
      lcd_(&DDRB, &PORTB),
      can_(),
      sonar_(),
      simHour_(NULL_VALUE),
      simMinute_(NULL_VALUE),
      simTimeWasSet_(false)
{
}

/***************************************************************************
* Fonction:     Display::startRoutine
* Description:  Affiche en boucle le menu de configuration lorsque la simulation
                prend fin
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::startRoutine()
{
    for (;;)
    {
        Display::configurationMode();
        Display::simulationMode();
    }
}

/***************************************************************************
* Fonction:     Display::configurationMode
* Description:  Affiche le menu principal (options 1 à 6)
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::configurationMode()
{
    uint8_t input = NULL_VALUE;

    lcd_.write("CONFIGURATION", LCM_FW_FIRST_CH, true);

    while (input != 6)
    {
        usart_.transmitString(MAIN_MENU);

        input = keyPad_.scan();

        usart_.transmitChar(Cast::intToChar(input));

        if (input >= FIRST_VALUE_MENU && input <= LAST_VALUE_MENU)
        {
            switch (input)
            {
            case 1:
                Display::setSimulationTime();
                break;
            case 2:
                Display::setStateDevice();
                break;
            case 3:
                Display::printActions();
                break;
            case 4:
                Display::programAction();
                break;
            case 5:
                Display::removeAction();
            }
        }
        else
            usart_.transmitString(ERROR);
    }
}

/***************************************************************************
* Fonction:     Display::setSimulationTime
* Description:  Configure l'horloge selon le temps entré par l'utilisateur sur le clavier
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::setSimulationTime()
{
    usart_.transmitString(SET_CLOCK);

    Time time;

    if (time.trySetValue(usart_, keyPad_))
    {
        simHour_ = time.getHour();
        simMinute_ = time.getMinute();
        clock_.setClock(simHour_, simMinute_);

        simTimeWasSet_ = true;

        usart_.transmitString("\n\n");
    }
}

/***************************************************************************
* Fonction:     Display::setStateDevice
* Description:  Configure un dispositif (porte ou servo) a une valeur entrée
                par l'utilisateur sur le clavier
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::setStateDevice()
{
    usart_.transmitString(SET_STATE);

    State state;

    if (state.trySetValue(usart_, keyPad_))
    {
        state.execute(door_, servo_);
    }
}

/***************************************************************************
* Fonction:     Display::printActions
* Description:  Permet d'afficher les actions dans le terminal de SimulIDE
                et affiche un message d'erreur si aucun temps n'a été saisie
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::printActions()
{
    if (!simTimeWasSet_)
    {
        usart_.transmitString(SIM_TIME_WAS_NOT_SET);
        return;
    }

    actionList_.orderActions(simHour_, simMinute_);
    actionList_.printList(usart_);
}

/***************************************************************************
* Fonction:     Display::programAction
* Description:  Programme une action et l'ajoute dans la liste d'actions
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::programAction()
{
    usart_.transmitString(PROGRAM_ACTION);

    Time time;
    State state;

    if (!time.trySetValue(usart_, keyPad_)) //trySetValue
        return;

    usart_.transmitChar(' ');

    if (!state.trySetValue(usart_, keyPad_))
        return;

    Action *action = (Action *)malloc(sizeof(Action));
    action->setValues(time, state);
    actionList_.insert(action);
}

/***************************************************************************
* Fonction:     Display::removeAction
* Description:  Permet de retirer une action de la liste d'actions
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::removeAction()
{
    usart_.transmitString(REMOVE_ACTION);

    uint8_t actionIndex[TWO_DIGITS];

    for (uint8_t i = 0; i < TWO_DIGITS; i++)
    {
        usart_.transmitChar(Cast::intToChar(actionIndex[i] = keyPad_.scan()));

        if (actionIndex[i] > MAX_VALUE_DIGIT)
        {
            usart_.transmitString(ERROR);
            return;
        }
    }

    actionList_.remove(actionIndex[INDEX_ZERO] * MAX_VALUE_DIGIT + actionIndex[INDEX_ONE]);
    usart_.transmitString("\n\n");
}

/***************************************************************************
* Fonction:     Display::simulationMode
* Description:  Permet de lancer la simulation et d'exécuter les actions
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::simulationMode()
{
    if (!simTimeWasSet_)
    {
        usart_.transmitString(SIM_TIME_WAS_NOT_SET);
        return;
    }

    lcd_.write("SIMULATION", LCM_FW_FIRST_CH, true);

    actionList_.orderActions(simHour_, simMinute_);

    Action *action;
    uint16_t previousActionTime = TEMP_TIME;
    bool equalToPrevious = false;
    bool objectInRange = false;

    while(sonar_.objectInRange()){}

    clock_.startClock();

    while (!actionList_.hitEnd())
    {
        action = actionList_.dequeue();

        if(action->getClockTicks() == previousActionTime) 
        {
            equalToPrevious = true;
        }

        while (equalToPrevious || clock_.getClockCounter() < action->getClockTicks())
        {
            if (sonar_.objectInRange()) // objet détecté
            {
                clock_.stopClock();
                objectInRange = true;
            }
            else
            {
                if(objectInRange)
                {
                    objectInRange = false;
                    if(!simulationIsStopped_)
                        clock_.startClock();
                }
                if(simulationIsStopped_)
                {
                    Display::stopSimulation(action);
                    return;
                }
                else
                {
                    clock_.catchNewVoltage(can_.read(READ_PORT_A0) >> RIGHT_BIT_SHIFT);
                }
            }

            if(equalToPrevious)
            {
                equalToPrevious = false;
                break;
            }
        } 
        action->executeAction(door_, servo_);
        previousActionTime = action->getClockTicks();
    }

    //Precaution
    clock_.startClock();

    //Si aucune action restante, attendre que l'horloge ait roulé 23h59
    while (!clock_.isComplete())
    {
        if (sonar_.objectInRange())
        {
            clock_.stopClock();
            while (sonar_.objectInRange());
            if(!simulationIsStopped_)
                clock_.startClock();
        }

        if (simulationIsStopped_)
        {
            break;
        }

        clock_.catchNewVoltage(can_.read(READ_PORT_A0) >> RIGHT_BIT_SHIFT);
    }

    usart_.transmitString(SIMULATION_END);
    Display::resetSettings();
}

/***************************************************************************
* Fonction:     Display::enableStopSimulation
* Description:  Permet d'autoriser la fin de la simulation en changeant un booléen
* Paramètres:   Aucun
* Retour:       Aucun
****************************************************************************/
void Display::enableStopSimulation()
{
    simulationIsStopped_ = true;
}

/***************************************************************************
* Fonction:     Display::stopSimulation
* Description:  Arrête la simulation lorsque le bouton D3 est appuyé ou que le
                temps a atteint 23h59 et exécute l'action au moment de l'arrêt
* Paramètres:   (action) : l'action a exécuté lorsque la simulation se termine
* Retour:       Aucun
****************************************************************************/
void Display::stopSimulation(Action *action)
{
    usart_.transmitString(SIMULATION_STOP);

    uint16_t lastCounter = clock_.getClockCounter();

    while (action->getClockTicks() <= lastCounter)
    {
        action->executeAction(door_, servo_);

        if (actionList_.hitEnd())
            break;

        action = actionList_.dequeue();
    }

    Display::resetSettings();
}

/***************************************************************************
* Fonction:     Display::resetSettings
* Description:  Reinitialise l'horloge lorsque la simulation prend fin
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
void Display::resetSettings()
{
    _delay_ms(END_SIM_DELAY);
    actionList_.resetQueue();
    clock_.resetClock();
    simTimeWasSet_ = false;
    simulationIsStopped_ = false;
}