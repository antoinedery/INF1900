/***********************************************************************
* Fichier: exo1.cpp	 					   
* Auteurs:	Antoine Déry	2049399
*			Stéphanie Ly	1925212
* Équipe: 03					   
* Section: 01					   
* Date: 2020/09/02						   
* Description: Controle d'un DEL  				   
***********************************************************************/

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

int main()
{
    DDRD = ~(1 << DDD2);              // D2 en mode entrée
    DDRA = (1 << DDA1) | (1 << DDA0); // A0 et A1 en mode sortie

    for(;;) //Boucle infinie
    {
        PORTA = (1 << PORTA0);
        _delay_ms(1000);

        PORTA = (1 << PORTA1);
        _delay_ms(1000);
    }
}