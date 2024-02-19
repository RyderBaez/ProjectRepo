/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 5 - Analog-to-digital conversion
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"
#include "adc.h"

void rand_init(void);

int main(void)
{

    // Initialize the LCD
  lcd_init();
lcd_writecommand(1);//clears LCD
    // Initialize the ADC
     adc_init();

    // Initialize the random number function
     rand_init();
     
    // Write splash screen and delay for 1 second
     char date[30];
     char* name = "Ryder Baez";
     char month = 12;
     char day = 19;
     char year = 3;
     lcd_moveto(0, 3);
     lcd_stringout(name);
     snprintf(date, 30, "%2d/%2d/%02d", month, day, year);
     lcd_moveto(1, 4);
     lcd_stringout(date);
     _delay_ms(1000);
     lcd_writecommand(1);//clears LCD
      lcd_moveto(0, 0);
    // Find three random numbers and display on top line
      char buf[30];
      char num1, num2, num3;
      char ans1 = 32, ans2 = 32, ans3 = 32; //creates storage space for answers
      num1 = rand() % 32; //sets the three random numbers on top of the screen
      lcd_moveto(0, 0);
      snprintf(buf, 30, "%3d", num1);
      lcd_stringout(buf);
      num2 = rand() % 32;
      lcd_moveto(0, 3);
      snprintf(buf, 30, "%3d", num2);
      lcd_stringout(buf);
      num3 = rand() % 32;
      lcd_moveto(0, 6);
      snprintf(buf, 30, "%3d", num3);
      lcd_stringout(buf);
    // Show the number selector ('>') on the bottom line
      lcd_moveto(1, 0);
      lcd_writedata('>');
      char posit = 0; //finds >

    while (1) {                 // Loop forever
	// Convert ADC channel for buttons to 0-255
    uint8_t bchannel = adc_sample(0);
    
     //uint8_t adc_result = adc_sample(3);
     //snprintf(buf, 30, "%4d", adc_result);
     //lcd_moveto(0, 0);
    // lcd_stringout(buf);

        // If right or left button pressed, move '>' to new position
      if(bchannel < 15){ //checks for right button
        if(posit != 2){ //checks if all the way to the right
             lcd_moveto(1, posit*3);
             lcd_writedata(' '); //removes old arrow and ads new one
             posit++;
             lcd_moveto(1, posit*3);
             lcd_writedata('>');
              _delay_ms(100);
        }
      }
      if((bchannel < 161) && (bchannel > 141)){//checks for left button
        if(posit != 0){ //checks if all the way to the left
             lcd_moveto(1, posit*3);
             lcd_writedata(' '); //removes old arrow and ads new one
             posit--;
             lcd_moveto(1, posit*3);
             lcd_writedata('>');
              _delay_ms(100);
        }
      }

        // Read potentiometer ADC channel
       uint8_t pchannel = adc_sample(3) / 8; //checks potentiometer value converts to 0-31
       lcd_moveto(1, 14);
       snprintf(buf, 30, "%2d", pchannel);
       lcd_stringout(buf); //prints in the corner
	// Convert ADC channel for potentiometer to 0-255, change to 0-31 and display
       

	// If select button pressed copy number to one of the lock inputs positions
      if((bchannel < 220) && (bchannel > 190)){
         lcd_moveto(1, 3*posit+1); //sets select posistion
         snprintf(buf, 30, "%2d", pchannel);
         lcd_stringout(buf); //puts in number
         if(posit == 0){
            ans1 = pchannel; //saves answer to first slot
         }
         if(posit == 1){
            ans2 = pchannel; //saves answer to second slot
         }
         if(posit == 2){
            ans3 = pchannel; //saves answer to third slot
         }
      }

	// Check to see if all three lock inputs match the combination
      if((ans1 == num1) && (ans2 == num2) && (ans3 == num3)){ //correct combo
          lcd_moveto(0, 10);
          lcd_stringout("UNLOCK");//prints
          while(1){
          } //end code
      }

    }

    return 0;   /* never reached */
}

void rand_init()
{
    int16_t seed = 0;
    uint8_t i, j, x;

    // Build a 15-bit number from the LSBs of an ADC
    // conversion of the channels 1-5, 3 times each
    for (i = 0; i < 3; i++) {
	for (j = 1; j < 6; j++) {
	    x = adc_sample(j);
	    x &= 1;	// Get the LSB of the result
	    seed = (seed << 1) + x; // Build up the 15-bit result
	}
    }
    srand(seed);	// Seed the rand function
}
