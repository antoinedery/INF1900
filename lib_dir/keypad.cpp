/***************************************************************************
* Fichier:		Keypad.cpp	 					   
* Auteurs:		Antoine Déry, Stéphanie Ly, Alexander Ciaciek, Wael Tarifi
* Équipe:		03-18					   
* Section:		01					   
* Date:			17 novembre 2020				   
* Description:  Implémentation de la classe Keypad			   
***************************************************************************/

#include "keypad.h"

const uint8_t BUTTON_POSITION   = 0;
const uint8_t OUTPUT_PINS       = 0b111110;

const uint8_t COLUMN_1          = 0b100000; 
const uint8_t COLUMN_2          = 0b101000; 
const uint8_t COLUMN_3          = 0b110000; 
const uint8_t COLUMN_4          = 0b111000; 

const uint8_t ROW_1             = 0b000000; 
const uint8_t ROW_2             = 0b000010; 
const uint8_t ROW_3             = 0b000100; 
const uint8_t ROW_4             = 0b000110; 

uint8_t COLUMNS [4] = {COLUMN_1, COLUMN_2, COLUMN_3, COLUMN_4};
uint8_t ROWS    [4] = {ROW_1, ROW_2, ROW_3, ROW_4};

const uint8_t KEY_0 = ROW_1 + COLUMN_1;
const uint8_t KEY_1 = ROW_1 + COLUMN_2;
const uint8_t KEY_2 = ROW_1 + COLUMN_3;
const uint8_t KEY_3 = ROW_1 + COLUMN_4;
const uint8_t KEY_4 = ROW_2 + COLUMN_1;
const uint8_t KEY_5 = ROW_2 + COLUMN_2;
const uint8_t KEY_6 = ROW_2 + COLUMN_3;
const uint8_t KEY_7 = ROW_2 + COLUMN_4;
const uint8_t KEY_8 = ROW_3 + COLUMN_1;
const uint8_t KEY_9 = ROW_3 + COLUMN_2;
const uint8_t KEY_A = ROW_3 + COLUMN_3;
const uint8_t KEY_B = ROW_3 + COLUMN_4;
const uint8_t KEY_C = ROW_4 + COLUMN_1;
const uint8_t KEY_D = ROW_4 + COLUMN_2;
const uint8_t KEY_E = ROW_4 + COLUMN_3; 
const uint8_t KEY_F = ROW_4 + COLUMN_4;

/***************************************************************************
* Méthode:      Keypad::Keypad
* Description:  Constructeur par paramètres de la classe Keypad 
* Paramètres:   (tag)   :   Lettre du port a initialiser                
* Retour:       Aucun         
****************************************************************************/
Keypad::Keypad(const Port& port)
    : port_(port), 
      button_(port, BUTTON_POSITION)
{
    port_.setOut(OUTPUT_PINS);
}

/***************************************************************************
* Méthode:      Keypad::scan
* Description:  Détecte le bouton du clavier appuye par l'utilisateur
* Paramètres:   Aucun                
* Retour:       La valeur ascii du bouton appuye sur le clavier         
****************************************************************************/
uint8_t Keypad::scan()
{
    for(;;){
        for(uint8_t row : ROWS){
            for(uint8_t column : COLUMNS){
                port_.write(column + row);
                if(button_.isClicked()){
                    switch(column + row){
                        case KEY_0:
                            return 0x0;
                        case KEY_1:
                            return 0x1;
                        case KEY_2:
                            return 0x2;
                        case KEY_3:
                            return 0x3;
                        case KEY_4:
                            return 0x4;
                        case KEY_5:
                            return 0x5;
                        case KEY_6:
                            return 0x6; 
                        case KEY_7:
                            return 0x7;
                        case KEY_8:
                            return 0x8;
                        case KEY_9:
                            return 0x9;
                        case KEY_A: 
                            return 0xA;
                        case KEY_B: 
                            return 0xB;
                        case KEY_C: 
                            return 0xC;
                        case KEY_D: 
                            return 0xD;
                        case KEY_E: 
                            return 0xE;
                        case KEY_F:  
                            return 0xF;
                    }
                }
            }
        }
    }
}

