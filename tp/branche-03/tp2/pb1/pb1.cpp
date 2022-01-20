/***********************************************************************
* Fichier:	    pb1.cpp 					   
* Auteurs:	    Antoine Déry	2049399
*			    Stéphanie Ly	1925212
* Équipe:       03					   
* Section:      01					   
* Date:         2020/09/09
* Description:  Allumer une DEL en vert en appuyant et relâchant sur un
                bouton-poussoir et, par la suite, allumer celui en rouge
                après avoir appuyé et relâché un autre bouton-poussoir
                à 3 reprises.
***********************************************************************/

/* Table des états
*  État présent |  btnD2  |  btnD3  |   Counter   | Timer |   État suivant    |   LED verte   |   LED rouge  |
* ------------------------------------------------------------------------------------------------------------
*     STATE0    |    X    |    0    |      0      |   0   |      STATE0       |       0       |      0       |
*     STATE0    |    X    |    1    |      0      |   0   |      STATE1       |       0       |      0       |
*     STATE1    |    0    |    X    |      0      |   0   |      STATE1       |       1       |      0       |
*     STATE1    |    1    |    X    |      1      |   0   |      STATE1       |       1       |      0       |
*     STATE1    |    1    |    X    |      2      |   0   |      STATE1       |       1       |      0       |
*     STATE1    |    1    |    X    |      3      |   0   |      STATE2       |       1       |      0       |
*     STATE2    |    X    |    X    |      X      |   0   |      STATE2       |       0       |      1       |
*     STATE2    |    X    |    X    |      X      | 1 000 |      STATE0       |       0       |      1       |
*/

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

//Variables globales
const uint8_t DELAI_ANTIREBOND = 10; //Nombre de millisecondes pour l'antirebond
const int DELAI_ARRET = 1000;        //Nombre de millisecondes pour revenir à l'état initiale l'ouverture de la DEL rouge
const int NB_PUSH_BOUTON = 3;        //Nombre de fois à appuyer et relâcher D2

//ENUMS
enum stateMachine
{
    STATE0,
    STATE1,
    STATE2
};

enum LED
{
    LED_OFF = 0x00,   //LED VERTE (A0) ET LED ROUGE (A1) ÉTEINTES
    LED_GREEN = 0x01, //LED VERTE (A0) ALLUMÉE, LED ROUGE (A1) ÉTEINTE
    LED_RED = 0x02    //LED ROUGE (A1) ALLUMÉE, LED VERTE (A1) ÉTEINTE
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
* Fonction:      pressedButton()
* Description:   Elle vérifie si le bouton a été appuyé ET relâché (avec antirebond)
* Paramètres:    (PIN): PIND sur laquelle lire la valeur d'entrée          
* Retour:        (bool): Si le bouton a été appuyé (avec antirebond), elle 
*                retourne true, sinon false
****************************************************************************/
bool pressedButton(uint8_t PIN)
{
    bool isPressed = PIND & (1 << PIN);
    if (isPressed)
    {
        _delay_ms(DELAI_ANTIREBOND);
        if (isPressed)
        {
            bool isPressed = PIND & (1 << PIN);
            if (!isPressed)
                return true;
        }
    }
    return false;
}

/***************************************************************************
* Fonction:     compteur()
* Description:  Elle vérifie le nombre de fois qu'on a appuyé sur le bouton
* Paramètres:   (nbPush): Le nombre fois qu'on a appuyé sur le bouton
* Retour:       (bool): ELle retourne true si on a appuyé et relâché nbPush 
*               fois sur le bouton, sinon false 
****************************************************************************/
bool compteur(int nbPush)
{
    int compteurD2 = 0;
    while (compteurD2 < nbPush)
    {
        if (pressedButton(PIND2))
            compteurD2++;
    }

    if (compteurD2 == nbPush)
    {
        return true;
    }
    return false;
}

int main()
{
    stateMachine currentState = STATE0;
    configure();
    while (true)
    {
        switch (currentState)
        {
        case (STATE0):
        {
            PORTA = LED_OFF;
            if (pressedButton(PIND3))
            {
                currentState = STATE1;
            }
            break;
        }

        case (STATE1):
        {
            PORTA = LED_GREEN;
            if (compteur(NB_PUSH_BOUTON))
            {
                currentState = STATE2;
            }
            break;
        }

        case (STATE2):
        {
            PORTA = LED_RED;
            _delay_ms(DELAI_ARRET);
            currentState = STATE0;
            break;
        }
        }
    }
    return 0;
}
