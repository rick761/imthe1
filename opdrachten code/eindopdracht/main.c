#include <avr/io.h>
#include <util/delay.h>
#include "main.h"
#define F_CPU 16000000L // run CPU at 16 MHz
#include "ds18b20_library.h"
#include "hc595_library.h"

volatile int graden = 0;         //wordt aangepast door de ds18b20..
volatile int graden_tiental = 0; //graden wordt opgesplitst
volatile int graden_decimal =0;  // --



volatile int slowdown_timer = 0;
volatile int tientallen_switch =0;

ISR(TIMER2_OVF_vect) {
	slowdown_timer++;
	if(slowdown_timer == 100){
		if(tientallen_switch == 0 ){ schrijfGetal(graden_tiental,0); tientallen_switch++; }
		else if(tientallen_switch == 1 ){ schrijfGetal(graden_decimal,1); tientallen_switch = 0;}
		slowdown_timer =0;
	}
}

void deelGradenOpInTweeVariabelen(){
	uint8_t positie = 0;
	int value = graden;
	while (value > 0) {
	 int digit = value % 10;
	 if(positie == 0){graden_tiental = digit;}
	 if(positie == 1){graden_decimal = digit;}
	 positie++;
	 value /= 10;
	}
}

int main( )
{
	//initiation
	initUSART(); //usart for debugging
	DDRC = 0x00; //PORTC activated for TEMP sensor 18B20

	TCCR2B |= (1 << CS00); // prescaler wordt geinstantieerd
	TIMSK2 |= (1 << TOIE2);//TimerMask register enabled op TOIE2. (timer 2)
	sei();				   //zet interrupts aan.

	REG_INPUTInit();       //initiate functie van de shift_register_output
	REG_OUTPUTInit();	   //inituate functie van de shift_reguster_input

	while ( 1 ){
		geefCelcius(&graden); //haalt de graden op uit de ds18b20 module.
		deelGradenOpInTweeVariabelen(); //deelt de vragen op in 2 variabelen voor de display

		printByte(graden);     //USART DEBUG functie (extra module)
		printString(". \n");   // -
	}
	return 0;
}
