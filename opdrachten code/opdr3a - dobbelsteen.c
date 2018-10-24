/*
 * main.c
 *
 *  Created on: 12 Sep 2018
 *      Author: Rick
 */

//includes
#include <avr/io.h>
#include <util/delay.h>

//debounce functie
uint8_t debounce(void){
	if(bit_is_clear(PINC,PC0)){
		_delay_us(1000);
		if(bit_is_clear(PINC,PC0)){
			return (1);
		}
	}
	return (0);
}

//dobbelsteen functie
void setNumber(int nmbr){
	//PORTB = 0b00011111;
	//PORTD = 0b11111100;
	switch(nmbr){
	case 1:
		PORTB = 0b00000010;
		PORTD = 0b00000000;
		break;
	case 2:
		PORTB = 0b00000000;
		PORTD = 0b10100000;
		break;
	case 3:
		PORTB = 0b00000010;
		PORTD = 0b10100000;
		break;
	case 4:
		PORTB = 0b00010000;
		PORTD = 0b10110000;
		break;
	case 5:
		PORTB = 0b00010010;
		PORTD = 0b10110000;
		break;
	case 6:
		PORTB = 0b00011001;
		PORTD = 0b10110000;
		break;
	case 7:
		PORTB = 0b00011011;
		PORTD = 0b10110000;
		break;
	case 8:
		PORTB = 0b00011101;
		PORTD = 0b11110000;
		break;
	case 9:
		PORTB = 0b01011101;
		PORTD = 0b11110000;
		break;
	}
}

int main (void)
{
    //init
	uint8_t buttonpressed = 0;

	DDRB = 0b00011111; // setup 5 pinnen voor PORT B
	DDRD = 0b11110000; // setup 4 pinnen voor PORT D
	DDRC = 0x00;       // setup input    voor PORT C

	//event
    while (1) { // zichzelf herhalen met een while True loop
    	if(debounce()){  // Debounce functie volgens boek (voor indrukken knop)
    		setNumber(rand()% 9 + 1 ); // dobbelsteen functie aanroepen met randomizer meegeven
    		_delay_ms(500);
    	}
    }
    return 0;
}
