/***************************************************************************
* Fichier:		Debug.h	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		  03-18					   
* Section:		01					   
* Date:			  23 octobre 2020				   		   		   
* Description: 	Déclaration de la classe Debug	   
***************************************************************************/

#include <stdio.h>
#include "usart.h"

#ifdef DEBUG
Usart USART;
char buffer[200];
int n = 0;
#define DEBUG_PRINT(message, specifier)    \
  ;                                        \
  n = sprintf(buffer, message, specifier); \
  for (uint8_t i = 0; i < n; i++)          \
  {                                        \
    USART.transmitChar(buffer[i]);             \
  }

#else
// Sinon on ne fait rien
#define DEBUG_PRINT(message, operand) \
  do                                  \
  {                                   \
  } while (0)
#endif


