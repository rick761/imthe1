
										//include van librarys voor de code
#include <avr/io.h>
#include <util/delay.h>



int main (void)
{
										// pin setup
    DDRB = 0xff;
    DDRC = 0xff;
	DDRD = 0xff;
										//variable setup
	int i = 0;
	int j = 0;
	const int iMax = 9;
	const int jMax = 9;
	const int delayTime = 100;

	void setCijfer(int cijfer1, int cijfer2){

										//functie om het eerste cijfer
										//en
										//het tweede cijder in de 7 led display te veranderen.

		switch(cijfer1){ 				//switch voor led display 1
		case 0: 						//voor cijfer 0
			PORTB = 0b11001000;			//binaire code voor de output port b
			PORTD &= ~(1 << 2);			//Bitshifting van 1 bit.
			break;
		case 1:							// voor cijfer 1
			PORTB = 0b11111011;
			PORTD &= ~(1 << 2);
			break;
		case 2:							//etc..
			PORTB = 0b11000001;
			PORTD |= (1 << 2);
			break;
		case 3:
			PORTB = 0b11010001;
			PORTD &= ~(1 << 2);
			break;
		case 4:
			PORTB = 0b11110010;
			PORTD &= ~(1 << 2);
			break;
		case 5:
			PORTB = 0b11010100;
			PORTD &= ~(1 << 2);
			break;
		case 6:
			PORTB = 0b11000100;
			PORTD &= ~(1 << 2);
			break;
		case 7:
			PORTB = 0b11111001;
			PORTD &= ~(1 << 2);
			break;
		case 8:
			PORTB = 0b11000000;
			PORTD &= ~(1 << 2);
			break;
		case 9:
			PORTB = 0b11010000;
			PORTD &= ~(1 << 2);
			break;
		}

		switch(cijfer2){					//switch voor cijfer 2
		case 0:								//als het cijfer 0 moet zijn
			PORTC = 0b11001000;				//Port c binaire waarden wordt anders
			PORTD &= ~(1 << 3);				//Port D bitshiften omdat het maar 1 getal mag veranderen
			break;
		case 1:								// als het cijfer 1 is etc...
			PORTC = 0b11111011;
			//PORTD = 0b11110111;
			PORTD &= ~(1 << 3);
			break;
		case 2:
			PORTC = 0b11000001;
			PORTD |= (1 << 3);
			break;
		case 3:
			PORTC = 0b11010001;
			//PORTD = 0b11110111;
			PORTD &= ~(1 << 3);
			break;
		case 4:
			PORTC = 0b11110010;
			//PORTD = 0b11110111;
			PORTD &= ~(1 << 3);
			break;
		case 5:
			PORTC = 0b11010100;
			PORTD &= ~(1 << 3);
			break;
		case 6:
			PORTC = 0b11000100;
			PORTD &= ~(1 << 3);
			break;
		case 7:
			PORTC = 0b11111001;
			PORTD &= ~(1 << 3);
			break;
		case 8:
			PORTC = 0b11000000;
			PORTD &= ~(1 << 3);
			break;
		case 9:
			PORTC = 0b11010000;
			PORTD &= ~(1 << 3);
			break;
		}

	}

    while (1) { 					//zichzelf herhalen met een while 1
        _delay_ms(delayTime); 		//delay volgens variabelen
        setCijfer(i,j);				//functie wordt uitgevoerd met de telbare variabelen
        j++;						//als er één actie is uitgevoerd dan telt hij j + 1.
        if(j>jMax){					//als j  zijn maximaal heeft bereikt
        	i++;					//tel i + 1 op
        	if(i>iMax){				//als i zijn max heeft gehaald
        		i=0;				//zet hem weer op 0
        	}
        	j=0;
        }
    }
    return 0;
}
