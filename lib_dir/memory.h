/***********************************************************************
* Fichier:		memory.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 novembre 2020				   		   		   				   
* Description:	Déclaration de la classe Memory
***********************************************************************/

#include <avr/eeprom.h>
#include <stdint.h> //uint8_t

class Memory
{
public:
    Memory();

    void read(uint8_t&data, const uint32_t&address);
    void write(const uint8_t&data , const uint32_t&address);

    void clear();
};
