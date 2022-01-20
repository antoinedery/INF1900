/***************************************************************************
* Fichier:		Compass.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			9 novembre 2020				   		   		   
* Description: 	Déclaration de la classe Compass		   
***************************************************************************/

#ifndef F_CPU
#define F_CPU 8000000UL
#endif

#ifndef COMPASS_H
#define COMPASS_H

#include <port.h>

class Compass
{
    public:
        //Matrice 3x3
        enum Pole
        {
            SOUTH = 0b101100,
            NORTH = 0b101001,
            WEST = 0b110010,
            EAST = 0b011010
        };

    public:
        Compass(const Port& port, const Pole& pole);

        void rightShift();
        void leftShift();
        void turnOn();
        void turnOff();

    private:
        Pole pole_;
        Port port_;
};

#endif