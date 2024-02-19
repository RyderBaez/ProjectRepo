/********************************************
 *
 *  Name:
 *  Email:
 *  Section:
 *  Assignment: project
 *
 ********************************************/

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "lcd.h"

void play_note(uint16_t);
void variable_delay_us(int16_t);
void debouncePB(uint8_t);
void debouncePC(uint8_t);
void timer0_init(void);
void timer1_init(void);
void timer2_init(void);
void transmitData(void);
char rx_char(void);
#define FOSC 16000000 // Clock frequency
#define BAUD 9600 // Baud rate used
#define MYUBRR (FOSC/16/BAUD-1) // Value for UBRR0
// Frequencies for natural notes from middle C (C4)
// up one octave to C5.
uint16_t frequency[8] =
    { 262, 294, 330, 349, 392, 440, 494, 523 };
	volatile uint8_t a, b, x, y, z; //
    volatile uint8_t new_state, old_state;
	volatile int16_t count = 20;		// range
	volatile int16_t distance = 0;		// controls speed of motor
	volatile uint8_t changed = 0;  // Flag for state change
	volatile uint32_t period;;    //keeps track of how many times the timer is called
	volatile uint8_t Ultrachanged = 1; //checks when ultra sonic sensor changes
	volatile uint16_t pulse_count = 0; //takes the times value
	volatile uint8_t distdecimal = 0; //hold the decimal for the cm distance of the sensor
	volatile char threshhold = 0; //0 for local 1 for remote
	volatile int16_t localrange = 20;
    volatile int16_t remoterange = 20;
	volatile uint8_t savedata = 0;
	volatile uint8_t newrange = 0;
	volatile char receivedata = -1; //0 for local 1 for remote
	volatile int receivedrange = 0; //takes the times value
	volatile int receiveddecimal = 0; //hold the decimal for the cm distance of the sensor
	volatile uint8_t datacnt = 0;
	volatile char receivestart = 0; //checks when data has started to be received
	volatile char receivecomplete = 0; //sends when data is finally received
	volatile char data[5];
