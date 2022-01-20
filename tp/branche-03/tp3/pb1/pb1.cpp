/***********************************************************************
* Fichier:	    pb2.cpp 					   
* Auteurs:	    Antoine Déry (2049399) et Stéphanie Ly (1925212)
* Équipe:       03					   
* Section:      01					   
* Date:         2020/09/18
* Description:  Contrôle des moteurs du robot avec un signal PWM
***********************************************************************/

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

//Variables globales
int POURCENTAGE_PERIODE = 100;

//ENUMS
enum stateMachine
{
    STATE0,
    STATE1,
    STATE2
};

enum LED
{
	LED_OFF = 0x00,		//LED VERTE (A0) ET LED ROUGE (A1) ÉTEINTES
	LED_GREEN = 0x01,	//LED VERTE (A0) ALLUMÉE, LED ROUGE (A1) ÉTEINTE
	LED_RED = 0x02		//LED ROUGE (A1) ALLUMÉE, LED VERTE (A1) ÉTEINTE
};

/***************************************************************************
* Fonction:     configure()
* Description:  Elle initialise les ports de sorties et d'entrees
* Paramètres:   Aucun                       
* Retour:       Aucun         
****************************************************************************/
void configure()
{
    DDRD &= ~((1 << DDD2) | (1 << DDD3)); //(Outputs): Bit clear les bits 3 et 2
    DDRA |= (1 << DDA1) | (1 << DDA0);    //(Inputs): Set clear les bits 1 et 0
}

/***************************************************************************
* Fonction:      isPressed()
* Description:   Elle vérifie si le bouton a été appuyé (avec antirebond)
* Paramètres:    Aucun       
* Retour:        (bool): Si le bouton a été appuyé (avec antirebond)
****************************************************************************/
bool isPressed()
{
    if ((PIND & (1 << PIND2)))
    {
        _delay_ms(10);
        if ((PIND & (1 << PIND2)))
        {
            return true;
        }
    }
    return false;
}

/***************************************************************************
* Fonction:      delay()
* Description:   Elle permet de contourner la fonction _delay_ms() en lui
                 passant en paramètres autre chose qu'un entier constant 
* Paramètres:    (pourcentage) : pourcentage de l'onde a ou (b-a) par rapport à la période       
* Retour:        Aucun
****************************************************************************/
void delay(int pourcentage)
{
    for (int i = 0; i < pourcentage; i++)
        _delay_us(10); //10 us = 0,01 ms --> 1% de 1 ms (periode de 1 ms car fréquence de 1 kHz)
                       //Donc si le pourcentage = 90 alors la boucle s'effectue 90 fois*0,01ms = 90 ms = 90% d'une période
}

int main()
{
    configure();
    stateMachine currentState = STATE0;

    while (1)
    {
        switch (currentState)
        {
        case STATE0: //Bouton relâché - État initial
            PORTA = LED_OFF;
            if (isPressed())
            {
                currentState = STATE1;
            }
            break;

        case STATE1: //Bouton appuyé
            PORTA = LED_GREEN;
            if (!isPressed())
            {
                currentState = STATE2;
            }
            break;

        case STATE2:     // Bouton "en relâchement" --> atténuation de la LED en 3 secondes
            int pourcentageA = POURCENTAGE_PERIODE; //valeur initiale de a est de 100 car LED à 100% (donc a dure toute la période)
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < 30; j++) //car 100*30 = 3000 ms = 3 secondes
                {                            //Une boucle J dure 1 ms donc en la faisant 30 fois = 30 ms
                    PORTA = LED_GREEN;       //Après on fait 100 boucles I donc 3 secondes
                    delay(pourcentageA);
                    PORTA = LED_OFF;
                    delay(POURCENTAGE_PERIODE - pourcentageA); //Pourcentage de B est 100-a
                }
                pourcentageA--; 
            }
            currentState = STATE0;
            break;
        }
    }
}
