/***************************************************************************
* Fichier:		Button.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   		   		   
* Description: 	Implementation de la classe Button		   
***************************************************************************/

#include "button.h"

// Constante pour le delai du rebond lie au bouton-poussoir
const uint8_t DELAY = 50;

/***************************************************************************
* Fonction:     Button::Button
* Description:  C'est le contructeur par parametres de la classe
* Paramètres:   (port)    : Le registre de lecture Pin de l'AVR                  
* Retour:       Aucun         
****************************************************************************/
Button::Button(const Port &port, const uint8_t& pin)
    : port_(port),
      pin_(pin) 
{
    port_.setIn(_BV(pin_));
}

/***************************************************************************
* Fonction:    Button::isPressed
* Description: Elle verifie si le bouton-poussoire est appuyé
* Paramètres:  Aucun                 
* Retour:      (true/false): ELle retourne vrai si le bouton-poussoir est 
                appuyé, sinon false 
****************************************************************************/
bool Button::isPressed()
{
    if (port_.read(pin_))
    {
        _delay_ms(DELAY);
        if (port_.read(pin_))
        {
            return true;
        }
    }
    return false;
}

/***************************************************************************
* Fonction:    Button::isClicked
* Description: Elle verifie si le bouton-poussoir a ete appuye ET relache
* Paramètres:  Aucun                 
* Retour:      (true/false): ELle retourne vrai si le bouton-poussoir a ete 
*              appuyer et relache, sinon false 
****************************************************************************/
bool Button::isClicked()
{
    if (port_.read(pin_))
    {
        _delay_ms(DELAY);
        if (!port_.read(pin_))
            return true;
    }
    return false;
}