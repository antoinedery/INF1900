/***************************************************************************
* Fichier:		Port.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			23 octobre 2020				   
* Description:  Implémentation de la classe Port			   
***************************************************************************/

#include <port.h>

const uint8_t INPUT = 0X00;
const uint8_t OUTPUT = 0XFF;
const uint8_t EMPTY = 0X00;

/***************************************************************************
* Méthode:      Port::Port
* Description:  Constructeur par paramètres de la classe Port qui initialise
                le port sans initialiser son mode d'utilisation
* Paramètres:   (tag)   :   Lettre du port a initialiser                
* Retour:       Aucun         
****************************************************************************/
Port::Port(const Tag& tag)
{
    Port::initPort(tag);
}


/***************************************************************************
* Méthode:      Port::Port
* Description:  Constructeur par paramètres de la classe Port qui le mode du Port
* Paramètres:   (tag)   :   Lettre du port a initialiser
                (mode)  :   Mode du port (IN/OUT)                 
* Retour:       Aucun         
****************************************************************************/
Port::Port(const Tag &tag, const Mode &mode)
{
    Port::initPort(tag);
    switch (mode)
    {
    case IN:
        Port::In();
        break;
    case OUT:
        Port::Out();
    }
}

/***************************************************************************
* Méthode:      Port::Port
* Description:  Methode qui permet d'initialiser le port 
* Paramètres:   (tag)   :   Lettre du port a initialiser                
* Retour:       Aucun         
****************************************************************************/
void Port::initPort(const Tag &tag)
{
    switch (tag)
    {
    case A:
        port_ = &PORTA;
        ddr_ = &DDRA;
        pin_ = &PINA;
        break;
    case B:
        port_ = &PORTB;
        ddr_ = &DDRB;
        pin_ = &PINB;
        break;
    case C:
        port_ = &PORTC;
        ddr_ = &DDRC;
        pin_ = &PINC;
        break;
    case D:
        port_ = &PORTD;
        ddr_ = &DDRD;
        pin_ = &PIND;
    }
}

/***************************************************************************
* Méthode:      Port::read
* Description:  Méthode qui retourne true si le bit de la broche lu est a 1 
                et false si la broche est a 0
* Paramètres:   Aucun                
* Retour:       true/false    
****************************************************************************/
bool Port::read(const uint8_t &pin)
{
    return *pin_ & _BV(pin);
}

/***************************************************************************
* Méthode:      Port::write
* Description:  Méthode qui assigne les broches d'un port a aux valeurs 
                assigne 
* Paramètres:   Nouveau integer non-signe de 8 bit                
* Retour:       Aucun       
****************************************************************************/
void Port::write(const uint8_t &pin)
{
    *port_ = pin;
}

/***************************************************************************
* Méthode:      Port::writeOne
* Description:  Méthode qui ecri 1 a la broche en parametre
* Paramètres:   Broche ou on veut ecrire 1                
* Retour:       Aucun       
****************************************************************************/
void Port::writeOne(const uint8_t &pin)
{ 
    *port_ |= _BV(pin);
}

/***************************************************************************
* Méthode:      Port::writeZero
* Description:  Méthode qui ecri 0 a la broche en parametre
* Paramètres:   Broche ou on veut ecrire 0                
* Retour:       Aucun       
****************************************************************************/
void Port::writeZero(const uint8_t &pin)
{
    *port_ &= ~_BV(pin);
}

/***************************************************************************
* Méthode:      Port::switchValues
* Description:  Méthode qui ecri 1 a la premiere broche et 1 a la deuxieme
*               broche
* Paramètres:   Broches ou on ecrit 0 et 1 respectivement               
* Retour:       Aucun       
****************************************************************************/
void Port::switchValues(const uint8_t &pin1, const uint8_t &pin2)
{
    uint8_t portValue = *port_ | _BV(pin1);
    *port_ = portValue & ~_BV(pin2);
}

/***************************************************************************
* Méthode:      Port::clear
* Description:  Méthode qui remet les valeurs en sortie des broches a 0
* Paramètres:   Aucun            
* Retour:       Aucun       
****************************************************************************/
void Port::clear()
{
    *port_ = EMPTY;
}

/***************************************************************************
* Méthode:      Port::In
* Description:  Méthode qui met tous les broches du port en mode entre
* Paramètres:   Aucun             
* Retour:       Aucun       
****************************************************************************/
void Port::In()
{
    *ddr_ = INPUT;
}

/***************************************************************************
* Méthode:      Port::Out
* Description:  Méthode qui met tous les broches du port en mode sortie
* Paramètres:   Aucun             
* Retour:       Aucun       
****************************************************************************/
void Port::Out()
{
    *ddr_ = OUTPUT;
}

/***************************************************************************
* Méthode:      Port::setIn
* Description:  Méthode qui met certaines broches du port en mode entree
* Paramètres:   Entier non-signee avec les broches que l'on veut mettre 
                en mode entree             
* Retour:       Aucun       
****************************************************************************/
void Port::setIn(const uint8_t &pin) 
{
    *ddr_ &= ~pin;
}

/***************************************************************************
* Méthode:      Port::setOut
* Description:  Méthode qui met certaines broches du port en mode sortie
* Paramètres:   Entier non-signee avec les broches que l'on veut mettre 
                en mode entree               
* Retour:       Aucun       
****************************************************************************/
void Port::setOut(const uint8_t &pin)
{
    *ddr_ |= pin;
}
