/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           joystick.h
** Last modified Date:  2018-12-30
** Last Version:        V1.00
** Descriptions:        Atomic joystick init functions
** Correlated files:    lib_joystick.c, funct_joystick.c
**--------------------------------------------------------------------------------------------------------       
*********************************************************************************************************/

#include "LPC17xx.h"
#include "joystick.h"


#define JOYSTICK_UP_PIN    29
#define JOYSTICK_DOWN_PIN  28
#define JOYSTICK_LEFT_PIN  27
#define JOYSTICK_RIGHT_PIN 26
#define JOYSTICK_CENTER_PIN 25
extern volatile uint16_t pacmanX, pacmanY;  // Pacman's position
//enum GameState { INIT, RUNNING};  // Game states
//volatile enum GameState currentState = INIT;  // Start in INIT state
extern CurrentState;
extern paused;
extern volatile uint8_t isPaused;  // Pause state: 0 = running, 1 = paused

/*----------------------------------------------------------------------------
  Function that initializes joysticks and switch them off
 *----------------------------------------------------------------------------*/
void joystick_init(void) {
    LPC_PINCON->PINSEL3 &= ~((3 << 26) | (3 << 25) | (3 << 24) | (3 << 23) | (3 << 22));  // Set GPIO mode for joystick pins
    LPC_GPIO1->FIODIR &= ~((1 << JOYSTICK_UP_PIN) | (1 << JOYSTICK_DOWN_PIN) | 
                           (1 << JOYSTICK_LEFT_PIN) | (1 << JOYSTICK_RIGHT_PIN) | 
                           (1 << JOYSTICK_CENTER_PIN));  // Set as inputs
}
//void joystick_init(void) {
//	/* joystick up functionality */
//  LPC_PINCON->PINSEL3 &= ~(3<<26);	//PIN mode GPIO (00b value per P1.29)
//	LPC_GPIO1->FIODIR   &= ~(1<<29);	//P1.25 Input (joysticks on PORT1 defined as Input) 
//    // Configure pins for GPIO mode
//   LPC_GPIO1->FIODIR   &= ~(1 << JOYSTICK_UP_PIN);  // Set P1.29 as input
//    // Repeat for other directions
//   LPC_GPIO1->FIODIR   &= ~(1 << JOYSTICK_DOWN_PIN);
//   LPC_GPIO1->FIODIR   &= ~(1 << JOYSTICK_LEFT_PIN);
//   LPC_GPIO1->FIODIR   &= ~(1 << JOYSTICK_RIGHT_PIN);
//   LPC_GPIO1->FIODIR   &= ~(1 << JOYSTICK_CENTER_PIN);
//}
uint8_t ReadJoystick(void) {

    if ((LPC_GPIO1->FIOPIN & (1 << JOYSTICK_UP_PIN)) == 0) {
        return 1;  // Up
    }
    if ((LPC_GPIO1->FIOPIN & (1 << JOYSTICK_DOWN_PIN)) == 0) {
        return 2;  // Down
    }
    if ((LPC_GPIO1->FIOPIN & (1 << JOYSTICK_LEFT_PIN)) == 0) {
        return 3;  // Left
    }
    if ((LPC_GPIO1->FIOPIN & (1 << JOYSTICK_RIGHT_PIN)) == 0) {
        return 4;  // Right
    }
//    if ((LPC_GPIO1->FIOPIN & (1 << JOYSTICK_CENTER_PIN)) == 0) {
//        return 5;  // Center (for pause)
//    }
    return 0;  // Neutral
}
volatile uint8_t joystick_enabled = 1;  // Default: joystick is enabled

void disable_joystick(void) {
    joystick_enabled = 0;  // Disable joystick
}

void enable_joystick(void) {
    joystick_enabled = 1;  // Enable joystick
}
static int moveDelayCounter = 0;
const int moveDelayThreshold = 5; // Adjust this to control the speed


void HandleJoystickInput(void) {

	 if (CurrentState == GAME_OVER || isPaused) {
        return;  // Do nothing if the game is over or paused
    }
	  if (moveDelayCounter >= moveDelayThreshold) {

    uint8_t joystickInput = ReadJoystick();
    if (joystickInput > 0 && joystickInput < 5) {
        movepacman(&pacmanX, &pacmanY, joystickInput);
    }
		        moveDelayCounter = 0;

}
		}

int isJoystickClicked(void) {

    if ((LPC_GPIO1->FIOPIN & (1 << JOYSTICK_CENTER_PIN)) == 0) {
        return 1;  // Center button clicked
    }
    return 0;  // Center button not clicked
}