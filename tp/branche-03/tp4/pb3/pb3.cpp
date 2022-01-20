/***********************************************************************
* Fichier:	pb3.cpp 					   
* Auteurs:	Antoine Déry (2049399) et Stéphanie Ly (1925212)
* Équipe: 	03					   
* Section: 	01					   
* Date: 	2020/09/25
* Description:	Implémentation d'un PWM avec l'utilisation de timers			   
***********************************************************************/

#define F_CPU 8000000 //8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

//Variables constantes
const uint8_t NOMBRE_RAPPORTS = 5;	 //5 rapports a/b (PWM = 0%, 25%, 50%, 75%, 100%)
const uint8_t INCREMENTATION_A = 25; //0%, 25%, 50%, 75%, 100%
const int DELAIS_2000MS = 2000;		 //Délais de 2 secondes entre

/***********************************************************************
* Fonction:		ajustementPWM	 					   
* Description:	Permet d'ajuster le signal PWM du timer1 selon un cycle de travail variable	                   
* Paramètres:	dutyCycle : le cycle de travail (onde A) du signal PWM	   
* Retour: 		Aucun				 				   
***********************************************************************/
void ajustementPWM(uint8_t dutyCycle)
{
	//Valeur de comparaison avec le timer pour OC1A 
	OCR1A = 255 * (1 - (dutyCycle / 100.0f))-1;				//100.0f -> float
	//Valeur de comparaison avec le timer pour OC1B
	OCR1B = 255 * (1 - (dutyCycle / 100.0f))-1;	

	TCCR1A = (1 << COM1A1) | (1 << COM1A0) | (1 << COM1B1) | (1 << COM1B0) | (1 << WGM10);	//PWM, Phase Correct, 8 bits + Set OC1A/OC1B à 1 en upcounting, clear en downcounting.
	TCCR1B = (1 << CS11);	//CLK/8 (from prescaler)
}

int main()
{
	DDRD |= (1 << DDD3) | (1 << DDD4) | (1 << DDD5) | (1 << DDD6); //D3, D4 (OC1B), D5 (OC1A) et D6 en output (D3 et D6 servent à la direction de la roue)
	PORTD = ((1 << PORTD3) | (1 << PORTD6));					   //Direction = 1 donc vers l'avant

	while (1)
	{
		int dutyCycle = 0;
		for (int i = 0; i < NOMBRE_RAPPORTS; i++)
		{
			ajustementPWM(dutyCycle);
			_delay_ms(DELAIS_2000MS);
			dutyCycle += INCREMENTATION_A;
		}
	}
}