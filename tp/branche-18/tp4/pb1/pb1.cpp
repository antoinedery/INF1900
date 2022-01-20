#include <avr/io.h>
#include <stdint.h> //uint8_t

#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

volatile bool active = false;
volatile bool red = false;
volatile bool green = false;
volatile enum StateCounter 
{
    InitialState,
    StateOne,
    StateTwo,
    StateThree,
    StateFour
} CurrentState = InitialState;

void initialisation() 
{
    cli ();

    DDRA = 0xff;//sortie
    //on enable les interruption int0 int1
    EIMSK |= (1 << INT0) | (1 << INT1);
    //On ajuste le bounton en front descendant
    EICRA |= (1 << ISC01) | (0 << ISC00) | (1 << ISC11) | (0 << ISC10);

    sei ();
}

void turnOn()
{
    if(!active){return;}
    if(green){        
        PORTA = 1 << PORTA1;
        active = false;
        return;
    } 
    if(red){        
        PORTA = 1 << PORTA0;
        active = false;
    }                  
}

void turnOff()
{
    PORTA = 0x00;
}

void stateMachine()
{
    switch(CurrentState)
    {
        case InitialState:
            if(red){
                CurrentState = StateTwo;
                break;
            }
            if(green){
                CurrentState = StateOne;
                break;
            }
            active = false;
            break;

        case StateOne:
            if(red){
                CurrentState = StateTwo;
                break;
            }
            active = false;
            break;

        case StateTwo:
            if(green){
                CurrentState = StateThree;
                break;
            }
            active = false;
            break;

        case StateThree:
            if(red){
                CurrentState = StateFour;
                break;
            }
            if(green){
                active = false;
                turnOff();
                CurrentState = InitialState;
                break;
            }
            active = false;
            break;
        case StateFour:
            if(red){
                red = false;
                green = true;
                CurrentState = StateThree;
                break;
            }
            active = false;
    }
}

ISR(INT0_vect) 
{
    _delay_ms(30);

    red = true;
    green = false;
    active = true;
    
    stateMachine();
    //registre qui memorise si une interruption survient durant linterruption courante et lexecute apres
    EIFR |= (1 << INTF0);
}

ISR(INT1_vect) 
{
    _delay_ms (30);

    green = true;
    red = false;
    active = true;

    stateMachine();
    //registre qui memorise si une interruption survient durant linterruption courante et lexecute apres
    EIFR |= (1 << INTF1);
}

int main()
{
    initialisation();
    for(;;) { turnOn(); }
}
