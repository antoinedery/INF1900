/*
\file    ProblemeDeux.cpp
\author  Alexander Ciaciek, Wael Tarifi
\version Dernière entrée : 2020-09-18
\since   Création : 2020-09-12
\Programme qui execute le problmeme 2.

Tableau à états en Mealy
╔══════════╦════════╦══════════╦═════════╗
║ États    ║ Entrés ║ États    ║ Sorties ║
║ présents ║        ║ suivants ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ Init     ║ Aucun  ║ Init     ║ Eteint  ║
╠══════════╬════════╬══════════╬═════════╣
║ Init     ║ D2     ║ E1       ║ Rouge   ║
╠══════════╬════════╬══════════╬═════════╣
║ Init     ║ D3     ║ E0       ║ Vert    ║
╠══════════╬════════╬══════════╬═════════╣
║ E0       ║ Aucun  ║ E0       ║ Vert    ║
║          ║ ou D3  ║          ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ E0       ║ D2     ║ E1       ║ Rouge   ║
╠══════════╬════════╬══════════╬═════════╣
║ E1       ║ Aucun  ║ E1       ║ Rouge   ║
║          ║ ou D2  ║          ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ E1       ║ D3     ║ E2       ║ Vert    ║
╠══════════╬════════╬══════════╬═════════╣
║ E2       ║ Aucun  ║ E2       ║ Vert    ║
╠══════════╬════════╬══════════╬═════════╣
║ E2       ║ D2     ║ E3       ║ Rouge   ║
╠══════════╬════════╬══════════╬═════════╣
║ E2       ║ D3     ║ Init     ║ Eteint  ║
╠══════════╬════════╬══════════╬═════════╣
║ E3       ║ Aucun  ║ E3       ║ Rouge   ║
║          ║ ou D3  ║          ║         ║
╠══════════╬════════╬══════════╬═════════╣
║ E3       ║ D2     ║ E2       ║ Vert    ║
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
    
    bool redWasClickedOnce = false;

    for(;;)
    {
        switch(CurrentState)
        {
            case InitialState:
                if(isClicked(red))
                {
                    turnOn(red);
                    CurrentState = StateTwo;
                    break;
                }
                if(isClicked(green))
                {
                    turnOn(green);
                    CurrentState = StateOne;
                }
                break;
            case StateOne:
                if(isClicked(red))
                {
                    turnOn(red);
                    CurrentState = StateTwo;
                }
                break;
            case StateTwo:
                if(isClicked(green))
                {
                    turnOn(green);
                    CurrentState = StateThree;
                }
                break;
            case StateThree:
                if(isClicked(red))
                {
                    redWasClickedOnce = true;
                    turnOn(red);
                    CurrentState = StateFour;
                    break;
                }
                if(isClicked(green))
                {
                    turnOff();
                    CurrentState = InitialState;
                }
                break;
            case StateFour:
                if(isClicked(red) && redWasClickedOnce)
                {
                    redWasClickedOnce = false;
                    turnOn(green);
                    CurrentState = StateThree;
                }
        }
    }
    return 0;
}
