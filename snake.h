#ifndef _SNAKE_H_
#define _SNAKE_H_

#include "main.h"

#include <avr/interrupt.h>

/************************************************************************/
/* Defines und Macros                                                                     */
/************************************************************************/
//Richtungen der Schlange
#define UP		0
#define RIGHT	1
#define DOWN	2
#define LEFT	3

//Verknüpfung mit Eingabefunktionen aus ATmega32.h
#define b_player_L	b_player1_L
#define b_player_R	b_player1_R

#define MAX_LENGTH 20

/************************************************************************/
/* Funktionen                                                                    */
/************************************************************************/
void initSnake(uint8_t play0);
void makeFood(uint8_t playInit);
uint8_t random(uint8_t max);
uint8_t PartOfSnake(uint8_t x, uint8_t y);
void playSnake();
void checkButtons(uint8_t ui_buttons);
void nextstep();
void drawSnake();

#endif
