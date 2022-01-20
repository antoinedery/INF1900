/***************************************************************************
* Fichier:      file.h			 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			03 novembre 2020				   
* Description:  Déclaration de la classe File qui permet de sauvegarder
                les données RS232 dans la mémoire EEPROM
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef FILE_H
#define FILE_H

#include <util/delay.h>
#include "usart.h"
#include "memory.h"

class File
{
public:
    File(Memory&eeprom , Usart &usart);
    void save();

private:
    Memory *eeprom_;
    Usart *usart_;
};

#endif