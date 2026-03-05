#include "button.h"
#include "LPC17xx.h"

#include "GLCD/GLCD.h" 
#include "joystick/joystick.h"
#include "timer/timer.h"

extern CurrentState;
extern volatile uint8_t joystick_enabled; 

void EINT0_IRQHandler (void)	  	/* INT0														 */
{

    LPC_SC->EXTINT |= (1 << 0); // Clear the interrupt flag for EINT0
}

volatile uint8_t key1_pressed = 0; // Track button press state

extern volatile int isPaused; // 0 = Game running, 1 = Game paused


void EINT1_IRQHandler(void) {
    volatile int i; 
    for (i = 0; i < 50000; i++);  // Small delay


    // Toggle the pause state
    isPaused = !isPaused;

    if (isPaused) {
        // Pause the game
        disable_timer(1);
        GUI_Text(100, 180, (uint8_t *)"PAUSED", Red, Black);
    } else {
        // Resume the game
        enable_timer(1);
        GUI_Text(100, 180, (uint8_t *)"       ", Black, Black);
    }

    LPC_SC->EXTINT |= (1 << 1);  // Clear KEY1 interrupt flag
}

volatile uint8_t paused = 0;       // Game state: 0 = Running, 1 = Paused



void EINT2_IRQHandler (void)	  	/* KEY2														 */
{
  LPC_SC->EXTINT &= (1 << 2);     /* clear pending interrupt         */  

}


