/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: Lab 9 - Hardware components
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>

#include "lcd.h"
#include "adc.h"

void timer1_init(void);
void shift_load(uint8_t, uint8_t);
void shift1bit(uint8_t);

#define BUTTON_CHAN 0

// Values returned by the DFRobot LCD shield
#define ADC_RIGHT    0
#define ADC_UP      51
#define ADC_DOWN   101
#define ADC_LEFT   156
#define ADC_SELECT 204

int main(void)
{
    uint8_t adc_result;
    uint8_t right_button, left_button, up_button, down_button;
    uint8_t color, level, update;

    // Initialize the LCD
    lcd_init();

    // Initialize the ADC
    adc_init();

    // Set DDR bits for output to the counter and shift register
    DDRB |= (1 << PB4);
	DDRC |= (1 << PC3) | (1 << PC5);

    // Initialize the timer
    timer1_init();

    // Enable interrupts
    sei();

    // Write splash screen
	 lcd_writecommand(1);//clears LCD
     lcd_moveto(0, 0);
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

    // Write the LED level and color screen
    lcd_moveto(0, 0);
    lcd_stringout("Level: >D  M  B");
    lcd_moveto(1, 0);
    lcd_stringout("Color: >R  G  B");

    color = 0;
    level=0;
    update = 1;

    while (1) {                 // Loop forever
	adc_result = adc_sample(BUTTON_CHAN);

	// Left and right buttons select which color LED to light up
	right_button = adc_result < ADC_RIGHT+20;
	left_button = (adc_result > ADC_LEFT-20) && (adc_result < ADC_LEFT+20);

	// Move the indicator around
	if (right_button && (color < 2)) {
	    lcd_moveto(1, color * 3 + 7);
	    lcd_writedata(' ');
	    color++;
	    lcd_moveto(1, color * 3 + 7);
	    lcd_writedata('>');
	    update = 1;
	    _delay_ms(200);
	}
	else if (left_button && (color > 0)) {
	    lcd_moveto(1, color * 3 + 7);
	    lcd_writedata(' ');
	    color--;
	    lcd_moveto(1, color * 3 + 7);
	    lcd_writedata('>');
	    update = 1;
	    _delay_ms(200);
	}

	// Up and downbuttons select which PWM level to use
	up_button = (adc_result > ADC_UP-20) && (adc_result < ADC_UP+20);
	down_button = (adc_result > ADC_DOWN-20) && (adc_result < ADC_DOWN+20);

	// Move the indicator around
	if (up_button && (level < 2)) {
	    lcd_moveto(0, level * 3 + 7);
	    lcd_writedata(' ');
	    level++;
	    lcd_moveto(0, level * 3 + 7);
	    lcd_writedata('>');
	    update = 1;
	    _delay_ms(200);
	}
	else if (down_button && (level > 0)) {
	    lcd_moveto(0, level * 3 + 7);
	    lcd_writedata(' ');
	    level--;
	    lcd_moveto(0, level * 3 + 7);
	    lcd_writedata('>');
	    update = 1;
	    _delay_ms(200);
	}

	if (update) {
	    shift_load(level, color+1); //why does this +1 color it offsets the entire bit register
	    update = 0;
	}
    }

    return 0;   /* never reached */
}

void shift_load(uint8_t mux, uint8_t demux) //mux first
{
    // Write code to use the "shift1bit" function to shift the two decoder
    // selector bits, and the two mux selector bits into the shift register
	demux--; // fixes the offset w/ color created in the lcd function
	if(demux == 2){//checks what value should be placed for demux in the 
		shift1bit(1);
		shift1bit(0);
	}
	else if(demux == 1){
		shift1bit(0);
		shift1bit(1);
	}
	else{
		shift1bit(0);
		shift1bit(0);
	}
	if(mux == 2){ //checks what value should be placed for mux in the 
		shift1bit(1);
		shift1bit(0);
	}
	else if(mux == 1){
		shift1bit(0);
		shift1bit(1);
	}
	else{
		shift1bit(0);
		shift1bit(0);
	}

	
}

void shift1bit(uint8_t bit)
{
    // Write code to shift one bit into the shift register
	PORTC |= (bit << PC3);

	PORTC |= (1 << PC5);
	PORTC &= ~(1 << PC5);
	PORTC &= ~(1 << PC3);

}

void timer1_init(void)
{
    TCCR1B |= (1 << WGM12);     // Set for CTC mode using OCR1A for the modulus
    TIMSK1 |= (1 << OCIE1A);    // Enable CTC interrupt
    // 16,000,000 * 0.0005 = 8,000
    OCR1A = 7999;              // Set the counter modulus correctly
    TCCR1B |= (0b001 << CS10);  // Set prescaler for /1, starts timer
}


ISR(TIMER1_COMPA_vect)
{
    PORTB ^= (1 << PB4);
}
