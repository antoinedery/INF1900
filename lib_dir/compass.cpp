/***************************************************************************
* Fichier:		Compass.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			9 novembre 2020				   		   		   
* Description: 	Implementation de la classe Compass	qui permet d'afficher 
                l'orientation sur la matrice 3x3
***************************************************************************/

#include <compass.h>

/***************************************************************************
* Méthode:      Compass::Compass
* Description:  Constructeur par défaut de la classe Compass
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
Compass::Compass(const Port& port, const Pole& pole)
    : pole_(pole), port_(port)
{
    port_.Out();
}

/***************************************************************************
* Méthode:      Compass::rightShift
* Description:  Permet de faire une rotation à droite de 90° selon la position courante
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
void Compass::rightShift()
{
    switch (pole_)
    {
    case NORTH:
        pole_ = EAST;
        break;
    case EAST:
        pole_ = SOUTH;
        break;
    case SOUTH:
        pole_ = WEST;
        break;
    case WEST:
        pole_ = NORTH;
    }
    turnOn();
}

/***************************************************************************
* Méthode:      Compass::leftShift
* Description:  Permet de faire une rotation à gauche de 90° selon la position courante
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
void Compass::leftShift()
{
    switch (pole_)
    {
    case NORTH:
        pole_ = WEST;
        break;
    case WEST:
        pole_ = SOUTH;
        break;
    case SOUTH:
        pole_ = EAST;
        break;
    case EAST:
        pole_ = NORTH;
    }
    turnOn();
}

/***************************************************************************
* Méthode:      Compass::turnOn
* Description:  Permet d'écrire l'orientation sur la matrice 3x3
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
void Compass::turnOn()
{
    port_.write(pole_);
}

/***************************************************************************
* Méthode:      Compass::turnOff
* Description:  Permet d'éteinte la matrice 3x3
* Paramètres:   Aucun             
* Retour:       Aucun      
****************************************************************************/
void Compass::turnOff()
{
    port_.clear();
}
