/****************************************Copyright (c)****************************************************
**                                      
**                                 http://www.powermcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               main.c
** Descriptions:            The GLCD application function
**
**--------------------------------------------------------------------------------------------------------
** Created by:              AVRman
** Created date:            2010-11-7
** Version:                 v1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             Paolo Bernardi
** Modified date:           03/01/2020
** Version:                 v2.0
** Descriptions:            basic program for LCD and Touch Panel teaching
**
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "LPC17xx.h"
#include "GLCD/GLCD.h" 
#include "timer/timer.h"
#include "joystick/joystick.h"
#include "RIT/RIT.h"
#include "button_EXINT/button.h"
#include <stdio.h>


extern uint8_t updateCountdownFlag;  // Flag to signal countdown display update
volatile int score = 0; // Define and initialize the score variable

#ifdef SIMULATOR
extern uint8_t ScaleFlag; 
extern volatile uint32_t remainingTime;         // Start with 60 seconds
extern void UpdateScoreDisplay(void);

volatile GameState_t CurrentState = INIT;  // Start in INIT state
volatile int pacmanLives = 1;  // Start with 1 life
volatile uint8_t isPaused = 0;  // 0 = Running, 1 = Paused
volatile uint8_t islose = 0;  // 0 = Running, 1 = Paused

#endif

//matrix of the maze
int ROWS =38;
int COLUMNS=30;
int matrix[1140] =
{
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,

	
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1   , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 3, 1, 1   , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1,
    1, 0, 2, 1, 1, 1, 0, 2, 2, 2, 2, 2, 0, 1, 1   , 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 1,
    1, 0, 2, 1, 1, 1, 0, 2, 2, 2, 2, 2, 0, 1, 1   , 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 0, 2, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0, 1, 1   , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 2, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0, 1, 1   , 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, 0, 2, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0, 1, 1   , 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 0, 2, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0, 1, 2   , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1,
    1, 0, 2, 1, 1, 1, 0, 2, 1, 1, 1, 1, 0, 1, 2   , 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 2, 1, 1, 1, 1, 2, 1, 2   , 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 2, 1, 1, 1, 1, 2, 1, 2   , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 2, 1, 1, 1, 1, 2, 1, 2   , 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
    1, 0, 1, 1, 1, 1, 0, 2, 1, 1, 1, 1, 2, 1, 2   , 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1,
		1, 0, 1, 1, 1, 1, 0, 2, 1, 1, 1, 1, 2, 2, 2 	, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   , 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 1,
	
    1, 1, 1, 1, 1, 1, 0, 2, 1, 1, 1, 1, 1, 0, 0   , 0, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 1, 1,
    2, 2, 2, 2, 2, 2, 0, 2, 1, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 0, 2, 1, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 2, 2, 2, 2, 0, 2, 1, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 1, 0, 2, 2, 2, 2, 2, 2, 2, 2,
    1, 1, 1, 1, 1, 1, 0, 2, 1, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 1, 0, 2, 1, 1, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 0, 2, 1, 1, 1, 1, 1, 1, 1	  , 1, 1, 1, 1, 1, 1, 0, 2, 2, 2, 2, 2, 2, 2, 1,
		
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 2, 2, 0, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1	  , 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 0, 2, 2, 0, 1,
    1, 0, 1, 1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2	  , 2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 1, 1, 0, 1,
    1, 3, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1	  , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2	  , 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2   , 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 0, 1, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2	  , 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 0, 1, 1, 3, 1,
    1, 0, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2	  , 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1,
    1, 0, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2	  , 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1,
		1, 2, 1, 1, 0, 1, 2, 1, 1, 1, 1, 1, 2, 1, 2	  , 1, 1, 1, 2, 2, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0	  , 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1	  , 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,


};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int areBillsRemaining() {
		uint16_t x, y;

    for ( y = 0; y < ROWS; y++) {
        for ( x = 0; x < COLUMNS; x++) {
            if (matrix[y * COLUMNS + x] == 0 || matrix[y * COLUMNS + x] == 3) { 
                return 1; // Bills still remain
            }
        }
    }
    return 0; // No bills remain
}

int main(void)
                       {

  SystemInit();  												/* System Initialization (i.e., PLL)  */
	
  LCD_Initialization();
  //TP_Init();
	//TouchPanel_Calibrate();
	
	LCD_Clear(Black);
	BUTTON_init();  //button initialization
				
  // Initialize the score 
  score = 0;
  
	//UpdateScoreDisplay();
	GUI_Text(0, 10, (uint8_t *) " GAME OVER IN  ", White, Black);
	GUI_Text(125, 10, (uint8_t *)"60", White, Black);
	GUI_Text(10, 23, (uint8_t *)"Score: ", White, Black);


	//Maze
	int i=0;
	for(i=0; i<ROWS*COLUMNS; i++){
		int x = i%COLUMNS;
		int y = i/COLUMNS;
		if(matrix[i] ==1){
			LCD_DrawSquare(x, y, Blue);
		}else if (matrix[i]== 2){
				
		}else if (matrix[i] == 3) {
        LCD_DrawExtraBill(x, y, White);  // Draw the extra bill
		}else{
			if (y<5){
				continue;
			}if (y>20 & y<24 & x>=0 & x<6){
				continue;
			}if (y>19 & y<25 & x>=10 & x<19){
				continue;
			}if (y>20 & y<24 & x>=23 & x<30){
				continue;
		}LCD_DrawBill(x, y, White);
		
		}
	}
	DrawLives(pacmanLives);  // Display initial lives

	GUI_Text(100, 180, (uint8_t *)"START", Green, Black);

	//pacman init location
	uint16_t pacmanX = 1;            
	uint16_t pacmanY = ROWS - 2;  
	
	volatile int gameStarted = 0; // Flag to track if the game has started
	
  LCD_Drawpacman(pacmanX, pacmanY, Yellow);

	

//  ****************************************
		
	init_timer(0, 0x4E2 ); 						    /* 500us * 25MHz = 1.25*10^3 = 0x4E2 100*10^-6*25*10^6*/

	enable_timer(0);
	joystick_init();											/* Joystick Initialization            */
	init_RIT(0x004C4B40);									/* RIT Initialization 50 msec       	*/
	enable_RIT();	
	LPC_SC->PCON |= 0x1;									/* power-down	mode										*/
	LPC_SC->PCON &= ~(0x2);						
  init_timer(1, 400000);  							/*208250,7500000 */



while (1) {
	
		if (updateCountdownFlag) {
			char timeStr[16];
			sprintf(timeStr, "%02d", remainingTime);
			GUI_Text(125, 10, (uint8_t *)timeStr, White, Black);  // Update directly
			updateCountdownFlag = 0;
	}
	
    if (!gameStarted) {
        if (isJoystickClicked()) {
            gameStarted = 1;
            GUI_Text(100, 180, (uint8_t *)"START", Black, Black);
            GUI_Text(100, 180, (uint8_t *)"STARTED", Green, Black);
            delay_ms(1000);
            GUI_Text(100, 180, (uint8_t *)"STARTED", Black, Black);
            enable_timer(1);
        }
    } else {
        if (!isPaused) {
            HandleJoystickInput();  // Move Pacman
        }

        if (isPaused) {
            GUI_Text(100, 180, (uint8_t *)"PAUSE", Red, Black);
        }
    }

    __ASM("wfi");
}

}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/