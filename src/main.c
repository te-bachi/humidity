#include "humidity.h"
#include "seg7.h"
#include "util.h"

#include <avr/interrupt.h>

ISR(INT1_vect)
{
    PORTH |= (1 << PH4);
    sleep_ms(500);
    PORTH &= ~(1 << PH4);
}


int
main(void)
{
    uint8_t i;
    
    //DDRD  = (1 << PD1);
    //PORTD = (1 << PD1);
    DDRH    = (1 << PH4);
    
    EIMSK = (1 << INT1);
    EICRA = (1 << ISC11 | 1 << ISC10);
    
    sei();
    
    seg7_init();
    
    while (1) {
        for (i = 0; i <= 99; i++) {
            seg7_output(i);
            sleep_ms(500);
        }
    }
}
