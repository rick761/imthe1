#include <avr/io.h>
#include <util/delay.h>


/***************************************

Configure Connections

****************************************/
//define port D en B voor shift register
#define REG_INPUT_PORT    PORTB
#define REG_OUTPUT_PORT   PORTD
#define REG_INPUT_DDR    DDRB
#define REG_OUTPUT_DDR    DDRD
//define pin setups voor shift register input en output
#define REG_INPUT_DS_POS PB0      //Data pin (DS) pin location
#define REG_INPUT_SH_CP_POS PB1      //Shift Clock (SH_CP) pin location
#define REG_INPUT_ST_CP_POS PB2      //Store Clock (ST_CP) pin location

#define REG_OUTPUT_DS_POS PD2
#define REG_OUTPUT_SH_CP_POS PD3
#define REG_OUTPUT_ST_CP_POS PD4



/***************************************
Configure Connections ***ENDS***
****************************************/

//Initialize REG_INPUT System
void REG_INPUTInit(){
   //Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   REG_INPUT_DDR|=((1<<REG_INPUT_SH_CP_POS)|(1<<REG_INPUT_ST_CP_POS)|(1<<REG_INPUT_DS_POS));
}
void REG_OUTPUTInit(){
   //Make the Data(DS), Shift clock (SH_CP), Store Clock (ST_CP) lines output
   REG_OUTPUT_DDR|=((1<<REG_OUTPUT_SH_CP_POS)|(1<<REG_OUTPUT_ST_CP_POS)|(1<<REG_OUTPUT_DS_POS));
}

//Low level macros to change data (DS)lines
#define REG_INPUTDataHigh() (REG_INPUT_PORT|=(1<<REG_INPUT_DS_POS))
#define REG_OUTPUTDataHigh() (REG_OUTPUT_PORT|=(1<<REG_OUTPUT_DS_POS))

#define REG_INPUTDataLow() (REG_INPUT_PORT&=(~(1<<REG_INPUT_DS_POS)))
#define REG_OUTPUTDataLow() (REG_OUTPUT_PORT&=(~(1<<REG_OUTPUT_DS_POS)))

//Sends a clock pulse on SH_CP line
void REG_INPUTPulse(){ // library functie voor het besturen van de shift register
   //Pulse the Shift Clock
   REG_INPUT_PORT|=(1<<REG_INPUT_SH_CP_POS);//HIGH
   REG_INPUT_PORT&=(~(1<<REG_INPUT_SH_CP_POS));//LOW
}
void REG_OUTPUTPulse(){ // library functie voor het besturen van de shift register
   //Pulse the Shift Clock
   REG_OUTPUT_PORT|=(1<<REG_OUTPUT_SH_CP_POS);//HIGH
   REG_OUTPUT_PORT&=(~(1<<REG_OUTPUT_SH_CP_POS));//LOW
}

//Sends a clock pulse on ST_CP line
void REG_INPUTLatch(){ // library functie voor het besturen van de shift register
   //Pulse the Store Clock
   REG_INPUT_PORT|=(1<<REG_INPUT_ST_CP_POS);//HIGH
   _delay_loop_1(1);
   REG_INPUT_PORT&=(~(1<<REG_INPUT_ST_CP_POS));//LOW
   _delay_loop_1(1);
}
void REG_OUTPUTLatch(){
   //Pulse the Store Clock
   REG_OUTPUT_PORT|=(1<<REG_OUTPUT_ST_CP_POS);//HIGH
   _delay_loop_1(1);
   REG_OUTPUT_PORT&=(~(1<<REG_OUTPUT_ST_CP_POS));//LOW
   _delay_loop_1(1);
}


void REG_INPUTWrite(uint8_t data){ //library code van de hc595 shift register om te schrijfen naar de led matrix
   for(uint8_t i=0;i<8;i++){
      if(data & 0b10000000){
         REG_INPUTDataHigh();//MSB is 1 so output high
      }
      else{
         REG_INPUTDataLow(); //MSB is 0 so output high
      }
      REG_INPUTPulse();  //Pulse the Clock line
      data=data<<1;  //Now bring next bit at MSB position
   }

   REG_INPUTLatch();
}

void REG_OUTPUTWrite(uint8_t data){ //library code van de hc595 shift register om te schrijfen naar de led matrix
   for(uint8_t i=0;i<8;i++){
      if(data & 0b10000000){
         REG_OUTPUTDataHigh();//MSB is 1 so output high
      }
      else{
         REG_OUTPUTDataLow(); //MSB is 0 so output high
      }
      REG_OUTPUTPulse();  //Pulse the Clock line
      data=data<<1;  //Now bring next bit at MSB position
   }
   REG_OUTPUTLatch();
}



volatile int tick = 0; // tick wordt gebruikt voor het eenmalig weergeven van 1 rij per aanroeping v/d functie
void schrijfGetal(int getal,int positie){
	uint8_t kolom[8] = { //input positie kolom gebruikt voor de switch
			0, //positie 0 = empty
			0b00000001, //1
			0b00000010, //2
			0b00000100, //3
			0b00001000, //4
			0b00010000, //5
			0b00100000, //6
			0b01000000, //7
			0b10000000  //8
	};

	switch(positie){ //positie van getal
		case 0://eerste gedeelte
			if(getal == 0){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b10000001 );}  //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000001 );} //rij  3
				if(tick == 2){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b10111101 );}  //rij 2
			}if(getal == 1){
				if(tick == 0 || tick==1 || tick==2){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b10000001 );}  //rij 1
			}if(getal == 2){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b10011011 );}   //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b10101101 );} //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10110011 );}  //rij 3
			}if(getal == 3){
				if(tick == 0){REG_INPUTWrite(  kolom[5]  ); REG_OUTPUTWrite( 0b10110101 );}  //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6]  ); REG_OUTPUTWrite( 0b10110101 );}  //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3

			}if(getal == 4){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b11110001 );}  //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b11110111 );}  //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000001 );}  //rij 3
			} if(getal == 5){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b10110001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000101 ); } //rij 3
			}if(getal == 6 ){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b10001101 );  }//rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b10101101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			} if(getal == 7 ){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b11111101 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b11110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			}if(getal == 8 ){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			}if(getal == 9 ){
				if(tick == 0){REG_INPUTWrite(  kolom[5] ); REG_OUTPUTWrite( 0b10110001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[6] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[7] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			}
		break;
		case 1://tien-tallen
			if(getal == 0){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
				if(tick == 2){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10111101 ); } //rij 2
			}if(getal == 1){
				if(tick == 0 || tick == 1 || tick==2 ){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 2
			}if(getal == 2){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b10011011 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10101101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10110011 ); } //rij 3
			}if(getal == 3){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 1 en 2
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			}if(getal == 4){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b11110001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b11110111 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			} if(getal == 5){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b10110001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000101 ); } //rij 3
			} if(getal == 6 ){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b10001101 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10101101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			} if(getal == 7 ){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b11111101 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b11110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			}if(getal == 8 ){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			}if(getal == 9 ){
				if(tick == 0){REG_INPUTWrite(  kolom[1] ); REG_OUTPUTWrite( 0b10110001 ); } //rij 1
				if(tick == 1){REG_INPUTWrite(  kolom[2] ); REG_OUTPUTWrite( 0b10110101 ); } //rij 2
				if(tick == 2){REG_INPUTWrite(  kolom[3] ); REG_OUTPUTWrite( 0b10000001 ); } //rij 3
			}
		break;
	}
	tick++;
	if (tick == 3) tick = 0;
}




