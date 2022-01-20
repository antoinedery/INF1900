/***********************************************************************
* Fichier:	pb1.cpp 					   
* Auteurs:	Antoine Déry (2049399) et Stéphanie Ly (1925212)
* Équipe: 	03					   
* Section: 	01					   
* Date: 	2020/09/23
* Description:	Contrôle d'une DEL par interruption				   
***********************************************************************/

#define F_CPU 8000000 //8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//Enums
enum Etats
{
	INIT,
	VERT1,
	ROUGE1,
	VERT2,
	ROUGE2
};

enum LED
{
	LED_OFF = 0x00,	  //LED VERTE (A0) ET LED ROUGE (A1) ÉTEINTES
	LED_GREEN = 0x01, //LED VERTE (A0) ALLUMÉE, LED ROUGE (A1) ÉTEINTE
	LED_RED = 0x02	  //LED ROUGE (A1) ALLUMÉE, LED VERTE (A1) ÉTEINTE
};

//Variables globales
const uint8_t DELAI_ANTIREBOND = 30; //Nombre de millisecondes pour l'antirebond

//Variable volatile
volatile Etats etatPresent = INIT;


void initialisation()
{
	// cli est une routine qui bloque toutes les interruptions.
	// Il serait bien mauvais d'être interrompu alors que
	// le microcontroleur n'est pas prêt...
	cli();

	// configurer et choisir les ports pour les entrées
	// et les sorties. DDRx... Initialisez bien vos variables
	DDRD &= ~((1 << DDD2) | (1 << DDD3)); // D2 et D3 en input
	DDRA |= (1 << DDA1) | (1 << DDA0);	  // A0 et A1 en output

	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0) | (1 << INT1);

	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA |= (1 << ISC00) | (1 << ISC10); //falling edge

	// sei permet de recevoir à nouveau des interruptions.
	sei();
}

ISR(INT0_vect)
{
	// laisser un delai avant de confirmer la réponse du
	// bouton-poussoir: environ 30 ms (anti-rebond)
	_delay_ms(30);
	switch (etatPresent)
	{
	//changements d'états tels que ceux de la semaine précédente
	case (INIT):
		if (!(PIND & (1 << PIND2)))
			etatPresent = ROUGE1;
		break;
	
	case (ROUGE1):
		break;

	case (VERT1):
		if (!(PIND & (1 << PIND2)))
			etatPresent = ROUGE1;
		break;

	case (VERT2):
		if (!(PIND & (1 << PIND2)))
			etatPresent = ROUGE2;
		break;
	
	case (ROUGE2):
		break;

	}

	// Voir la note plus bas pour comprendre cette instruction et son rôle
	EIFR |= (1 << INTF0);
}

ISR(INT1_vect)
{
	// laisser un delai avant de confirmer la réponse du
	// bouton-poussoir: environ 30 ms (anti-rebond)
	_delay_ms(DELAI_ANTIREBOND);
	switch (etatPresent)
	{
	//changements d'états tels que ceux de la semaine précédente
	case (INIT):
		if (!(PIND & (1 << PIND3)))
			etatPresent = VERT1;
		break;

	case (VERT1):
		break;

	case (ROUGE1):
		if (!(PIND & (1 << PIND3)))
			etatPresent = VERT2;
		break;

	case (VERT2):
		if (!(PIND & (1 << PIND3)))
			etatPresent = INIT;
		break;

	case (ROUGE2):
		if (!(PIND & (1 << PIND3)))
			etatPresent = VERT2;
		break;
	}

	// Voir la note plus bas pour comprendre cette instruction et son rôle
	EIFR |= (1 << INTF1);
}


int main()
{
	initialisation();

	while (1)
	{
		switch (etatPresent)
		{

		case (INIT):
			PORTA = LED_OFF;
			break;

		case (VERT1):
			PORTA = LED_GREEN;
			break;

		case (ROUGE1):
			PORTA = LED_RED;
			break;

		case (VERT2):
			PORTA = LED_GREEN;
			break;

		case (ROUGE2):
			PORTA = LED_RED;
			break;
		}
	}
}
