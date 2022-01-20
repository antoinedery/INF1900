/***************************************************************************
 * Fichier:     usart.cpp			 					   
 * Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
 * Équipe:		03-18					   
 * Section:		01					   
 * Date:        23 octobre 2020				   
 * Description: Implémentation de la classe USART			   
***************************************************************************/

#include "usart.h"

/***************************************************************************
* Méthode:      USART::USART
* Description:  Constructeur par défaut permettant d'initialiser les différents registres de USART0
* Paramètres:   Aucun                
* Retour:       Aucun         
****************************************************************************/
Usart::Usart()
{
    UBRR0L = 0xCF;                           //Baud Rate à 2400 bps. OxCF = 207 (page 192)
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0);   //Activer la réception (Rx) et la transmission (Tx)
    UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Format de 8 bits (UCSZ), 1 stop bit (USBS = 0), sans parité (UMP = 0)
}

/***************************************************************************
* Méthode:      Usart::transmitChar
* Description:  Méthode qui envoie les données de l'UART0 vers le terminal SimulIDE
* Paramètres:   (data) : L'octet à transmettre.              
* Retour:       Aucun    
****************************************************************************/
void Usart::transmitChar(const uint8_t &data)
{
    while (!(UCSR0A & (1 << UDRE0)))
    {
    } //Attendre que le buffer de transmission soit vide
    UDR0 = data;
}

/***************************************************************************
* Méthode:      Usart::transmitString
* Description:  Méthode qui une chaine de caractères vers le terminal de SimulIDE
* Paramètres:   (src) : Pointeur vers la chaine de caractères             
* Retour:       Aucun    
****************************************************************************/
void Usart::transmitString(const char *src)
{
    uint16_t i = 0;
    while (src[i] != '\0')
    {
        transmitChar(src[i++]);
    }
}

/***************************************************************************
* Méthode:      Usart::receive
* Description:  Méthode qui recoit les octets de l'UART0
* Paramètres:   Aucun              
* Retour:       L'octet reçu    
****************************************************************************/
uint8_t Usart::receive()
{

    while (!(UCSR0A & (1 << RXC0)))
    {
    } //wait for data to be received

    return UDR0;
}