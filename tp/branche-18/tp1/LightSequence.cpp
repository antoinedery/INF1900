#include <avr/io.h>
#include <stdint.h> //uint8_t

#define F_CPU 8000000UL
#include <util/delay.h>

const double ShortDelay = 10;
const double LongDelay = 450;

bool isPressed()
{
    if(PIND & (1 << PIND2))
    {
        _delay_ms(ShortDelay);
        if(PIND & (1 << PIND2))
        {
            return true;
        }
    }
    return false;
}

int main()
{
    DDRA = 0xff; // PORT A est en mode sortie
    DDRD = 0x00; // PORT D est en mode entree
    
    for(;;)
    {
        if(isPressed())
        {
            PORTA = 1; //rouge
            _delay_ms(LongDelay);

            PORTA = 1 << PORTA1;//vert
            _delay_ms(LongDelay);

            for(int i = 1; i < 20; i++) //jaune
            {
                PORTA = 1;
                _delay_ms(ShortDelay);
                PORTA = 1 << PORTA1;
                _delay_ms(ShortDelay);
            }
        }
        else
        {
            PORTA = 0x00;
        }
    }
    return 0;
}