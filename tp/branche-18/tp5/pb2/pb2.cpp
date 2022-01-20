#include <avr/io.h>
#include <stdint.h> //uint8_t

#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/eeprom.h>

uint8_t dataIn[48]("*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*\n");
uint8_t mots[35]("Le simulateur SimulIDE en INF1900\n");

void initUART() 
{
    //Set a 2400 bauts (fait pour nous)
    UBRR0H = 0;
    UBRR0L = 0xCF;
    //Enable la transmission et la reception
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    //Format : 8 bits tranmis 
    UCSR0C  = (1 << UCSZ01) | (1 << UCSZ00);
    //Non-parity mode
    UCSR0C |= (0 << UPM01)  | (0 << UPM00);
    //separer par un bit darret
    UCSR0C |= (0 << USBSn)
}

void UART_Transmit(uint8_t donnee) 
{
    // Attend que les registres soit vide, 
    // Quand lancienne donnee a ete transmise, 
    // Le registre devient vide quand la transmition
    // est terminer, ainsi cest pret pour la prochaine
    while (!( UCSR0A & (1<<UDRE0))) {}
    //Donnee (caharacter) quon veut transmettre
    UDR0 = donnee;
}

void transmitData()
{
    uint8_t i, j;
    for ( i = 0; i < 100; i++ ) {
        for ( j=0; j < sizeof(dataIn); j++ ) {
            UART_Transmit(dataIn[j]);
        }
    }
}

int main()
{
    initUART();
    transmitData();
}
