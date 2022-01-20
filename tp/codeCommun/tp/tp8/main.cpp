/*************************************************************************
* Fichier:		main.cpp
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020	
* Description:	Test de la librairie. Simulation où il faut appuyer sur le
				bouton D2 pour démarrer les moteurs et produire différentes
				variations de PWM. Finalement, ce termine en allumant la DEL
				sur A0/A1 en rouge, vert et ambre.
*************************************************************************/

#define F_CPU 8000000UL //8 MHz
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "motorPWM.h"
#include "timer.h"
#include "port.h"
#include "led.h"
#include "button.h"
#include "debug.h"

//Déclaration des variables globales
volatile bool isExpired = 0;
const uint16_t ONE_SEC_10MS = 100;
const uint16_t ONE_SEC_1MS = 1000;
const uint16_t TWO_SEC_1MS = 2000;

//Pour test de la classe Timer
ISR(TIMER1_COMPA_vect)
{
	isExpired = true;
}

int main()
{
	//Déclarations pour test de la classe Port
	Port portA(Port::A);
	Port portD(Port::D);
	portA.setOut((1<<PORTA0) | (1<<PORTA1)); 	//DEL
	portD.setIn(1 << PORTD2);			//Bouton

	//Déclaration pour test de la classe Bouton
	Button button(portD);

	//Déclaration pour test de la classe Timer
	Timer monTimer;

	//Déclaration pour test de la classe Led
	Led led(portA, PORTA0, PORTA1);
	
	motorPWM pwm1;

	while (true)
	{

		if (button.isClicked(PIND2)) //Test de la classe Bouton
		{
			DEBUG_PRINT(("Bouton D2 a ete clique et relache\n"));
			//Test de la classe motorPWM
			pwm1.start(100, 100, FORWARD, FORWARD);
			DEBUG_PRINT(("PWM : Deux moteurs a 100% vers l'avant\n"));
			_delay_ms(TWO_SEC_1MS);

			pwm1.invertDirectionLeft();
			DEBUG_PRINT(("PWM : Inversion de la direction du moteur gauche\n"));
			_delay_ms(TWO_SEC_1MS);

			pwm1.invertDirectionRight();
			DEBUG_PRINT(("PWM : Inversion de la direction du moteur droit\n"));
			_delay_ms(TWO_SEC_1MS);

			pwm1.setPercent(0, 50);
			DEBUG_PRINT(("PWM : Moteur gauche arrete, moteur droit a 50%\n"));
			_delay_ms(TWO_SEC_1MS);

			pwm1.stopMotor();
			DEBUG_PRINT(("Moteurs arretes\n"));

			//Test de la classe timer - Allumer la DEL en rouge après 1 seconde
			sei();
			monTimer.startTimer(TWO_SEC_1MS);
			DEBUG_PRINT(("Minuterie de 2 secondes\n"));

			while (!isExpired)
			{
			};
			cli();
			if (isExpired)
			{
				//Test de la classe Led et de la classe Delay
				DEBUG_PRINT(("DEL rouge\n"));
				led.timelapse(Led::RED, ONE_SEC_10MS);
				DEBUG_PRINT(("DEL verte\n"));
				led.timelapse(Led::GREEN, ONE_SEC_10MS);
				DEBUG_PRINT(("DEL ambre\n"));
				led.timelapse(Led::AMBER, ONE_SEC_10MS);
				led.turnOff();
				DEBUG_PRINT(("DEL eteinte\n"));
			}
			sei();
			monTimer.stopTimer();
			isExpired = false;
			DEBUG_PRINT(("Appuyer sur le bouton D2 pour relancer la simulation\n"));
		}
	}
}
