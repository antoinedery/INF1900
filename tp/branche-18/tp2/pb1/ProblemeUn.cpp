/*
\file    ProblemeUn.cpp
\author  Alexander Ciaciek, Wael Tarifi
\version Dernière entrée : 2020-09-18
\since   Création : 2020-09-12
\Programme qui execute le problmeme 1.

Tableau à états en Mealy
╔══════════╦════════╦══════════╦═════════╗
║ États    ║ Entrés ║ États    ║ Sorties ║
║ présents ║        ║ suivants ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ Init     ║ Aucun  ║ Init     ║ Eteint  ║
║          ║ ou D2  ║          ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ Init     ║ D3     ║ E0       ║ Vert    ║
╠══════════╬════════╬══════════╬═════════╣
║ E0       ║ Aucun  ║ E0       ║ Vert    ║
║          ║ ou D3  ║          ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ E0       ║ D2     ║ E1       ║ Vert    ║
╠══════════╬════════╬══════════╬═════════╣
║ E1       ║ Aucun  ║ E1       ║ Vert    ║
║          ║ ou D3  ║          ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ E1       ║ D2     ║ E2       ║ Vert    ║
╠══════════╬════════╬══════════╬═════════╣
║ E2       ║ Aucun  ║ E2       ║ Vert    ║
║          ║ ou D3  ║          ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ E2       ║ D2     ║ E3       ║ Rouge   ║
╠══════════╬════════╬══════════╬═════════╣
║ E3       ║ Aucun  ║ Init     ║ Eteint  ║
║          ║ ou D2  ║          ║         ║
║          ║ ou D3  ║          ║         ║
╚══════════╩════════╩══════════╩═════════╝
Légende:
╔═════════════════╦═══════════════════════╗
║      Entrés     ║ Sorties               ║
╠═══════╦════╦════╬════════╦═══════╦══════╣
║       ║    ║    ║ Aucun  ║ A0    ║ A1   ║
║ Aucun ║ D2 ║ D3 ╠════════╬═══════╬══════╣
║       ║    ║    ║ Eteint ║ Rouge ║ Vert ║
╠═══════╬════╬════╬════════╬═══════╬══════╣
║ 00    ║ 01 ║ 10 ║ 00     ║ 01    ║ 10   ║
╚═══════╩════╩════╩════════╩═══════╩══════╝
*/

#include <avr/io.h>
#include <stdint.h> //uint8_t

#define F_CPU 8000000UL
#include <util/delay.h>

const bool red = true;
const bool green = false;
const bool pressedState = true;
const bool unpressedState = false;
bool greenState = false;
bool redState = false;  
uint8_t decalagePINA = 0x00;
uint8_t decalagePIND = 0x00;

bool getColorState(const bool& color) //Retourne l'etat du bouton (appuyer ou relacher)
{
    if(color == red)
        return redState;
    return greenState;
}

void setColorState(const bool& color, const bool& state) //Met le bouton dans l'etat appuyer ou l'etat relacher
{
    if(color == red)
        redState = state;
    else
        greenState = state; 
}

bool isPressed(const bool& color) //Si le bouton est appuyer, renvoit true, false sinon
{
    if(color == red)
        decalagePIND = PIND2;
    else
        decalagePIND = PIND3;

    if(PIND & (1 << decalagePIND))
        return true;
    return false;
}

bool isClicked(const bool& color) //Si le bouton a ete appuyer et relacher, renvoit true, false sinon
{
    if(!getColorState(color) && isPressed(color))
    {
        setColorState(color, pressedState);
    }

    if(getColorState(color) && !isPressed(color))
    {
        setColorState(color, unpressedState);
        return true;
    }
    return false;
}

void turnOn(const bool& color)
{
    if(color == red) 
    {
        decalagePINA = 0x00;
    }
    else 
    {
        decalagePINA = PORTA1;
    }
    PORTA = 1 << decalagePINA;
}

void turnOff()
{
    PORTA = 0x00;
}

int main()
{
    DDRA = 0xff; // PORT A est en mode sortie
    DDRD = 0x00; // PORT D est en mode entree

    enum StateCounter 
    {
        InitialState,
        StateOne,
        StateTwo,
        StateThree,
        StateFour
    } CurrentState = InitialState;

    const double OneSecondDelay = 1000; //1s = 1000ms

    for(;;)
    {
        switch (CurrentState)
        {
            case InitialState:
                if(isClicked(green))
                {
                    turnOn(green);
                    CurrentState = StateOne;
                }
                break;
            case StateOne:
                if(isClicked(red))
                {
                    CurrentState = StateTwo;
                }
                break;
            case StateTwo:
                if(isClicked(red))
                {
                    CurrentState = StateThree;
                }
                break;
            case StateThree:
                if(isClicked(red))
                {
                    turnOn(red);
                    CurrentState = StateFour;
                }
                break;
            case StateFour:
                _delay_ms(OneSecondDelay);
                turnOff();
                CurrentState = InitialState;
        }
    }
    return 0;
}
