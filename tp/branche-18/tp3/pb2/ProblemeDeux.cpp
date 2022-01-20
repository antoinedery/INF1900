#include <avr/io.h>
#include <stdint.h> //uint8_t

#define F_CPU 8000000UL
#include <util/delay.h>

const int FirstMaxCycle = 120; //nbr de cycles requis (120x(1/60)=2s)   => 2/(1/60) = 120
const int SecondMaxCycle = 800; //nbr de cycles requis (800x(1/400)=2s) => 2/(1/400) = 800
const int MaxSpeed = 5; //nbr de vitesses (PWM) possibles

const double FirstPeriod = 50/3; //(1/60)*1000ms  :temps ecouler pour le nbr de cycles demander,  
const double SecondPeriod = 5/2; //(1/400)*1000ms :ici on parle de la periode ou on est en haut et en bas

bool SwitchPeriod = true;

void turnOn()
{
    PORTB = 1 << PORTB0;
}

void turnOff()
{
    PORTB = 0x00;
}

void firstPWM(const int& CurrentSpeed)
{
    for(int cycle = 0; cycle < FirstMaxCycle; cycle++){
        switch(CurrentSpeed)
        {
            case 1:
                _delay_ms(FirstPeriod);
                break;
            case 2:        
                turnOn();
                _delay_ms(FirstPeriod*0.25);
                turnOff();
                _delay_ms(FirstPeriod*0.75);
                break;
            case 3:
                turnOn();
                _delay_ms(FirstPeriod*0.50);
                turnOff();
                _delay_ms(FirstPeriod*0.50);
                break;
            case 4:
                turnOn();
                _delay_ms(FirstPeriod*0.75);
                turnOff();
                _delay_ms(FirstPeriod*0.25);
                break;
            case 5:
                turnOn();
                _delay_ms(FirstPeriod);
                turnOff();
        }
    }
}

void secondPWM(const int& CurrentSpeed)
{
    for(int cycle = 0; cycle < SecondMaxCycle; cycle++){
        switch(CurrentSpeed)
        {
            case 1:
                _delay_ms(SecondPeriod);
                break;
            case 2:
                turnOn();
                _delay_ms(SecondPeriod*0.25);
                turnOff();
                _delay_ms(SecondPeriod*0.75);
                break;
            case 3:
                turnOn();
                _delay_ms(SecondPeriod*0.50);
                turnOff();
                _delay_ms(SecondPeriod*0.50);
                break;
            case 4:
                turnOn();
                _delay_ms(SecondPeriod*0.75);
                turnOff();
                _delay_ms(SecondPeriod*0.25);
                break;
            case 5:
                turnOn();
                _delay_ms(SecondPeriod);
                turnOff();
        }
    }
}

void wheelRolling(){
    for(int speed = 1; speed <= MaxSpeed; speed++){
        if(SwitchPeriod){
            firstPWM(speed);
            if(speed == MaxSpeed) { SwitchPeriod = false; }
        }
        else{
            secondPWM(speed);
            if(speed == MaxSpeed) { SwitchPeriod = true; }
        }
    }
}

int main()
{
    DDRB = 0xff; // PORT B est en mode sortie
    PORTB = 0x00;
    for(;;){ wheelRolling(); }
    return 0;
}