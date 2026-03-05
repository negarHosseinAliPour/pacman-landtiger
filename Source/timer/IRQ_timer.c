/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_timer.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    timer.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include <string.h>
#include "LPC17xx.h"
#include "timer.h"
#include "GLCD/GLCD.h" 
#include <stdio.h> /*for sprintf*/

/******************************************************************************
** Function name:		Timer0_IRQHandler
**
** Descriptions:		Timer/Counter 0 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
/*uint16_t SinTable[45] =                                                      
{
    410, 467, 523, 576, 627, 673, 714, 749, 778,
    799, 813, 819, 817, 807, 789, 764, 732, 694, 
    650, 602, 550, 495, 438, 381, 324, 270, 217,
    169, 125, 87 , 55 , 30 , 12 , 2  , 0  , 6  ,   
    20 , 41 , 70 , 105, 146, 193, 243, 297, 353
};*/
extern GameState_t CurrentState;
extern volatile uint8_t isPaused;  // 0 = Running, 1 = Paused
extern volatile uint8_t islose;  // 0 = Running, 1 = Paused



// Declare the currentState variable
// extern volatile enum GameState currentState;
volatile uint32_t remainingTime = 60;  // Start with 60 seconds

void TIMER0_IRQHandler (void)
{

  LPC_TIM0->IR = 1;			/* clear interrupt flag */
  return;
}


/******************************************************************************
** Function name:		Timer1_IRQHandler
**
** Descriptions:		Timer/Counter 1 interrupt handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile uint8_t updateCountdownFlag = 0;  // Flag to signal countdown display update

void TIMER1_IRQHandler(void) {
    if (remainingTime > 0) {
        remainingTime--;
        
        // Update countdown display
        updateCountdownFlag = 1;

        // Check if all bills are eaten
        if (!areBillsRemaining()&& remainingTime >= 0) {
            // Display Winner message

            GUI_Text(100, 180, (uint8_t *)"WINNER!", Green, Black);

            // Stop the timer or game
            disable_timer(1);
            CurrentState = INIT;  // Stop game loop
					return;
        }
    } else {
        CurrentState = GAME_OVER;  // Set the game state to "GAME_OVER"
        // Timer reached zero; display result
        if (areBillsRemaining()) {
            // Display Loser message
            GUI_Text(100, 180, (uint8_t *)"LOSER!", Red, Black);
					// Stop Pacman from moving
            islose = 1;

        } else {
            // Display Winner message
            GUI_Text(100, 180, (uint8_t *)"WINNER!", Green, Black);
        }

        // Stop the timer or game
        disable_timer(1);
        CurrentState = INIT;  // Stop game loop
    }

    LPC_TIM1->IR = 1;  // Clear interrupt flag
}

/******************************************************************************
**                            End Of File
******************************************************************************/