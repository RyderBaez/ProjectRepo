/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 6 - Timers
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "lcd.h"

void debounce(uint8_t);

enum states { PAUSE, RUN, LAP };

char deci = 0; //declare variables to store time 
char sec = 0;
volatile char print = 0;
uint8_t state = PAUSE;

int main(void) {

    


    // Initialize the LCD and TIMER1
    lcd_init();
    lcd_writecommand(1);//clears LCD
    DDRD |= (1 << PC5);
    timer1_init();
    // Enable pull-ups for buttons
    PORTC |= (1<<PC2) | (1<<PC4);

    // Show the splash screen
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

    // Enable interrupts
    sei();

    char buf[30];
    lcd_moveto(0, 0);
    snprintf(buf, 30, "%02d.%1d", sec, deci);
    lcd_stringout(buf);
    while (1) {                 // Loop forever


	// Read the buttons
   


	if (state == PAUSE) {               // PAUSE state
      if((PINC & (1<<PC2)) == 0){
        state = RUN; //changes to run and starts counting
        debounce(PC2);
      }
       if((PINC & (1<<PC4)) == 0){
        _debounce(PC4);
        sec = 0; //resets the variables to 0
        deci = 0;
        lcd_moveto(0, 0); //resets the screen to zero
        snprintf(buf, 30, "%02d.%d", sec, deci);
        lcd_stringout(buf); 
      }
	}
	else if (state == RUN) {            // RUN state
      if((PINC & (1<<PC2)) == 0){ //changes to pause
        state = PAUSE;
        debounce(PC2);
      }
      if((PINC & (1<<PC4)) == 0){ //changes to LAP
        state = LAP;
        debounce(PC4);
      }

      

	}
	else if (state == LAP) {            // LAP state
       if((PINC & (1<<PC2)) == 0){ //changes to run
        state = RUN;
        debounce(PC2);
      }
      if((PINC & (1<<PC4)) == 0){ //changes to run
        state = RUN;
        debounce(PC4);
      }



	}

	// If necessary write time to LCD
    if(print && (state != LAP)){
        lcd_moveto(0, 0); //overrides screen
        snprintf(buf, 30, "%02d.%d", sec, deci);
        lcd_stringout(buf); 
        print = 0;//resets truth
    }


    }

    return 0;   /* never reached */
}

/* ----------------------------------------------------------------------- */

void debounce(uint8_t bit)
{
    // Add code to debounce input "bit" of PINC
    // assuming we have sensed the start of a press.
    char buf[30];
    _delay_ms(5);
    while((PINC & (1<< bit)) == 0){ //checks while held down
         if(print){
        lcd_moveto(0, 0); //allows the screen to update during that time
        snprintf(buf, 30, "%02d.%d", sec, deci);
        lcd_stringout(buf); 
        print = 0;
      }
    }
    _delay_ms(5);
}

/* ----------------------------------------------------------------------- */

void timer1_init(void)
{
    // Add code to configure TIMER1 by setting or clearing bits in
    // the TIMER1 registers.
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    OCR1A = 25000;
    TCCR1B |= (1 << CS11) | (1<< CS10);
   
}

ISR(TIMER1_COMPA_vect)
{
    if(state != PAUSE){
    // Increment the time
    deci++;
    if(deci == 10){ //checks for carry
        sec++;
        deci = 0;
    }
    if(sec == 60){ //checks for overflow
        sec = 0;
    }
    print = 1; //tells to update the screen
    }
}
