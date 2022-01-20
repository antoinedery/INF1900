/***************************************************************************
* Fichier:		door.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 novembre 2020			   
* Description:  Implémentation de la classe door 
***************************************************************************/

#include "door.h"

const uint8_t ONE_BYTE = 8;
const uint8_t FOUR_BYTES = 32;

const uint8_t PORT_POSITION_1 = 1;
const uint8_t PORT_POSITION_2 = 2;
const uint8_t PORT_POSITION_3 = 3;
const uint8_t PORT_POSITION_4 = 4;
const uint8_t PORT_POSITION_5 = 5;

const uint32_t DELAY = 125;
const uint32_t DISPLAY_BEGIN = 0;

const uint32_t CLOSE_MASK = 0X80000000;
const uint32_t OPEN_MASK = 0X01000000;
const uint32_t SEND_MASK = 1;

/***************************************************************************
* Fonction:     Door::Door
* Description:  Constructeur par paramètres de la porte
* Paramètres:   (port) : le port où est branché les portes          
* Retour:       Aucun       
****************************************************************************/
Door::Door(const Port &port)
    : port_(port),
      previousDisplay_(DISPLAY_BEGIN)
{
    port_.setOut((1 << PORT_POSITION_1) |
                 (1 << PORT_POSITION_2) |
                 (1 << PORT_POSITION_3) |
                 (1 << PORT_POSITION_4) |
                 (1 << PORT_POSITION_5));

    port_.writeOne(PORT_POSITION_4);
    port_.writeZero(PORT_POSITION_5);
}

/***************************************************************************
* Fonction:     Door::pulseClock
* Description:  Permet d'envoyer des signaux sur l'entrée horloge et latch
* Paramètres:   Aucun         
* Retour:       Aucun       
****************************************************************************/
void Door::pulseClock()
{
    port_.writeOne(PORT_POSITION_1);
    port_.writeZero(PORT_POSITION_2); //Low latch

    port_.writeZero(PORT_POSITION_1);
    port_.writeOne(PORT_POSITION_2); //High latch
}

/***************************************************************************
* Fonction:     Door::send
* Description:  Permet d'allumer ou éteindre les DELs sur les portes
* Paramètres:   (printValue) : la valeur a envoyée sur les portes         
* Retour:       Aucun       
****************************************************************************/
void Door::send(const uint32_t &printValue) 
{
    for (uint8_t i = 0; i < FOUR_BYTES; i++)
    {
        if (printValue & (SEND_MASK << i))
            port_.writeOne(PORT_POSITION_3);
        else
            port_.writeZero(PORT_POSITION_3);
        pulseClock(); //read PORTA3 et write del
    }
    _delay_ms(DELAY);
}

/***************************************************************************
* Fonction:     Door::close
* Description:  Permet de fermer une porte en allumant ses DELs
* Paramètres:   (tag) : le "tag" de la porte        
* Retour:       Aucun       
****************************************************************************/
void Door::close(const Tag &tag)
{
    uint32_t shift = returnShift(tag);
    for (uint8_t i = 0; i < ONE_BYTE; i++)
        send(previousDisplay_ = (previousDisplay_ | ((CLOSE_MASK >> i) >> shift)));
}

/***************************************************************************
* Fonction:     Door::open
* Description:  Permet d'ouvrir une porte en éteignant ses DELs
* Paramètres:   (tag) : le "tag" de la porte        
* Retour:       Aucun       
****************************************************************************/
void Door::open(const Tag &tag)
{
    uint32_t shift = returnShift(tag);
    for (uint8_t i = 0; i < ONE_BYTE; i++)
        send(previousDisplay_ = (previousDisplay_ & ~((OPEN_MASK << i) >> shift)));
}

/***************************************************************************
* Fonction:     Door::returnShift
* Description:  Permet de retourner le nombre de bits décalés selon la porte sélectionnée.
* Paramètres:   (tag) : le "tag" de la porte        
* Retour:       Aucun       
****************************************************************************/
uint32_t Door::returnShift(const Tag &tag){
    uint32_t shift = 0;
    switch(tag){
        case A:
            break;
        case B:
            shift = 8;
            break;
        case C:
            shift = 16;
            break;
        case D:
            shift = 24;
    }
    return shift;
}