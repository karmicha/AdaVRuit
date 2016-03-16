
#include "SnakeII.h"

uint8_t direction = UP;		//Bewegungsrichtung

//	Struktur:		Snake
//	Beschreibung:	Diese Struktur stellt die gesteuerte Schlange dar.	
struct Snake	{
	uint8_t snakeX[MAX_LENGTH];	//X-Koordinaten
	uint8_t snakeY[MAX_LENGTH];	//Y-Koordinaten
	uint8_t snakeLength;	//Laenge der Schlange
	uint8_t eraseX, eraseY;		//"Spur" l�schen
	} snake;


//	Struktur:		Food
//	Beschreibung:	Diese Struktur stellt das zu fressende Futter dar.
struct Food		{
	uint8_t foodX, foodY;
	uint8_t foodLED;		//f�r eventuelle Anzeigeeffekte
	} food;
	
/*
//Globale Variablen

struct Snake snake;
struct Food food;*/


//f�r Button-Abfrage
uint8_t buttonRead = FALSE;

/*
//f�r delay in mainloop
unsigned long prevTime = 0;
unsigned long delayTime = 500;
*/

/*
	Funktion:		initSnake
	Beschreibung:	Diese Funktion initialisiert das Spiel Snake. Die Startposition der Schlange 
					wird initialisiert und der restliche Schlangenk�rper wird "gel�scht".
					Anschlie�end wird die Position des ersten Futters initialisiert.*/
void initSnake(uint8_t play0)
{	
	clearDisplay();
	//Button-Initialisierung (fehlt noch)
	snake.snakeLength=1;
	snake.snakeX[0] = 4;	//Startposition
	snake.snakeY[0] = 4;
	for(int i=1; i<MAX_LENGTH; i++)	//restliche Glieder "abschalten"
	{
		snake.snakeX[i] = snake.snakeY[i] = -1;
	}	
	food.foodLED=LED_ON;
	makeFood(play0);	
}

/*
Funktion:		makeFood
Beschreibung:	Diese Funktion erzeugt eine zuf�llige x-y-Position, gleicht sie mit der Schlange
				ab und erzeugt gegebenenfalls eine neue Position. Anschlie�end werden die Werte in
				den Positionsspeicher des Futters geschrieben.*/
void makeFood(uint8_t playInit)
{
	if (playInit == 1)	printBit(food.foodX, food.foodY, LED_OFF);	//letztes Futter l�schen
	uint8_t x = random(15);
	uint8_t y = random(7);
	while(PartOfSnake(x,y))		//Schlange bereits an neuer Futterposition?
	{
		x = random(15);
		y = random(7);
	}
	food.foodX = x;
	food.foodY = y;
}

uint8_t random(uint8_t max)
{
	return TCNT1L % max;
}

//�berpr�ft, ob die Position bereits von der Schlange belegt ist
uint8_t PartOfSnake(uint8_t x, uint8_t y)
{
	for (int i=0; i<snake.snakeLength; i++)
	{
		if(x==snake.snakeX[i] && y==snake.snakeY[i]) return TRUE;
	}
	return FALSE;
}

/*
Funktion:		playSnake
Beschreibung:	Die Funktion ruft zuerst die Initialisierung auf. Die anschlie�ende Schleife fr�gt als 1. ab, 
				ob die Richtung per Eingabe ge�ndert wurde. Nach der Zeit eines "Spielzyklus�"
				werden alle Echtzeitfunktionen einmal ausgef�hrt. Zuletzt werden alle neuen Positionen
				in die Ausgabe geschrieben*/
void playSnake()
{		
	uint8_t play = 0;
	initSnake(play);
	
	uint8_t ui_buttons;	
	
	play = 1;
		
	while (play)
	{		
		ui_buttons = ui_input();	//nimmt Eingangspins auf
		checkButtons(ui_buttons);
		if (ui_timerFlag==1)
		{				
			nextstep();			
			buttonRead = FALSE;						
			ui_timerFlag = 0; 
		}
		ui_buttons = 0;
		drawSnake();
	}	

}

/*
Funktion:		checkButtons
Beschreibung:	�berpr�ft, ob eine Eingabe im aktuellen Spielzyklus bereits registriert wurde.
				Falls nicht, werden die Buttons `links� und `rechts� ausgewertet und gegebenenfalls
				die Richtungsvariable um 1 in- oder dekrementiert, was einer Drehung um 90� entspricht
				Anschlie�end wird ein Flag f�r die erfolgte Tastenauswertung gesetzt*/
void checkButtons(uint8_t ui_buttons)
{
	if(!buttonRead)	//Buttoneingabe bereits geschehen?
	{
		int currentDirection = direction;
		if(b_player_L(ui_buttons) == 1)		
		{
			direction--;
			if(direction < 0) direction = LEFT;			
		}
		else if(b_player_R(ui_buttons) == 1)	
		{
			direction++;
			if(direction > 3) direction = UP;
		}
		buttonRead = (currentDirection != direction);	//Buttoneingabe registriert!
	}
}

/*
Funktion:		nextstep
Beschreibung:	Die Funktion bewegt zuerst den Schlangenk�rper um eine Position weiter und anschlie�end
				den Kopf entsprechend der aktuellen Bewegungsrichtung. Falls der Kopf dabei auf das Futter
				trifft, wird die L�nge der Schlange um 1 inkrementiert. Hat Snake danach noch nicht seine
				maximale L�nge erreicht, wird eine Funktion zum neuen Setzen des Futters aufgerufen */
void nextstep()
{
	uint8_t play1 = 1;
	for(int i=snake.snakeLength-1; i>0; i--)	//Bewegung Schlangenk�rper
	{
		if (snake.snakeX[i+1] == -1)		//eventuelles L�schbit f�r Ausgabe vorbereiten
		{
			snake.eraseX = snake.snakeX[i];
			snake.eraseY = snake.snakeY[i];
		}
		snake.snakeX[i] = snake.snakeX[i-1];
		snake.snakeY[i] = snake.snakeY[i-1];
	}
	switch(direction)					//Bewegung Kopf
	{
		case UP:
			snake.snakeY[0] = snake.snakeY[0]+1;
			break;
		case RIGHT:
			snake.snakeX[0] = snake.snakeX[0]+1;
			break;
		case DOWN:
			snake.snakeY[0] = snake.snakeY[0]-1;
			break;
		case LEFT:
			snake.snakeX[0] = snake.snakeX[0]-1;
			break;
	}
	if((snake.snakeX[0] == food.foodX) && (snake.snakeY[0] == food.foodY))	//Futter erreicht?
	{
		snake.snakeLength++;
		if (snake.snakeLength < MAX_LENGTH) makeFood(play1);
		else food.foodX = food.foodY = -1;						//maximales Wachstum erreicht
	}
}

/*
Funktion:		drawSnake
Beschreibung:	Schreibt die aktuelle Position des Futters in den Anzeigespeicher. �berpr�ft 
				jedes einzelne Glied der Schlange und schreibt die Positionen
				in den Anzeigespeicher. L�scht zuletzt die "Spur" der Schlange */
void drawSnake()
{	
	//Futter
	if(food.foodX>=0 && food.foodX<16 && food.foodY>=0 && food.foodY<8) printBit(food.foodX, food.foodY, food.foodLED);
	//Snake
	for(int i=0; i<snake.snakeLength; i++)
	{
		if (snake.snakeX == -1) break;		//�berpr�fen, ob bereits aktuelles Ende der Schlange erreicht
		printBit(snake.snakeX[i], snake.snakeY[i], LED_ON);
	}
	printBit(snake.eraseX, snake.eraseY, LED_OFF);
}

