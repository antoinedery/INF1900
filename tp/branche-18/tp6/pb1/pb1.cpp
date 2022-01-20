/*
    \author  Alexander Ciaciek, Wael Tarifi
    \version Dernière entrée : 2020-10-13
    \since   Création : 2020-10-13
    \Programme qui execute le tp6.
*/

#include <can.h>
#include <stdint.h>
#include <avr/io.h> 
#define F_CPU 8000000UL
#include <util/delay.h>

const uint16_t MODE_SORTIE = 0xff;
const uint16_t MODE_ENTREE = 0x00;
const uint8_t ROUGE = 1 << PORTB0;
const uint8_t VERT = 1 << PORTB1;
const uint8_t SHIFT = 2;

// Les valeurs de l'intensite de la lumiere 
// et la valeur du delai ont ete obtenue 
// par essais erreur.
// Les distances de la lampe desquels on 
// assume que l'intensite son arbitraire
const uint8_t forte = 254;
const uint8_t faible = 190;
const double delay = 10;

void init() 
{
    DDRB = MODE_SORTIE;
    DDRA = MODE_ENTREE;
}

void colorUpdate(uint8_t intensite)
{
    if(intensite > forte) { PORTB = ROUGE; } 
    else if(intensite < faible) { PORTB = VERT; }
    else
    {
        PORTB = ROUGE;   
        _delay_us(delay);
        PORTB = VERT;
        // Pas besoin d'un deuxieme delai ici. 
        // On se sert de la lecture des prochaines 
        // lignes de code pour etablir un delai
    }
}

int main() 
{ 
    init(); 
    can converter = can();
    for(;;) { colorUpdate(converter.lecture(PORTA0) >> SHIFT); }
}
