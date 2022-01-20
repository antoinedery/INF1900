/***********************************************************************
* Fichier:	pb2.cpp 					   
* Auteurs:	Antoine Déry (2049399) et Stéphanie Ly (1925212)
* Équipe: 	03					   
* Section: 	01					   
* Date: 	2020/09/24
* Description:	Jeu de réflexe avec l'utilisation d'une minuterie			   
***********************************************************************/

#define F_CPU 8000000 //8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//Variables globales
const uint8_t DELAI_ANTIREBOND = 30; //Nombre de millisecondes pour l'antirebond
volatile uint8_t minuterieExpiree = 0;
volatile uint8_t boutonPoussoir = 0;

//Enum pour la couleur des LEDs
enum LED
{
	LED_OFF = 0x00,	  //LED VERTE (A0) ET LED ROUGE (A1) ÉTEINTES
	LED_GREEN = 0x01, //LED VERTE (A0) ALLUMÉE, LED ROUGE (A1) ÉTEINTE
	LED_RED = 0x02	  //LED ROUGE (A1) ALLUMÉE, LED VERTE (A1) ÉTEINTE
};

void initialisation()
{
	// cli est une routine qui bloque toutes les interruptions.
	// Il serait bien mauvais d'être interrompu alors que
	// le microcontroleur n'est pas prêt...
	cli();

	// configurer et choisir les ports pour les entrées
	// et les sorties. DDRx... Initialisez bien vos variables
	DDRD &= ~(1 << DDD2);			   // D2 en input
	DDRA |= (1 << DDA1) | (1 << DDA0); // A0 et A1 en output

	// cette procédure ajuste le registre EIMSK
	// de l’ATmega324PA pour permettre les interruptions externes
	EIMSK |= (1 << INT0);

	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA |= (1 << ISC01); //falling edge du bouton D2 (INT0)

	// sei permet de recevoir à nouveau des interruptions.
	sei();
}

ISR(TIMER1_COMPA_vect) //Timer/Counter1 Compare Match A
{
	minuterieExpiree = 1; //MinuterieExpiree = 1 lorsque TCNT1 = 7812.
}

ISR(INT0_vect)
{
	boutonPoussoir = 1;
	_delay_ms(DELAI_ANTIREBOND);

	EIFR |= (1 << INTF1);
}

void partirMinuterie(uint16_t duree)
{
	minuterieExpiree = 0;
	// mode CTC du timer 1 avec horloge divisée par 1024
	// interruption après la durée spécifiée //ctc = clear timer on compare
	TCNT1 = 0;							//Starts at 0 (0 to 65 535 --> (2^16)-1))
	OCR1A = duree;						//Value that is continuously compared with the counter value TCNT1 (page 132)
	TCCR1A = (1 << COM1A1);				//Clear OCR1A on compare match (page 128)
	TCCR1B = (1 << CS12) | (1 << CS10); //Set clock to CLK/1024 (from prescalar) (page 131)
	TCCR1C = 0;
	TIMSK1 = (1 << OCIE1A); //Enable interrupt on OCR1A. When timer == OCR1A --> interrupt (page 134)
}

int main()
{

	initialisation();
	_delay_ms(10000); //Attente de 10 secondes
	PORTA = LED_RED;  //Allumer LED rouge

	_delay_ms(100);	 //Attente de 1/10 seconde
	PORTA = LED_OFF; //Éteindre LED

	partirMinuterie(7812); //(8'000'000 inst/sec)/1'024 = 7'812 inst/sec --> Attente d'une seconde

	do
	{
		//Attente
	} while (minuterieExpiree == 0 && boutonPoussoir == 0);

	cli(); // Une interruption s'est produite. Arrêter toute forme d'interruption. Une seule réponse suffit.

	// Verifier la réponse
	if (boutonPoussoir == 1)
	{
		PORTA = LED_GREEN;
	}
	else //if minuterieExpiree == 1
	{
		PORTA = LED_RED;
	}
}
