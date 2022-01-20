#include <avr/io.h>
#include <stdint.h> //uint8_t

#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>

volatile uint8_t boutonPoussoir = 0;
volatile uint8_t minuterieExpiree = 0;
volatile bool timerStarted = false;
const uint16_t dixSecondes = 10000;
const uint16_t uneSeconde = 7813; // (8000000/1024)

void turnGreen()
{      
    PORTA = 1 << PORTA1;           
}

void turnRed()
{ 
    PORTA = 1 << PORTA0;               
}

void turnOff()
{
    PORTA = 0x00;
}

ISR(TIMER1_COMPA_vect) 
{
    minuterieExpiree = 1;
}

ISR(INT0_vect) 
{
    if(timerStarted)
        boutonPoussoir = 1;
    _delay_ms(30);
}

void init() 
{
    cli();

    DDRA = 0xff;
    //Indique l'interrupteur qu'on veut activer
    EIMSK |= (1 << INT0);
    //Indique si cest front montant/descendant 
    EICRA |= (1 << ISC01) | (1 << ISC00);

    sei();
}

void partirMinuterie ( uint16_t duree ) {
    //Position initale du compteur
    TCNT1 = 0;
    //Position a laquelle le compteur lance une interruption
    OCR1A = duree;
    //Ajuste le mode (PWM=>8-9-10bits, CTC=> mode counter dans ce cas ci)
    TCCR1A = (1 << WGM12);
    //On choisit le diviseur de la frequence du CPU (1, 8, 64, 256, 1024)
    TCCR1B = (1 << CS10) | (1 << CS12);
    //Autre registre non utiliser ici
    TCCR1C = 0;
    // Registre qui setup l'interruption TIMER1_COMPA_vect
    // (comparaison de OCR1A et TCNT1)
    // Si on aurait voulu setup une interruption 
    // pour OCR1B, on aurais setup l'interrupt 
    // en OCIEIB.
    TIMSK1 = (1 << OCIE1A);
    timerStarted = true;
}

void attente(){
    _delay_ms(dixSecondes);
}

void clignotement(){
    turnRed();
    _delay_ms(100);
    turnOff();
}

int main()
{
    init();  
    attente();
    clignotement(); 

    partirMinuterie(uneSeconde);

    do {} while ( minuterieExpiree == 0 && boutonPoussoir == 0 );   

    cli (); //fermeture des interruptions   

    if(boutonPoussoir == 1)
        turnGreen();
    else 
        turnRed();
    
}