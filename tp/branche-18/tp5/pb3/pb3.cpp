#include <avr/io.h>
#include <stdint.h> //uint8_t

#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/eeprom.h>

const int nOctetsMax = 1024;
const uint8_t vide = 0xFF;
uint8_t dataIn[48]("*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*\n");
uint8_t mots[35]("Le simulateur SimulIDE en INF1900\n");

/*
void setPosition(uint8_t index)
{
    eeprom_update_byte((uint8_t*)0, index);
}

uint8_t getPosition()
{
    return eeprom_read_byte((uint8_t*)0);
}
*/

void initUART() 
{
    UBRR0H = 0;
    UBRR0L = 0xCF;
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Transmit(uint8_t donnee) 
{
    while (!((UCSR0A) & (1 << UDRE0))){}
    UDR0 = donnee;
}

/*
uint8_t UART_Receive() 
{
    while (!(UCSR0A & (1<<RXC0))){}
    return UDR0;
}
*/

bool isClear(uint8_t byte){
    return byte == vide;
}


void writeData()
{
    uint8_t byte;
    for(int adress = 0; adress < nOctetsMax; adress++){
        if(isClear(byte = eeprom_read_byte((uint8_t*)adress))) { 
            eeprom_write_block((const void*)&dataIn, (void*)adress, sizeof(dataIn));
            return; 
        }
    }
    /*
    int position = getPosition();
    eeprom_update_block((const void*)&dataIn, (void*)position, sizeof(dataIn));
    setPosition(position + sizeof(dataIn));
    */
}

void readData()
{
    uint8_t byte;
    for(int adress = 0; adress < nOctetsMax; adress++){
        if(isClear(byte = eeprom_read_byte((uint8_t*)adress))) 
            return; 
        UART_Transmit(byte);
    }
    /*
    int lengthOfEEPROM = getPosition() - 1;
    if(lengthOfEEPROM == 0) return;
    uint8_t stringOfData[lengthOfEEPROM];
    eeprom_read_block((void*)&stringOfData, (const void*)1, lengthOfEEPROM);
    for(uint8_t value : stringOfData) { UART_Transmit(value); }
    */
}

/*
void writtingFromUart()
{
    uint8_t byte;
    int adress = 0;
    do{
        byte = UART_Receive();
        for( ; adress < nOctetsMax ; adress++){
            if(isClear(eeprom_read_byte((uint8_t*)adress))) { 
                eeprom_update_byte((uint8_t*)adress, byte);
                break;
            }
        }
    } while (byte != 3);
}
*/

void clearData()
{
    for(int adress = 0; adress < nOctetsMax; adress++){
        if(!isClear(eeprom_read_byte((uint8_t*)adress))) {
            eeprom_update_byte((uint8_t*)adress, vide); 
        }
    }
}

int main()
{
    clearData();
    initUART();
    //setPosition(1);
    //writtingFromUart();
    writeData();
    readData();
}
