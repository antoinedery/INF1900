#include <avr/io.h>
#include <stdint.h> //uint8_t

#include <avr/interrupt.h>
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/eeprom.h>

bool active = false;
bool red = false;
bool green = true;
const uint8_t oneBit = 1;
const uint8_t adress = 0;
uint8_t dataIn[]("*P*O*L*Y*T*E*C*H*N*I*Q*U*E* *M*O*N*T*R*E*A*L*");
uint8_t dataOut[sizeof(dataIn)];

void turnOn()
{
    if(!active){return;}
    if(red){PORTA = oneBit << PORTA0;}  
    else if(green){PORTA = oneBit << PORTA1;}
    active = false;
}

void writeData()
{
    eeprom_write_block((const void*)&dataIn, (void*)adress, sizeof(dataIn)); 
}

void readData()
{
    eeprom_read_block((void*)&dataOut, (const void*)adress, sizeof(dataIn));
}

void compareIO()
{
    for(unsigned int i = 0; i < sizeof(dataIn); i++)
    {
        if(dataIn[i] != dataOut[i])
        {
            green = false; 
            red = true;
            break;
        }
    } 
    active = true;
}

void init() 
{
    writeData();
    readData();
    compareIO();
    turnOn();
}

int main() { init(); }