int main(void) {
    
    
    UBRR0 = MYUBRR;
	UCSR0B |= (1 << TXEN0 | 1 << RXEN0); // Enable RX and TX
    UCSR0C = (3 << UCSZ00); // Async., no parity,
                            // 1 stop bit, 8 data bits
	
	data[4] = '\0';
    char buf[30];
    // Initialize DDR and PORT registers and LCD
    lcd_init();
    lcd_writecommand(1);//clears LCD
	DDRC |= (1 << PC1) | (1 << PC3) | (1 << PC4) | (1 << PC5);
	DDRB |= (1 << PB5) | (1 << PB3);
	PORTB |= (1 << PB4);
    PORTD |= (1 << PD2) | (1 << PD3);
	PORTC &= ~(1 << PC3);
	PCICR |= (1 << PCIE1) | (1 << PCIE2);
	PCMSK2 |= (1 << PCINT18) | (1<< PCINT19);
	PCMSK1 |= (1 << PCINT10);
	UCSR0B |= (1 << RXCIE0); //enables Rx

	sei();
	timer0_init();
	timer1_init(); //initialize all timers
	timer2_init();


    // Read the A and B inputs to determine the intial state.
    // In the state number, B is the MSB and A is the LSB.
    // Warning: Do NOT read A and B separately.  You should read BOTH inputs
    // at the same time, then determine the A and B values from that value. 
	x = PIND;   
	if(x & (1 << PD2)){
		a = 1;
	}
    if(x & (1 << PD3)){
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
	int16_t checkval; 
	checkval = eeprom_read_word((void *) 100); //pulls ranges
	if(checkval > 0 && checkval < 401){
        localrange = checkval;
		count = checkval;
	}
	checkval = eeprom_read_word((void *) 200);
	if(checkval >= 1 && checkval <= 400){
        remoterange = checkval;
	}
    while (1) {                 // Loop forever
	// Read the input bits and determine A and B.
	y = PINB;
	z = PINC;
		if((y & (1 << PB4)) == 0){ //checks if the acquire button has been pressed
			debouncePB(PB4);
			PORTC |= (1 << PC1);
			_delay_us(10);
			PORTC &= ~(1 << PC1);
			
		}
		if((z & (1 << PC0)) == 0){//checks if the remote/local button has been pushed
            debouncePC(PC0);
			if(threshhold){
				threshhold = 0;
				remoterange = count;
				count = localrange;
			}
			else{
				threshhold = 1;
				localrange = count;
				count = remoterange;
			}
			Ultrachanged = 1; 
		}
	// The following code is for Tasks 4 and later.
	// For each state, examine the two input bits to see if state
	// has changed, and if so set "new_state" to the new state,
	// and adjust the count value.
	   if(Ultrachanged){ //checks if the screen needs to be updated 
		distdecimal = (pulse_count / 2 % 58) * 10 / 58 ;
		distance = pulse_count / 116;
		OCR2A = (35 - (23 * distance / 400));
		if(newrange){
		if(Ultrachanged == 2){
			 PORTC |= (1 << PC4); // 1 1
			 PORTB |= (1 << PB5);
		}else if(count > distance){ // 1 0
			 PORTC |= (1 << PC4);
			 PORTB &= ~(1 << PB5);
		}else if(count < distance){
			 PORTC &= ~(1 << PC4); // 0 1
			 PORTB |= (1 << PB5);
		}
		if(threshhold){
		     transmitData();
		}
	   }
		lcd_writecommand(1);//clears LCD
		if(!threshhold){
			lcd_moveto(0, 0);
			lcd_stringout("local ");
		}
		else{
			lcd_moveto(0, 0);
			lcd_stringout("remote");
		}
		lcd_moveto(0, 11);
        snprintf(buf, 30, "%3d.%01d", distance, distdecimal);
		lcd_stringout(buf);
		lcd_moveto(1, 0);
        snprintf(buf, 30, "%d", count);
		lcd_stringout(buf);
		lcd_moveto(1, 11);
        snprintf(buf, 30, "%3d.%01d", receivedrange, receiveddecimal);
		lcd_stringout(buf);
		if(savedata){
		if(!threshhold){
		eeprom_update_word((void *) 100, count); //saves ranges
		} else {
	    eeprom_update_word((void *) 200, count);
		} 
		}
		savedata = 0;
		Ultrachanged = 0; 
		newrange = 0;
	   }
	   if(receivecomplete){ //if all the characters have been sent convert from string to int
			if(datacnt == 1){ //checks for a short number
				data[datacnt+2] = '\0';
				data[datacnt+1] = data[datacnt];
				data[datacnt] = ' ';
				data[datacnt-1] = 0;
			}
			else if(datacnt != 0){ //converts all others to correct format
				data[datacnt+1] = '\0';
				data[datacnt] = data[datacnt - 1];
				data[datacnt-1] = ' ';
			}
			sscanf(data, "%d %d", &receivedrange, &receiveddecimal); //converts to int
			Ultrachanged = 1;
            if(((receivedrange < count) && threshhold) || ((receivedrange < remoterange) && !threshhold)){ //if under the threshhold sound the buzzer!
				period = 250;
				OCR0A = 20;
				TCCR0B |= (1 << CS00) | (1 << CS02); //turns timer back off
			}
			receivecomplete = 0;
	   }
    }
}

ISR(TIMER0_COMPA_vect)
{
	PORTC ^= (1 << PC5); // flips bit
	period--;
	if(period <= 0){
		TCCR0B &= ~(0b111 << CS00); //turns timer back off
	}
}
ISR(PCINT1_vect)
{
	pulse_count = TCNT1;
     if(pulse_count == 0){
     TCCR1B |= (1 << CS11); //sets prescalar to 8
	 }
	 else{
        TCCR1B &= ~(1 << CS11); //sets prescalar to 0
		TCNT1 = 0;
		if(Ultrachanged == 0){
		Ultrachanged = 1;
		newrange = 1;
		}
	 }

	  
}
ISR(TIMER1_COMPA_vect)
{
	pulse_count = 46400;
	TCCR1B &= ~(1 << CS11); //sets prescalar to 0
	TCNT1 = 0;
	Ultrachanged = 2;
	newrange = 1;
}
ISR(PCINT2_vect)
{
    // In Task 6, add code to read the encoder inputs and determine the new
    // count value
	x = PIND;
	// if((x & (1 << PD0)) != 0 && receivedata == -1){
	// 	receivedata = 0;
	// }
	// else if((x & (1 << PD0)) == 0 && receivedata == 0){
	// 	receivedata = 1;
	// } 
	// else { 
	if(x & (1 << PD2)){
		a = 1;
	}
	else 
		a = 0;
    if(x & (1 << PD3)){
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
	
	if(old_state != new_state);{
	Ultrachanged = 1;
	savedata = 1;
    old_state = new_state;
	}
	if(count < 1){ //corrects out of bounds values
		count = 1;
		savedata = 0;
	}
	if(count > 400){//corrects out of bounds values
		count = 400;
		savedata = 0;
	}
	//OCR2A = count; //updates the PWNs
	//OCR1B = width;
	//}
	 
}
ISR(USART_RX_vect){
	int datavalue;
	datavalue = rx_char();
	if(receivestart){ //checks if it should add the character
       data[datacnt] = datavalue;
	   datacnt++;
	}
	if(((datavalue - '<') == 0)){ //checks for the starting value
       receivestart = 1;
	   datacnt = 0;
	   receivecomplete = 0;
	}
	if((((datavalue - '>') == 0) || datacnt > 3) && receivestart){ //checks if it is out of room or if the final character is sent
       receivestart = 0;
	   data[datacnt] = '\0';
	   receivecomplete = 1;
	}
	
}

void timer0_init(void)
{
    TCCR0A |= (1 << WGM01); //enables timer0 for the buzzer
    TIMSK0 |= (1 << OCIE0A);
}
void timer1_init(void)
{
	TCCR1B &= ~((1 << WGM10) | (1 << WGM11));
    TCCR1B &= ~((1 << CS10) | (1 << CS11) | (1 << CS12)); //sets prescalar to 0
    TIMSK1 |= (1 << OCIE1A);
	OCR1A = 46400;
    TCNT1 = 0;
    
}
void timer2_init(void)
{
    TCCR2A |= (1 << WGM20) | (1 << WGM21); // Fast PWM mode, modulus = 256
	TCCR2A |= (0b10 << COM2A0); // Turn D11 on at 0x00 and off at OCR2A
	OCR2A = 23;                // Initial pulse duty cycle of 50%
    TCCR2B |= (0b111 << CS20);  // Prescaler = 1024 for 16ms period
}

void debouncePC(uint8_t bit)
{
    // Add code to debounce input "bit" of PINC
    // assuming we have sensed the start of a press.
    _delay_ms(5);
    while((PINC & (1<< bit)) == 0){ //checks while held down
       
    }
	_delay_ms(5);
    
}
void debouncePB(uint8_t bit)
{
    // Add code to debounce input "bit" of PINC
    // assuming we have sensed the start of a press.
    _delay_ms(5);
    while((PINB & (1<< bit)) == 0){ //checks while held down
      
    }
	_delay_ms(5);
    
}

void transmitData(void){
	char str[5];
	snprintf(str, 5, "%3d%01d", distance, distdecimal);
	UDR0 = '<';//sends the start bit
	int j = 0;
	while(str[j] != '\0'){
	    while ((UCSR0A & (1 << UDRE0)) == 0) { }
        UDR0 = str[j]; //iterates through the 
		j++;
	}
	while ((UCSR0A & (1 << UDRE0)) == 0) { }
    UDR0 = '>';
}
char rx_char(void)
{
// Wait for receive complete flag to go high
 while ( !(UCSR0A & (1 << RXC0)) ) {}
 return UDR0;
}

