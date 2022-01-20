/***************************************************************************
* Fichier:      file.cpp			 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			03 novembre 2020				   
* Description:  Implémentation de la classe File qui permet de sauvegarder
                les données RS232 dans la mémoire EEPROM
***************************************************************************/

#include "file.h"

const uint8_t TRANSMIT_DEBUGGER = 0;
const uint8_t ONE_BYTE_OFFSET = 8;
const uint16_t ADDRESS_BEGIN = 2;
const uint16_t ADDRESS_ZERO = 0;
const uint16_t ADDRESS_ONE = 1;
const uint16_t SIZE_ZERO = 0;
const uint8_t DELAY_5MS = 5;

/***************************************************************************
* Fonction:     File::File
* Description:  Contructeur par défaut de la classe File
* Paramètres:   (eeprom) : référence vers l'objet eeprom
                (usart) : référence vers l'objet usart           
* Retour:       Aucun         
****************************************************************************/
File::File(Memory &eeprom, Usart &usart)
    : eeprom_(&eeprom),
      usart_(&usart)
{
}

/***********************************************************************
* Fonction :    File::save 					   
* Description : Fonction qui sauvegarde un fichier transmitChar en memoire           
* Paramètres :  Aucun
* Retour:       Aucun				 				   
***********************************************************************/
void File::save()
{
    usart_->transmitChar(TRANSMIT_DEBUGGER);

    uint8_t data = usart_->receive();
    uint8_t dataNext = usart_->receive();
    eeprom_->write(data, ADDRESS_ZERO);
    eeprom_->write(dataNext, ADDRESS_ONE);


    uint16_t byteCodeSize = SIZE_ZERO;
    byteCodeSize |= data;
    byteCodeSize <<= ONE_BYTE_OFFSET;
    byteCodeSize |= dataNext;

    uint16_t address = ADDRESS_BEGIN;
    for (; address < byteCodeSize; address++)
    {
        data = usart_->receive();
        _delay_ms(DELAY_5MS);
        eeprom_->write(data, address);
    }
}
