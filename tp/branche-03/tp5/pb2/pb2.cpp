/***********************************************************************
* Fichier:		pb2.cpp	 					   
* Auteurs:		Antoine Déry (2049399) et Stéphanie Ly (1925212)
* TP : 			05	   
* Équipe: 		03					   
* Section: 		01					   
* Date:			2020/09/30						   
* Description:	Écriture d'une chaine de caractères dans le terminal de
				SimulIDE avec une transmission UART
***********************************************************************/

#define F_CPU 8000000 //8 MHz
#include <avr/io.h>


/***********************************************************************
* Fonction:		initialisationUART()	 					   
* Description:	Permet d'initialiser les différents registres de UART0		                   
* Paramètres:	Aucun	 				   
* Retour:		Aucun				 				   
***********************************************************************/
void initialisationUART()
{
	UBRR0L = 0xCF;							 //Baud Rate à 2400 bps. OxCF = 207 (page 192)
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);	 //Activer la réception (Rx) et ;a transmission (Tx)
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Format de 8 bits (UCSZ), 1 stop bit (USBS = 0), sans parité (UMP)
}

/***********************************************************************
* Fonction:		tranmissionUART()	 					   
* Description:	Permet d'envoyer des données de l'UART0 vers le terminal de SimulIDE                   
* Paramètres:	uint8_t donnee : l'octet à envoyer vers le simulateur	 			  
* Retour:		Aucun				 				   
***********************************************************************/
void transmissionUART(uint8_t donnee)
{
	while (!( UCSR0A & (1<<UDRE0)));	//Attendre que le buffer de transmission soit vide
	UDR0 = donnee;
}

int main()
{
	initialisationUART();

	unsigned char phrase[] = "Le simulateur SimulIDE en INF1900\n"; //Chaine de caractères à transmettre dans le terminal de SimulIDE
	for (uint8_t i = 0; i < 100; i++)								//La phrase est transmise 100 fois
	{
		for (uint8_t j = 0; j < sizeof(phrase) - 1; j++) //sizeof(phrase)-1 car on ne veut pas le caractère de terminaison
			transmissionUART(phrase[j]);
	}
}