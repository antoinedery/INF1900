/***********************************************************************
* Fichier:		pb1.cpp	 					   
* Auteurs:		Antoine Déry (2049399) et Stéphanie Ly (1925212)
* TP : 			05	   
* Équipe: 		03					   
* Section: 		01					   
* Date:			2020/09/30						   
* Description:	Écriture et lecture d'une chaine de caractères dans la 
				mémoire EEPROM et comparaison entre les deux 				   
***********************************************************************/

#define F_CPU 8000000 //8 MHz
#include <avr/io.h>
#include <avr/eeprom.h>


//Initialisation des variables
unsigned char CHAINE_ECRITURE[] = "*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*";
unsigned char CHAINE_LECTURE[sizeof(CHAINE_ECRITURE)];
uint16_t ADRESSE_DEBUT = 0x00;
uint8_t LED_VERT = 0x01;
uint8_t LED_ROUGE = 0x02;

int main()
{
	DDRA |= (1 << DDA0) | (1 << DDA1); //A0 et A1 sont en sortie (LEDs)

	eeprom_write_block((const void *)CHAINE_ECRITURE, (void *)ADRESSE_DEBUT, sizeof(CHAINE_ECRITURE)); //Écriture sur le EEPROM
	eeprom_read_block((void *)CHAINE_LECTURE, (const void *)ADRESSE_DEBUT, sizeof(CHAINE_ECRITURE)); //Lecture du EEPROM

	//Comparaison des chaines de caractères CHAINE_LECTURE et CHAINE_ECRITURE
	for (unsigned int i = 0; i < sizeof(CHAINE_ECRITURE); i++)
	{
		if (CHAINE_ECRITURE[i] != CHAINE_LECTURE[i])
		{
			PORTA = LED_ROUGE; //Si différentes
			break;
		}
		else
			PORTA = LED_VERT; //Si identiques
	}
}