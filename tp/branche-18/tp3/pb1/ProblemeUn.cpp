#include <avr/io.h>
#include <stdint.h> //uint8_t

#define F_CPU 8000000UL
#include <util/delay.h>

const bool red = true;
const bool green = false;
const int maxCycle = 273; //nbr de cycles execute par vitesse (273x11=3000ms=3s)
const int maxSpeed = 10; //nbr de vitesses (PWM) possibles

void turnOnRed()
{
    PORTA = 1; //rouge
}

void turnOnGreen()
{
    PORTA = 1 << PORTA1; //vert
}

void turnOn(const bool& color)
{
    if(color == red) { turnOnRed(); }
    else { turnOnGreen(); }
}

void turnOff()
{
    PORTA = 0x00;
}

void PWM(const bool& color, const int& speed)
{
    for(int cycle = 0; cycle < maxCycle; cycle++){
        switch(speed)
        {
            case 10:
                turnOn(color);
                _delay_ms(1);
                break;
            case 9:
                turnOn(color);
                _delay_ms(0.9);
                turnOff();
                _delay_ms(0.1);
                break;
            case 8:
                turnOn(color);
                _delay_ms(0.8);
                turnOff();
                _delay_ms(0.2);
                break;
            case 7:
                turnOn(color);
                _delay_ms(0.7);
                turnOff();
                _delay_ms(0.3);
                break;
            case 6:
                turnOn(color);
                _delay_ms(0.6);
                turnOff();
                _delay_ms(0.4);
                break;
            case 5:
                turnOn(color);
                _delay_ms(0.5);
                turnOff();
                _delay_ms(0.5);
                break;
            case 4:
                turnOn(color);
                _delay_ms(0.4);
                turnOff();
                _delay_ms(0.6);
                break;
            case 3:
                turnOn(color);
                _delay_ms(0.3);
                turnOff();
                _delay_ms(0.7);
                break;
            case 2:
                turnOn(color);
                _delay_ms(0.2);
                turnOff();
                _delay_ms(0.8);
                break;
            case 1:
                turnOn(color);
                _delay_ms(0.1);
                turnOff();
                _delay_ms(0.9);
                break;
            case 0:
                _delay_ms(1);
        }
    }
}

void attenuation(const bool& color)
{
    for(int speed = maxSpeed; speed >= 0; speed--){
        PWM(color, speed);
    }
}

int main()
{
    DDRA = 0xff; // PORT A est en mode sortie

    for(;;){
        attenuation(red);
        attenuation(green);
    }
    return 0;
}