/***********************************************************************
* Fichier:	    pb2.cpp 					   
* Auteurs:	    Antoine Déry (2049399) et Stéphanie Ly (1925212)
* Équipe:       03					   
* Section:      01					   
* Date:         2020/09/18
* Description:  Contrôle des moteurs du robot avec un signal PWM
***********************************************************************/

#define F_CPU 8000000
#include <util/delay.h>
#include <avr/io.h>

//Variables constantes
const uint8_t POURCENTAGE_60HZ = 167;    //167 us = 0,1667 ms --> 1% de 16.667 ms (periode de 16.667 ms car fréquence de 60 Hz)
const uint8_t POURCENTAGE_400HZ = 25;    //25 us = 0,025 ms --> 1% de 2.5 ms (periode de 2.5 ms car fréquence de 400 Hz)
const uint8_t POURCENTAGE_PERIODE = 100; //Une période complète correspond à 100% du PWM
const uint8_t NOMBRE_RAPPORTS = 5;       //5 rapports car PWM = 0%, 25%, 50%, 75%, 100%
const int NOMBRE_CYCLE_60HZ = 120;       //120 cycles de 16.667 ms = 2 000 ms
const int NOMBRE_CYCLE_400HZ = 800;      //800 cycles de 2.5 ms = 2 000 ms
const uint8_t INCREMENTATION_A = 25;     //0%, 25%, 50%, 75%, 100%


/***************************************************************************
* Fonction:     delay
* Description:  Elle permet de contourner la fonction _delay_ms() en lui
                passant en paramètres autre chose qu'un entier constant 
* Paramètres:   frequence : frequence du signal PWM (60 Hz ou 400 Hz)
                pourcentage : pourcentage de l'onde a ou (b-a) par rapport à la période       
* Retour:       Aucun
****************************************************************************/
void delay(int frequence, int pourcentage)
{
    if (frequence == 60)
    {
        for (int i = 0; i < pourcentage; i++)
            _delay_us(POURCENTAGE_60HZ);
    }

    else if (frequence == 400)
    {
        for (int i = 0; i < pourcentage; i++)
            _delay_us(POURCENTAGE_400HZ);
    }
}


int main()
{
    DDRB &= ~((1 << DDB0) | (1 << DDB1)); //Output: Bit clear B0 et B1

    while (1)
    {
        //60 Hz
        int pourcentageA = 0;
        for (int i = 0; i < NOMBRE_RAPPORTS; i++)
        {
            for (int j = 0; j < NOMBRE_CYCLE_60HZ; j++)
            {
                PORTB = (1 << PORTB0);
                delay(60, pourcentageA);
                PORTB = ~(1 << PORTB0);
                delay(60, (POURCENTAGE_PERIODE - pourcentageA)); //pourcentage de b-a
            }
            pourcentageA += INCREMENTATION_A;
        }

        //400 Hz
        pourcentageA = 0;
        for (int i = 0; i < NOMBRE_RAPPORTS; i++)
        {
            for (int j = 0; j < NOMBRE_CYCLE_400HZ; j++)
            {
                PORTB = (1 << PORTB0);
                delay(400, pourcentageA);
                PORTB = ~(1 << PORTB0);
                delay(400, (POURCENTAGE_PERIODE - pourcentageA));; //pourcentage de b-a
            }
            pourcentageA += INCREMENTATION_A;
        }
    }
}