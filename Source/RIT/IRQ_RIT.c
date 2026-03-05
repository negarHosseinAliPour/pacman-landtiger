/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "GLCD/GLCD.h"
#include "button_EXINT/button.h"
#include <stdio.h>

#define ROWS  39
#define COLUMNS 30
/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/
// Pacman's position variables (global for simplicity)
volatile uint16_t pacmanX = 1;  // Start X position
volatile uint16_t pacmanY = ROWS -2;  // Start Y position (row)
extern int matrix[ROWS][COLUMNS];  // Declare the matrix (define elsewhere)
extern volatile int isPaused;
volatile int down=0;
extern key1_pressed;
volatile uint8_t movementProcessed = 0;

void RIT_IRQHandler(void) {
    key1_pressed = 0;            // Allow new button presses
		static uint8_t joystickInput = 6;
		uint8_t in = ReadJoystick();
		disable_RIT();
		reset_RIT();
		
		if(in != 0)
			joystickInput = in;
    if (joystickInput > 0 && joystickInput < 5) {
        movepacman(&pacmanX, &pacmanY, joystickInput);
        movementProcessed = 1;
    }
    //LPC_RIT->RICTRL |= 0x1;  // Clear interrupt flag
		LPC_RIT->RICTRL |= (1 << 0); // Clear the RIT interrupt flag

		enable_RIT();
}


/******************************************************************************
**                            End Of File
******************************************************************************/
