// includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//defines
#define F_CPU 16000000UL         //kloksnelheid processor
#define BAUD 9600        // baudrate
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1) // prescaler

void main() {
	//interrupt initiate
    UCSR0B =(1 << RXCIE0) | (1 << RXEN0);
    UCSR0C =(0 << UPM01) |(0 << UPM00) | (0 << UMSEL01) |(0 << UMSEL00) |(0 << USBS0) |(1 << UCSZ01) |(1 << UCSZ00);
   	sei();

	//init usart
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);

	DDRB = 0xFF; //init port B
	PORTB = 0xFF;
	while (1) {

	}
}


ISR(USART_RX_vect)
{
	switch (UDR0){
		case 'r':
			PORTB ^= (1<<3);
			break;
		case 'g':
			PORTB ^= (1<<4);
			break;
		case 'b':
			PORTB ^= (1<<2);
			break;
	}
}
