/***********************************************************************
* Fichier:		memory.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 novembre 2020				   		   		   				   
* Description:	Implementation de la classe Memory, qui permet de
                transmettre et écrire en mémoire EEPROM
***********************************************************************/

#include "memory.h"

const int MAX_BYTES = 1024;
const uint8_t EMPTY = 0xFF;

/***************************************************************************
* Méthode:      Memory::Memory
* Description:  Constructeur par défaut de la classe Memory
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
Memory::Memory()
{
}

// /***************************************************************************
// * Méthode:      Memory::read
// * Description:  Methode qui permet de lire un block de données de la mémoire EEPROM
// * Paramètres:   (dataOut) : Pointeur vers l'endroit où stocker les données lues
//                 (address) : Adresse où débuter la lecture
// * Retour:       Aucun
// ****************************************************************************/
void Memory::read(uint8_t &data, const uint32_t &address)
{
    data = eeprom_read_byte((uint8_t *)address);
}

/***************************************************************************
* Méthode:      Memory::write
* Description:  Methode qui permet d'écrire un block de données dans la mémoire EEPROM
* Paramètres:   (dataIn)    : Pointeur vers l'endroit où prendre les données à écrire
*               (address)   : Adresse où débuter la lecture
* Retour:       Aucun
****************************************************************************/
void Memory::write(const uint8_t &data, const uint32_t &address)
{
    eeprom_write_byte((uint8_t *)address, data);
}

/***************************************************************************
* Méthode:      Memory::clear
* Description:  Methode qui vide la mémoire EEPROM
* Paramètres:   Aucun
* Retour:       Aucun      
****************************************************************************/
void Memory::clear()
{
    uint8_t data;
    for (int address = 0; address < MAX_BYTES; address++)
    {
        Memory::read(data, address);
        if (data != EMPTY)
            Memory::write(EMPTY, address);
    }
}
