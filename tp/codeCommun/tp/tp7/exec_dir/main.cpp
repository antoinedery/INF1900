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

//Déclaration des variables globales
volatile bool isExpired = 0;
const uint16_t ONE_SEC_10MS = 100;
const uint16_t ONE_SEC_1MS = 1000;

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
	portA.setOut((1<<PORTA0) | (1<<PORTA1));
	portD.setIn(1 << PORTD2);

	//Déclaration pour test de la classe Bouton
	Button button(portD);

	//Déclaration pour test de la classe Timer
	Timer monTimer;

	//Déclaration pour test de la classe Led
	Led led(portA, PORTA0, PORTA1);

	while (true)
	{

		if (button.isClicked(PIND2)) //Test de la classe Bouton
		{
			//Test de la classe motorPWM
			motorPWM pwm1(100, 100, FORWARD, FORWARD);
			_delay_ms(ONE_SEC_1MS);

			pwm1.invertDirectionLeft();
			_delay_ms(ONE_SEC_1MS);

			pwm1.invertDirectionRight();
			_delay_ms(ONE_SEC_1MS);

			pwm1.setPercent(0, 100);
			_delay_ms(ONE_SEC_1MS);

			pwm1.stopMotor();

			//Test de la classe timer - Allumer la DEL en rouge après 1 seconde
			sei();
			monTimer.startTimer(ONE_SEC_1MS);

			while (!isExpired)
			{
			};
			cli();
			if (isExpired)
			{
				//Test de la classe Led et de la classe Delay
				led.timelapse(Led::RED, ONE_SEC_10MS);
				led.timelapse(Led::GREEN, ONE_SEC_10MS);
				led.timelapse(Led::AMBER, ONE_SEC_10MS);
				led.turnOff();
			}
			sei();
			monTimer.stopTimer();
			isExpired = false;
		}
	}
}
