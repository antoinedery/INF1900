/***************************************************************************
* Fichier:		Port.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   
* Description:  Délaration de la classe Port			   
***************************************************************************/

#ifndef PORT_H
#define PORT_H

#include <stdint.h>
#include <avr/io.h>

class Port
{
public: 
    enum Tag
    {
        A,
        B,
        C,
        D
    };
    enum Mode
    {
        IN,
        OUT
    };

public:
    Port(const Tag &tag);
    Port(const Tag &tag, const Mode &mode);

    bool read(const uint8_t &pin);
    void write(const uint8_t &pin);
    void writeOne(const uint8_t &pin);
    void writeZero(const uint8_t &pin);
    void switchValues(const uint8_t &pin1, const uint8_t &pin2);
    void clear();

    void In();
    void Out();
    void setIn(const uint8_t &pin);
    void setOut(const uint8_t &pin);

private:
    void initPort(const Tag &tag);

private:
    volatile uint8_t* port_;
    volatile uint8_t* ddr_;
    volatile uint8_t* pin_;
};

#endif
