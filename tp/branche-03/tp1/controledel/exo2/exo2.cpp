/***********************************************************************
* Fichier: exo2.cpp	 					   
* Auteurs:	Antoine Déry	2049399
*			Stéphanie Ly	1925212
* Équipe: 03					   
* Section: 01					   
* Date: 2020/09/02  						   
* Description: Contrôle d'une DEL avec un bouton poussoir				   
***********************************************************************/

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

/***********************************************************************
* Fonction: antirebond			   
* Description: lecture du bouton poussoir avec antirebond
* Paramètres: --			   
* Retour: un bool qui informe si l'appuie sur le bouton est appuyé sans antirebond				 				   
***********************************************************************/
bool isPressed()
{
    if ((PIND & (1<<PIND2))) //Masque pour isoler la valeur de la pin D2 - Même chose que (PIND & 0x04)
    {
        _delay_ms(10);
        if ((PIND & (1<<PIND2)))
        {
            return true;
        }
    }
    return false;
}

int main()
{
    DDRD = ~(1 << DDD2);              // D2 en mode entrée
    DDRA = (1 << DDA1) | (1 << DDA0); // A0 et A1 en mode sortie

    for(;;) //Boucle infinie
    {
        if (isPressed())
        {
            PORTA = (1 << PORTA0);
        }
        else
        {   
            PORTA = (1 << PORTA1);
        }
    }
}