/***************************************************************************
* Fichier:		container.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			4 décembre 2020			   		   		   
* Description: 	Déclaration de la classe Container		   
***************************************************************************/

#ifndef CONTAINER_H
#define CONTAINER_H

#include "action.h"
#include <stdlib.h>

class Container
{
public:
    Container();
    ~Container();

    void insert(Action *action);
    void remove(const uint8_t &index);
    void printList(Usart& usart);

    void orderActions(const uint8_t &simHour, const uint8_t &simMinute);

    bool hitEnd();
    Action *dequeue();
    void resetQueue();

private:
    uint8_t size_;
    uint8_t queueIndex_;
    Action **list_;
};

#endif
