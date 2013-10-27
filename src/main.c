
#include <avr/interrupt.h>
#include <util/delay.h>

void sleep_ms(uint16_t ms);

/* LED blinking when interrupt arrives */
ISR(INT1_vect)
{
    PORTH |= (1 << PH4);
    sleep_ms(500);
    PORTH &= ~(1 << PH4);
}

int
main(void)
{
    /* set PH4 (digital pin 7) to output (= 1) */
    DDRH |= (1 << PH4);
    
    /* set PD1 (digital pin 20) to input (= 0) */
    DDRD &= ~(1 << PD1);
    
    EIMSK = (1 << INT1);                /* EIMSK – External Interrupt Mask Register:
                                                   1 = external pin interrupt is enabled, 0 = disabled
                                                   
                                           Bit   7      6              1      0
                                               ___________________________________
                                              |      |      |       |  *   |      |
                                              | INT7 | INT6 | [...] | INT1 | INT0 |
                                              |______|______|_______|______|______|
                                        */
                                        
    EICRA = (1 << ISC11 | 1 << ISC10);  /* EICRA – External Interrupt Control Register A
                                                   0  0  =  low-level generates interrupt
                                                   0  1  =  any edge generates interrupt
                                                   1  0  =  falling edge generates interrupt
                                                   1  1  =  rising edge generates interrupt
                                                   
                                           Bit            3       2       1       0
                                               _______________________________________
                                              |       |   *   |   *   |       |       |
                                              | [...] | ISC11 | ISC10 | ISC01 | ISC00 |
                                              |_______|_______|_______|_______|_______|
                                        */
    
    sei();                              /* SREG – AVR Status Register:
                                                  I-bit in the Status Register (SREG) is set (one)
                                                  Bit 7 – I: Global Interrupt Enable
                                                  Bit 6 – T: Bit Copy Storage
                                                  Bit 5 – H: Half Carry Flag
                                                  Bit 4 – S: Sign Bit
                                                  Bit 3 – V: Two’s Complement Overflow Flag
                                                  Bit 2 – N: Negative Flag
                                                  Bit 1 – Z: Zero Flag
                                                  Bit 0 – C: Carry Flag
                                                  
                                           Bit  7   6   5   4   3   2   1   0
                                               _______________________________
                                              | * |   |   |   |   |   |   |   |
                                              | I | Z | H | S | V | N | Z | C |
                                              |___|___|___|___|___|___|___|___|
                                        */
    
    while (1) {
        //
    }
}

void
sleep_ms(uint16_t ms)
{
    while (ms--) {
        _delay_ms(1);
    }
}

