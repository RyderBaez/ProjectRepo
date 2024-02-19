/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 7 - Rotary Encoder
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"

void play_note(uint16_t);
void variable_delay_us(int16_t);

// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
uint16_t frequency[8] =
    { 262, 294, 330, 349, 392, 440, 494, 523 };
	volatile uint8_t a, b, x; //
    volatile uint8_t new_state, old_state;
	volatile int16_t count = 0;		// Count to display
	volatile uint8_t changed = 0;  // Flag for state change
	volatile uint32_t period;    //keeps track of how many times the timer is called
int main(void) {
    
    
   
    char buf[30];
    // Initialize DDR and PORT registers and LCD
      lcd_init();
      lcd_writecommand(1);//clears LCD
	  DDRB |= (1<<PB4);
      PORTC |= (1 << PC1) | (1 << PC5);
	  PCICR |= (1 << PCIE1);
	  PCMSK1 |= (1 << PC1) | (1<< PC5);
	  sei();
	  timer1_init();
    // Write a spash screen to the LCD
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


    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value. 
	     x = PINC;   
		 if(x & (1 << PC1)){
			a = 1;
		 }
         if(x & (1 << PC5)){
			b = 1;
		 }
    if (!b && !a) // 0 0
	old_state = 0;
    else if (!b && a) // 0 1
	old_state = 1;
    else if (b && !a) // 1 0
	old_state = 2;
    else             // 1 1
	old_state = 3;

    new_state = old_state;
    lcd_moveto(0, 0);
    snprintf(buf, 30, "%d", count);
	lcd_stringout(buf);
    while (1) {                 // Loop forever
	// Read the input bits and determine A and B.

	// For the Checkpoint, print the values of A and B on the LCD.
     //if(new_state != old_state){
		//lcd_moveto(0, 0);
       // snprintf(buf, 10, "A=%d B=%d", a, b);
		//lcd_stringout(buf);
	// }
	// The following code is for Tasks 4 and later.
	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	   

	// If state changed, update the value of old_state,
	// and set a flag that the state has changed.
	

        if (changed) { // Did state change?
	    changed = 0;        // Reset changed flag

	    // Output count to LCD
		lcd_writecommand(1);//clears LCD
	 	lcd_moveto(0, 0);
        snprintf(buf, 30, "%d", count);
		lcd_stringout(buf);
	    
        }
    }
}

/*
  Play a tone at the frequency specified for one second
*/
void play_note(uint16_t freq)
{
   

    period = 1000000 / freq;    // Period of note in microseconds

    while (freq--) {
	PORTB |= (1 << PB4);    // Buzzer output high
	variable_delay_us(period / 2);  // Delay for half the period
	PORTB &= ~(1 << PB4);   // Buzzer output log
	variable_delay_us(period / 2);  // Delay for half the period
    }
}

/*
    variable_delay_us - Delay a variable number of microseconds
*/
void variable_delay_us(int delay)
{
    int i = (delay + 5) / 10;

    while (i--)
        _delay_us(10);
}

ISR(PCINT1_vect)
{
    // In Task 6, add code to read the encoder inputs and determine the new
    // count value
	     x = PINC;   
		  if(x & (1 << PC1)){
			a = 1;
		 }
		 else 
		   a = 0;
         if(x & (1 << PC5)){
			b = 1;
		 }
		 else 
		  b = 0;
    
	if (old_state == 0) { // 0 0
	    // Handle A and B inputs for state 0
		if(a == 1){
			count++;
			new_state = 1;
		}
		if(b == 1){
			count--;
			new_state = 2;
		}

	}
	else if (old_state == 1) { // 0 1
           if(a == 0){
				count--;
				new_state = 0;
			}
			if(b == 1){
				count++;
				new_state = 3;
			}
	    // Handle A and B inputs for state 1

	}
	else if (old_state == 2) { // 1 0
            if(a == 1){
			 	count--;
				new_state = 3;
			}
			if(b == 0){
				count++;
				new_state = 0;
			}
	    // Handle A and B inputs for state 2

	}
	else {   // old_state = 3 // 1 1
			if(a == 0){
				count++;
				new_state = 2;
			}
			if(b == 0){
				count--;
				new_state = 1;
			}
	    // Handle A and B inputs for state 3

	}
	if (new_state != old_state) { //checks if the screen needs to be updated
	    changed = 1;
	    old_state = new_state;

	}
	// Do we play a note?
	    if ((count % 8) == 0) {
		 uint8_t freq;
		// Determine which note (0-7) to play
		 freq = (abs(count) % 64) / 8;
		// Find the frequency of the note
        period = (frequency[freq] * 2); //sets 1 second counter
		// Call play_note and pass it the frequency
        OCR1A = 16000000 / (frequency[freq] * 2); //sets timer value
        TCCR1B |= (1<< CS10); //enables timer

	    }
	
}


void timer1_init()
{
    // In Task 7, add code to inititialize TIMER1, but don't start it counting
    TCCR1B |= (1 << WGM12);
    TIMSK1 |= (1 << OCIE1A);
    //OCR1A = 25000;
    
}
ISR(TIMER1_COMPA_vect)
{
    // In Task 7, add code to change the output bit to the buzzer, and to turn
    // off the timer after enough periods of the signal
	PORTB ^= (1 << PB4); // flips bit
	period--;
	if(period <= 0){
		TCCR1B &= ~(1<< CS10); //turns timer back off
	}
}
