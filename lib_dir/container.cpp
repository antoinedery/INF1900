/***************************************************************************
* Fichier:		container.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 décembre 2020			   		   		   
* Description: 	Implémentation de la classe Container		   
***************************************************************************/

#include "container.h"

const uint8_t NUMBER_ACTIONS = 100;
const uint8_t INT_TO_ASCII = 48;
const uint8_t NULL_VALUE = 0;
const uint8_t TEN_DIGITS = 10;
const uint8_t INCREMENT_ONE = 1;
const uint8_t SECOND_DIGIT_MULTIPLIER = 10;

/***************************************************************************
* Fonction:     Container::Container
* Description:  Constructeur par défaut de la classe Container
                Alloue la mémoire pour le tableau de pointeurs d'action
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
Container::Container()
    : size_(NULL_VALUE),
      queueIndex_(NULL_VALUE)
{
    list_ = (Action **)malloc(NUMBER_ACTIONS * sizeof(Action *));

    for (uint8_t i = 0; i < NUMBER_ACTIONS; i++)
        list_[i] = nullptr;
}

/***************************************************************************
* Fonction:     Container::~Container
* Description:  Destruction de la classe Container
                Désalloue la mémoire du tableau de pointeurs d'action
* Paramètres:   Aucun          
* Retour:       Aucun       
****************************************************************************/
Container::~Container()
{
    for (uint8_t i = 0; i < size_; i++)
        free(list_[i]);
    free(list_);
}

/***************************************************************************
* Fonction:     Container::insert
* Description:  Permet d'ajouter une action dans la liste de pointeur d'actions
                Fait aussi le tri des actions selon l'heure de déclenchement
* Paramètres:   (action) : pointeur vers l'action à ajouter à la liste          
* Retour:       Aucun       
****************************************************************************/
void Container::insert(Action *action)
{
    action->setInsertPriority(size_);
    list_[size_++] = action;
}

/***************************************************************************
* Fonction:     Container::remove
* Description:  Destruction de la classe Container
* Paramètres:   (index) : Index de l'action à retirer          
* Retour:       Aucun       
****************************************************************************/
void Container::remove(const uint8_t &index)
{
    if (size_ == NULL_VALUE)
        return;
    if (index >= size_)
        return;

    //Libérer l'espace mémoire associée à l'action
    free(list_[index]);
    list_[index] = nullptr;

    size_--;

    //Ajuster les numéros d'actions
    for (uint8_t j = index; j < size_; j++)
        list_[j] = list_[j + INCREMENT_ONE];
}

/***************************************************************************
* Fonction:     Container::printList
* Description:  Affiche la liste d'actions dans le terminal de SimulIDE
* Paramètres:   (usart) : Référence vers le usart          
* Retour:       Aucun       
****************************************************************************/
void Container::printList(Usart& usart)
{
    if (size_ == NULL_VALUE)
    {
        usart.transmitString("\n\nAucune action programmee\n\n");
        return;
    }

    usart.transmitString("\n\nActions programmees :\n");

    uint8_t index = NULL_VALUE;

    for (uint8_t i = 0; i < TEN_DIGITS; i++)
    {
        for (uint8_t j = 0; j < TEN_DIGITS; j++)
        {
            index = (i*SECOND_DIGIT_MULTIPLIER) + j;
            if (index == size_)
            {
                usart.transmitChar('\n');
                return;
            }

            usart.transmitChar(i + uint8_t(INT_TO_ASCII));   //+48 pour transmettre le caractère de la "dizaine" du numéro d'action 
            usart.transmitChar(j + uint8_t(INT_TO_ASCII));   //+48 pour transmettre le caractère de l'unité du numéro d'action      
            usart.transmitString(" - ");

            list_[index]->printAction(usart);
            usart.transmitChar('\n');
        }
    }
}

/***************************************************************************
* Fonction:     Container::orderActions
* Description:  Calcule le nombre de "ticks" d'horloge entre chaque action et l'heure de départ
                permet aussi de garder l'ordre si deux actions sont à la même heure
* Paramètres:   (simHour) : Les heures de départ de la simulation
                (simMinute) : Les minutes de départ de la simulation         
* Retour:       Aucun       
****************************************************************************/
void Container::orderActions(const uint8_t &simHour, const uint8_t &simMinute)
{
    for (uint8_t i = 0; i < size_; i++)
    {
        list_[i]->setClockTicks(simHour, simMinute);
    }
    
    uint8_t minIdx;
    Action* minAction;
    Action* actionTmp;
    for (uint8_t i = 0; i < size_; i++)
    {
        minIdx = i;
        minAction = list_[i];
        for (uint8_t j = i + 1; j < size_; j++)
        {
            if(list_[j]->getClockTicks() == minAction->getClockTicks())
            {
                if(list_[j]->getInsertPriority() < minAction->getInsertPriority())
                {
                    minAction = list_[j];
                    minIdx = j;
                }
            }
            else if (list_[j]->getClockTicks() < minAction->getClockTicks())
            {
                minAction = list_[j];
                minIdx = j;
            }
        }
        actionTmp = list_[i];
        list_[i] = list_[minIdx];
        list_[minIdx] = actionTmp;
    }
}


/***************************************************************************
* Fonction:     Container::hitEnd
* Description:  Vérifie si la liste a atteint la fin
* Paramètres:   Aucun       
* Retour:       Un booléen qui retourne si la liste est a la fin ou non       
****************************************************************************/
bool Container::hitEnd()
{
    if (queueIndex_ < size_)
        return false;
    return true;
}

/***************************************************************************
* Fonction:     Container::dequeue
* Description:  Ajuste la liste d'action pour que le "premier élément" soit la prochaine action
* Paramètres:   Aucun       
* Retour:       La liste d'action mise à jour    
****************************************************************************/
Action *Container::dequeue()
{
    return list_[queueIndex_++];
}

/***************************************************************************
* Fonction:     Container::resetQueue
* Description:  Permet de réinitialiser la "queue"
* Paramètres:   Aucun       
* Retour:       Aucun  
****************************************************************************/
void Container::resetQueue(){
    queueIndex_ = NULL_VALUE;
}