//include van librarys voor code
#include <avr/io.h>
#include <util/delay.h>

int main (void)
{
    // alle pins worden op output gezet.
    DDRB = 0xFF;

    while (1) { // zichzelf herhalen met een while 1

        PORTB = 0x0000001; //laatste pin van port b staat op aan
        _delay_ms(1000); //delay 1s
        PORTB = 0x0000010; //1-na laatste pin van port b staat op aan!
        _delay_ms(1000); //delay 1s
    }
    return 0;
}
