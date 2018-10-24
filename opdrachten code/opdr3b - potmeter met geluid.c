/**
//usart include
#include "main.h"
**/
#include <avr/io.h>
#include <util/delay.h>

static inline void initTimer(void){
	TCCR0A |= ( 1 << WGM01 );
	TCCR0A |= ( 1 << COM0A0 );
    TCCR0B |= ( 1 << CS00 ) | ( 1 << CS01 );
}

static inline void initADC0(void) {
	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS1) | (1 << ADPS0);
	ADCSRA |= (1 << ADEN);
}

int main(void) {
  /*
  //usart serial communication
  initUSART();
  printString("OK");
  */
  // -------- Inits --------- //
   DDRD = (1 << PD6);
   initADC0();
   initTimer();

   while (1) {
	   ADCSRA |= (1 << ADSC);                     /* start ADC conversion */
	   loop_until_bit_is_clear(ADCSRA, ADSC);          /* wait until done */
	   OCR0A = ADC/4;
	   _delay_ms(10);

	 /*
     //gebruik van USART.c
     printByte(ADC/4);
	 printString("  -- \r\n");
`	 */
  }
  return (0);
}
