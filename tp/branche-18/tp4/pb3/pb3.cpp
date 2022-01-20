#include <avr/io.h>
#include <stdint.h> //uint8_t

#define F_CPU 8000000UL
#include <util/delay.h>

const uint16_t deuxSecondes = 2000;
const uint8_t p0 = 0;
const uint8_t p25 = 64;
const uint8_t p50 = 128;
const uint8_t p75 = 191;
const uint8_t p100 = 254;
enum CurrentSpeed{
    speedOne,
    speedTwo,
    speedThree,
    speedFour,
    speedFive
} speed = speedOne;

void init(){
    DDRD |= (1 << DDD4) | (1 << DDD5); //en mode sortie

    TCCR1A = (1 << WGM10); //mode PWM phase correct 8-bit  (top est de 255)

    TCCR1A |= ((1 << COM1A1) | (1 << COM1B1)); //Clear (met OC1A/OC1B a 0) upcounting, set downcounting (met OC1A/OC1B a 1)

    TCCR1B = (1 << CS11); //division de 8000000 par 8 donc cpu de 1000000hz
    TCCR1C = 0;
}

void ajustementPWM (uint8_t duree) {
    OCR1A = duree;
    OCR1B = duree;
}

void PWM()
{
    switch(speed)
    {
        case speedOne:
            ajustementPWM(p0); 
            _delay_ms(deuxSecondes);
            speed = speedTwo;

        case speedTwo:        
            ajustementPWM(p25); 
            _delay_ms(deuxSecondes);
            speed = speedThree;

        case speedThree:
            ajustementPWM(p50); 
            _delay_ms(deuxSecondes);
            speed = speedFour;
            
        case speedFour:
            ajustementPWM(p75); 
            _delay_ms(deuxSecondes);
            speed = speedFive;
            
        case speedFive:
            ajustementPWM(p100); 
            _delay_ms(deuxSecondes);
            speed = speedOne;
    }
}

int main()
{
    init();
    for(;;){ PWM(); }
    return 0;
}