/***********************************************************************
* Fichier:		pb1.cpp	 					   
* Auteurs:		Antoine Déry (2049399) et Stéphanie Ly (1925212)
* TP : 			06	   
* Équipe: 		03					   
* Section: 		01					   
* Date:			2020/10/07					   
* Description:	Faire changer la couleur d'une DEL en fonction de
				l'intensité lumineuse.
***********************************************************************/

#define F_CPU 8000000 //8 MHz
#include <avr/io.h>
#include <util/delay.h>
#include <can.h>

/* 	CALCULS - VOLTAGE --> Vout = Vin*(R2/(R1+R2)) --> R2 = 10 kΩ, R1 = photoresistance (4-11 kΩ), Vin = 5 V
*	-Lumière basse	: photorésistance à 11 kΩ donc tension de (10/21)*5 = 2.38 V
*	-Lumière forte	: photorésistance à 4 kΩ donc tension de (10/14)*5 = 3.57 V 
*	
*	CALCULS - VALEURS ENTIÈRES SUR 8 BITS
*	-Lumière basse	: (2.38 V/5 V)*255 = 121
*	-Lumière forte	: (3.57 V/5 V)*255 = 182	
*
*	Intervalles d'intensité choisis:
*	-Lumière basse : < 130
*	-Lumière ambiante : 130-175
*	-Lumière forte : > 175
*/

//Initialisation des variables constantes
const uint8_t SEUIL_LUMIERE_FAIBLE = 130;
const uint8_t SEUIL_LUMIERE_FORTE = 175;
const uint8_t LED_VERT = 0x01;
const uint8_t LED_ROUGE = 0x02;
const uint8_t DELAI_PWM = 10;
const uint8_t DECALAGE_BITS = 2;
const uint8_t POSITION_LECTURE = 0x00;

/***************************************************************************
* Fonction:     init()
* Description:  Elle initialise les ports de sorties et d'entrees
* Paramètres:   Aucun                       
* Retour:       Aucun         
****************************************************************************/
void init()
{
	DDRB |= (1 << DDB0) | (1 << DDB1); //Configurer DDB0 et DDB1 comme sorties
	DDRA &= ~(1 << DDA0);			   //Configurer DDA0 comme entrée
};

/***************************************************************************
* Fonction:     couleurAmbree()
* Description:  Elle produit la couleur ambrée en faisant alterner rapidement
*               deux couleurs 
* Paramètres:   Aucun                       
* Retour:       Aucun         
****************************************************************************/
void couleurAmbree()
{
	PORTB = LED_VERT;
	_delay_ms(DELAI_PWM);
	PORTB = LED_ROUGE;
	_delay_ms(DELAI_PWM);
}

int main()
{
	init();
	can convertisseur;
	while (true)
	{
		uint8_t intensite = (convertisseur.lecture(0x00) >> DECALAGE_BITS); //Décalage de 2 bits à droite pour garder les 8 bits les plus significatifs.

		if (intensite < SEUIL_LUMIERE_FAIBLE) //Si intensité < 130, allumer lumière verte
		{
			PORTB = LED_VERT;
		}

		else if (intensite > SEUIL_LUMIERE_FORTE) //Si intensité > 175, allumer lumière rouge
		{
			PORTB = LED_ROUGE;
		}

		else //Si 130 < intensité > 175, allumer lumière ambrée
		{
			couleurAmbree();
		}
	}
}