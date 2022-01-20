/***********************************************************************
* Fichier:		pb3.cpp	 					   
* Auteurs:		Antoine Déry (2049399) et Stéphanie Ly (1925212)
* TP : 			05	   
* Équipe: 		03					   
* Section: 		01					   
* Date:			2020/10/01						   
* Description:	Écriture et lecture d'une chaine de caractères dans la 
				mémoire EEPROM et affichage dans le terminal de
				SimulIDE avec une transmission UART				   
***********************************************************************/

#define F_CPU 8000000 //8 MHz
#include <avr/io.h>
#include <avr/eeprom.h>


//Initialisation des variables
uint8_t CHAINE_ECRITURE[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*\n";
uint8_t CHAINE_LECTURE[sizeof(CHAINE_ECRITURE)];
uint16_t ADRESSE_DEBUT = 0x00;
uint16_t VALEUR_FINALE = 0xFF;

/***********************************************************************
* Fonction:		initialisationUART()	 					   
* Description:	Permet d'initialiser les différents registres de UART0		                   
* Paramètres:	Aucun	 				   
* Retour:		Aucun				 				   
***********************************************************************/
void initialisationUART()
{
	UBRR0L = 0xCF;							 //Baud Rate à 2400 bps. OxCF = 207 (page 192)
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);	 //Activer la réception (Rx) et la transmission (Tx)
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //Format de 8 bits (UCSZ), 1 stop bit (USBS = 0), sans parité (UMP = 0)
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

	eeprom_write_block((const void *)CHAINE_ECRITURE, (void *)ADRESSE_DEBUT, sizeof(CHAINE_ECRITURE)); //Écriture sur le EEPROM
	uint16_t adresse = ADRESSE_DEBUT;

	while (true)
	{
		uint8_t valeurLue = eeprom_read_byte((const uint8_t*)adresse);
		if (valeurLue == VALEUR_FINALE)
		{
			break;
		}
		transmissionUART(valeurLue);
		adresse++;
	}
}