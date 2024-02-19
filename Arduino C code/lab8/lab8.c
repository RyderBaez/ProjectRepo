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
	volatile int16_t count = 128;		// Count to display
	volatile int16_t width = 3000;		// controls speed of motor
	volatile uint8_t changed = 0;  // Flag for state change
	volatile uint32_t period;    //keeps track of how many times the timer is called
int main(void) {
    
    
   
    char buf[30];
    // Initialize DDR and PORT registers and LCD
    //   lcd_init();
    //   lcd_writecommand(1);//clears LCD
	  DDRB |= (1<<PB3) | (1<<PB2);
      PORTC |= (1 << PC4) | (1 << PC5);
	  PCICR |= (1 << PCIE1);
	  PCMSK1 |= (1 << PC4) | (1<< PC5);
	  sei();
	  timer1_init(); //initialize both timers
	  timer2_init();


    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value. 
	     x = PINC;   
		 if(x & (1 << PC4)){
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
	//lcd_stringout(buf);
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
	// x = PINC;
	//  if((x & (1 << PC4)) == 0){
	// 		OCR2A = 51;
	// 	 }
	// else{
	// 	OCR2A = 204;
	// }
    
    }
}





ISR(PCINT1_vect)
{
    // In Task 6, add code to read the encoder inputs and determine the new
    // count value
	     x = PINC;   
		  if(x & (1 << PC4)){
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
			width +=30; //updates servo by 30 as 1/3000 was too insignificante
			new_state = 1;
		}
		if(b == 1){
			count--;
			width -= 30;
			new_state = 2;
		}

	}
	else if (old_state == 1) { // 0 1
           if(a == 0){
				count--;
				width -= 30;
				new_state = 0;
			}
			if(b == 1){
				count++;
				width += 30;
				new_state = 3;
			}
	    // Handle A and B inputs for state 1

	}
	else if (old_state == 2) { // 1 0
            if(a == 1){
			 	count--;
				width -= 30;
				new_state = 3;
			}
			if(b == 0){
				count++;
				width += 30;
				new_state = 0;
			}
	    // Handle A and B inputs for state 2

	}
	else {   // old_state = 3 // 1 1
			if(a == 0){
				count++;
				width += 30;
				new_state = 2;
			}
			if(b == 0){
				count--;
				width -= 30;
				new_state = 1;
			}
	    // Handle A and B inputs for state 3

	}
	if(count < 0){ //corrects out of bounds values
		count = 0;
	}
	if(count > 255){//corrects out of bounds values
		count = 255;
	}
	if(width < 1500){//corrects out of bounds values
		width = 1500;
	}
	if(width > 4500){//corrects out of bounds values
		width = 4500;
	}
	OCR2A = count; //updates the PWNs
	OCR1B = width;
	 
}


void timer1_init() //motor
{
    TCCR1A |= (1 << WGM10) | (1 << WGM11); // Fast PWM mode, modulus = 256
	TCCR1B |= (1 << WGM13) | (1 << WGM12);
	TCCR1A |= (0b10 << COM1B0); // Turn D11 on at 0x00 and off at OCR1B
	OCR1A = 40000;
	OCR1B = 3000;                // Initial pulse duty cycle of 50%
    TCCR1B |= (0b010 << CS10);  // Prescaler = 1024 for 16ms period
    
}
void timer2_init()//led
{
    TCCR2A |= (1 << WGM20) | (1 << WGM21); // Fast PWM mode, modulus = 256
	TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
	OCR2A = 128;                // Initial pulse duty cycle of 50%
    TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
}

