//#include <avr/io.h>

//#include <util/delay.h>

#include <avr/io.h>
#include <util/delay.h>
/**/
#define THERM_PORT PORTC
#define THERM_DDR DDRC
#define THERM_PIN PINC
#define THERM_IO 0

#define ClearBit(x,y) x &= ~_BV(y) // equivalent to cbi(x,y)
#define SetBit(x,y) x |= _BV(y) // equivalent to sbi(x,y)
#define ReadBit(x,y) x & _BV(y) // call with PINx and bit#

#define THERM_INPUT() ClearBit(THERM_DDR,THERM_IO) // make pin an input
#define THERM_OUTPUT() SetBit(THERM_DDR,THERM_IO) // make pin an output
#define THERM_LOW() ClearBit(THERM_PORT,THERM_IO) // take (output) pin low
#define THERM_HIGH() SetBit(THERM_PORT,THERM_IO) // take (output) pin high
#define THERM_READ() ReadBit(THERM_PIN,THERM_IO) // get (input) pin value

/* alle soorten modi die van deze sensor beschikbaar zijn*/
#define THERM_CONVERTTEMP 0x44
#define THERM_READSCRATCH 0xBE
#define THERM_WRITESCRATCH 0x4E
#define THERM_COPYSCRATCH 0x48
#define THERM_READPOWER 0xB4
#define THERM_SEARCHROM 0xF0
#define THERM_READROM 0x33
#define THERM_MATCHROM 0x55
#define THERM_SKIPROM 0xCC
#define THERM_ALARMSEARCH 0xEC

/*DE TE GEBRUIKEN ROM IS TE VINDEN ALS JE DE ROMREADERPROGRAM FUNCTIE UITVOER*/
uint8_t huidige_rom[] = {0x28,0x5A,0x63,0x95,0x05,0x00,0x00,0xF5 };

uint8_t therm_Reset() // reset function van de stand van de sensor
{
	uint8_t  i;
	THERM_OUTPUT(); // zet pins als output
	THERM_LOW(); // trek pin laag for 48 0microsecondes
	_delay_us(480);
	THERM_INPUT(); // zet de pins als input
	_delay_us(60); // wacht 60 microsecondes
	i = THERM_READ(); // geef pin waarde
	_delay_us(420); // wachf 480 microsecondes
	return i;
}
void therm_WriteBit(uint8_t bit)
{
	THERM_OUTPUT(); // zet pin als output
	THERM_LOW(); // zet pin laag voor 1 microseconden
	_delay_us(1);
	if (bit) THERM_INPUT(); //schrijf en release
	_delay_us(60);
	THERM_INPUT(); // wait 60uS & release pin
}

uint8_t therm_ReadBit()
{
	uint8_t bit=0;
	THERM_OUTPUT(); // zet pin als output
	THERM_LOW(); // trek pin laag for 1microsec
	_delay_us(1);
	THERM_INPUT(); // release pin & wacht 14 microsec
	_delay_us(14);
	if (THERM_READ()) bit=1; // lees pin value
	_delay_us(45); // wacht voor 60microsecs
	return bit;
}

void therm_WriteByte(uint8_t data) // functie die 8 keer writeBit schrijft
{
	uint8_t i=8;
	while(i--){ // for 8 bits:
		therm_WriteBit(data&1);
		data >>= 1; // shift all bits right
    }
}
uint8_t therm_ReadByte() // functie die 8 keer een bit uitleest
{
	uint8_t i=8, data=0;
	 while(i--){
		 data >>= 1; // sbitshift
		 data |= (therm_ReadBit()<<7); // krijg volgende bit (LSB first)
	 }
	 return data;
}

void therm_MatchRom(uint8_t rom[]){
	therm_WriteByte(THERM_MATCHROM); //gaat naar de match-rom modus
	for (uint8_t i=0;i<8;i++)
		therm_WriteByte(rom[i]); //voert het rom 64-bit signaal in
}

void leesRauweTemperatuurData(uint8_t id[], uint8_t *t0, uint8_t *t1){
	therm_Reset(); // skip ROM & start temp conversion
	 if (id) therm_MatchRom(id);
	 else therm_WriteByte(THERM_SKIPROM);
	 therm_WriteByte(THERM_CONVERTTEMP);
	 while (!therm_ReadBit()); // wait until conversion completed

	 therm_Reset(); // read first two bytes from scratchpad
	 if (id) therm_MatchRom(id);
	 else therm_WriteByte(THERM_SKIPROM);
	 therm_WriteByte(THERM_READSCRATCH);
	 *t0 = therm_ReadByte(); // first byte
	 *t1 = therm_ReadByte(); // second byte

}

uint8_t RomReaderProgram(int i){ //
	 therm_Reset();
	 therm_WriteByte(THERM_READROM); // zet de rom-modus aan, waardoor de sensor zijn 64bit rom code zal returnen
	 uint8_t data[8]; //instantieert een array
	 for (uint8_t i=0; i<8; i++){
		 data[i] = therm_ReadByte();
	 }
	 return data[i]; // geeft gewenste stuk van de array terug
}

uint8_t geefCelcius(int *getal_heel, int *decimal){
	/*uint8_t t0,t1;
	leesRauweTemperatuurData(huidige_rom,&t0,&t1);
	*getal_heel = (t1 & 0x07) << 4; // grab lower 3 bits of t1
	*getal_heel |= t0 >> 4; // and upper 4 bits of t0
	*/
	uint8_t t0,t1;
	leesRauweTemperatuurData(huidige_rom,&t0,&t1); // get temperature values
	 *getal_heel = (t1 & 0x07) << 4; // grab lower 3 bits of t1
	 *getal_heel |= t0 >> 4; // and upper 4 bits of t0
	 *decimal = t0 & 0x0F; // decimals in lower 4 bits of t0
	 *decimal *= 625; // conversion factor for 12-bit resolution
	 return getal_heel;

}
