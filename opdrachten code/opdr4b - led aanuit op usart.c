// includes
#include <avr/io.h>
#include <util/delay.h>

//defines
#define F_CPU 16000000UL         //kloksnelheid processor
#define BAUD 9600        // baudrate
#define BAUD_PRESCALLER (((F_CPU / (BAUD * 16UL))) - 1) // prescaler


//functions
void USART_send( unsigned char data){
 while(!(UCSR0A & (1<<UDRE0)));
 UDR0 = data;
}

unsigned char USART_receive(void){
 while(!(UCSR0A & (1<<RXC0)));
 return UDR0;
}

void main() {
	//init usart
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = ((1<<UCSZ00)|(1<<UCSZ01));

	DDRB = 0xff; //init port B
	char aan = '1';

	while (1) {
		unsigned char c = USART_receive(); //ontvang de char (code wacht hier op)
		if (c != '\0'){					   //als hij niet null is
			if (c == aan){                 //if voor lamp aan
				PORTB=0xFF;
				USART_send('o');
			} else {                       //else voor lamp uit
				PORTB=0x00;
				USART_send('x');
			}
		}
	}
}

