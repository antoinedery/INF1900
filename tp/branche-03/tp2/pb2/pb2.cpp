/***********************************************************************
* Fichier:	pb2.cpp 					   
* Auteurs:	Antoine Déry	2049399
*			Stéphanie Ly	1925212
* Équipe: 	03					   
* Section: 	01					   
* Date: 	2020/09/11
* Description:   Allumer et éteindre deux DEL de couleurs différentes
*                de maniere alternée selon l'ordre d'appui des
*                boutons-poussoirs  				   
***********************************************************************/

/* Table des états
*  État présent |  btnD2  |  btnD3  |    État suivant    |   LED verte   |   LED rouge  |
* ----------------------------------------------------------------------------------------
*     INIT      |    0    |    0    |       INIT         |       0       |      0       |
*     INIT      |    1    |    0    |       ROUGE1       |       0       |      0       |
*     INIT      |    0    |    1    |       VERT1        |       0       |      0       |
*     VERT1     |    0    |    X    |       VERT1        |       1       |      0       |
*     VERT1     |    1    |    X    |       ROUGE1       |       1       |      0       |
*     ROUGE1    |    X    |    0    |       ROUGE1       |       0       |      1       |
*     ROUGE1    |    X    |    1    |       VERT2        |       0       |      1       |
*     VERT2     |    0    |    0    |       VERT2        |       1       |      0       |
*     VERT2     |    1    |    0    |       ROUGE2       |       1       |      0       |
*     VERT2     |    0    |    1    |       INIT         |       1       |      0       |
*     ROUGE2    |    0    |    X    |       ROUGE2       |       0       |      1       |
*     ROUGE2    |    1    |    X    |       VERT2        |       0       |      1       |
*/


#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

//Variables globales
const uint8_t DELAI_ANTIREBOND = 10; //Nombre de millisecondes pour l'antirebond

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
	LED_OFF = 0x00,		//LED VERTE (A0) ET LED ROUGE (A1) ÉTEINTES
	LED_GREEN = 0x01,	//LED VERTE (A0) ALLUMÉE, LED ROUGE (A1) ÉTEINTE
	LED_RED = 0x02		//LED ROUGE (A1) ALLUMÉE, LED VERTE (A1) ÉTEINTE
};

/***************************************************************************
* Fonction:     configure()
* Description:  Elle initialise les ports de sorties et d'entrees
* Paramètres:   Aucun                       
* Retour:       AUcun         
****************************************************************************/
void configure()
{
    DDRD = ~((1 << DDD2)|(1 << DDD3));         //(Outputs): Bit clear les bits 3 et 2  
    DDRA |= (1 << DDA1)|(1 << DDA0);            //(Inputs): Set clear les bits 1 et 0
}

/***************************************************************************
* Fonction:      pressedButton()
* Description:   Elle verifie si on a appuyé (antirebond) et relâché le bouton
* Paramètres:    (PIN): PIND sur laquelle lire la valeur           
* Retour:        (bool): Si le bouton a été appuyé (avec antirebond), elle 
*                retourne true, sinon false
****************************************************************************/
bool pressedButton(uint8_t PIN)
{
    bool isPressed = PIND & (1 << PIN);
    if(isPressed)
    {
        _delay_ms(DELAI_ANTIREBOND);
        if(isPressed)
        {
            bool isPressed = PIND & (1 << PIN);
            if(!isPressed)
                return true;
        }
    }
    return false;
}

int main()
{
    configure();
	Etats etatPresent = INIT;

	while(1)
	{
		switch (etatPresent)
		{

		case (INIT):
			PORTA = LED_OFF;
			if(pressedButton(PIND3))
				etatPresent = VERT1;

            else if (pressedButton(PIND2))
                etatPresent = ROUGE1;
			break;
	
		case (VERT1):
			PORTA = LED_GREEN;
			if (pressedButton(PIND2))
				etatPresent = ROUGE1;
			break;

		case (ROUGE1):
			PORTA = LED_RED;
			if(pressedButton(PIND3))
				etatPresent = VERT2;
			break;
		
		case (VERT2):
			PORTA = LED_GREEN;
			if(pressedButton(PIND3))
				etatPresent = INIT;

            else if (pressedButton(PIND2))
                etatPresent = ROUGE2;
			break;

		case (ROUGE2):
			PORTA = LED_RED;
			if (pressedButton(PIND2))
				etatPresent = VERT2;
			break;
		}
	}
}