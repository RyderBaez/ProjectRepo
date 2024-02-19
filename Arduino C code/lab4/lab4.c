/********************************************
*
*  Name:
*  Email:
*  Section:
*  Assignment: Lab 4 - Up/Down counter on LCD display
*
********************************************/

#include <avr/io.h>
#include <util/delay.h>

#include "lcd.h"

enum states { UP, DOWN, PAUSE };

int main(void) {

    // Setup DDR and PORT bits for the 3 input buttons as necessary
     PORTC |= (1<< PC2)|(1<< PC4);
     PORTB |= (1 << PB3);    
    // Initialize the LCD
      lcd_init();
     lcd_writecommand(1); //clears the LCD
     lcd_writedata('0'); //sets the starting character to 0
    // Use a state machine approach to organize your code
    //   - Declare and initialize a variable to track what state you
    //     are in by assigning the values UP, DOWN or PAUSE to that variable.
     char state = PAUSE;
     char cnt = 0; //checks for the number of iterations through
     char disp = 0;
    while (1) {               // Loop forever
       //lcd_writedata(0);
        // Use "if" statements to read the buttons and determine which
        // state you are in
      if((PINC & (1<< PC2)) == 0){
        state = UP; //checks for input of button 1 to set state to UP
      }
      else if((PINC & (1<<PC4)) == 0){
        state = DOWN; //checks for input of button 2 to set state to DOWN
      }
      else if((PINB & (1<<PB3)) == 0){
        state = PAUSE; //checks for input of button 3 to PAUSE the counter
      }

	// Based on the current state, determine the new count value
        if(state == PAUSE){

        }
        else if(state == UP && cnt == 10){ //if total delay is 500ms count!
            cnt = 0;
            if(disp == 9){ //if at max return to min value
                disp = 0;
            }
            else{
                disp++;
            }
            lcd_moveto(0,0);
            lcd_writedata((disp + '0')); //overwrite to LCD

        }
        else if (state == DOWN && cnt == 10){
            cnt = 0;
            if(disp == 0){
                disp = 9; //sets min value back to max
            }
            else{
                disp--;
            }
            lcd_moveto(0,0);
            lcd_writedata((disp + '0')); //overwrite to LCD
        }
        else{
            cnt++; //otherwise increase the counter
        }

	// Perform the output operations if necessary


        // Delay before we go to the next iteration of the loop

      _delay_ms(50);  //shorter delay to better detect button pushes
    }

    return 0;   /* never reached */
}

