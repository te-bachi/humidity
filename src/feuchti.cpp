
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define SEG7_ONE        PORTA
#define SEG7_ONE_DDR    DDRA
#define SEG7_TEN        PORTC
#define SEG7_TEN_DDR    DDRC

void sleep_ms(uint16_t ms);
void seg7_output(uint8_t bin);

#include "SevenSegment.h"

using namespace huminity;

SevenSegment one((uint8_t *)SEG7_ONE);

/*================================
 *             P0
 *          _________
 *         /        /
 *    P5  /  P6    /  P1
 *       /________/
 *      /        /
 * P4  /        /  P2
 *    /________/
 *
 *       P3
 *==============================*/
const static uint8_t seg7_lut[] = {
    0x3f,   // 0
    0x06,   // 1
    0x5b,   // 2
    0x4f,   // 3
    0x66,   // 4
    0x6d,   // 5
    0x7d,   // 6
    0x07,   // 7
    0x7f,   // 8
    0x6f    // 9
};

int
main(void)
{
    uint8_t i;
    
    SEG7_ONE_DDR = 0xff;
    SEG7_TEN_DDR = 0xff;
    
    SEG7_ONE = ~0x01;
    SEG7_TEN = ~0x01;
    sleep_ms(500);
    
    SEG7_ONE = ~0x02;
    SEG7_TEN = ~0x02;
    sleep_ms(500);
    
    SEG7_ONE = ~0x04;
    SEG7_TEN = ~0x04;
    sleep_ms(500);
    
    SEG7_ONE = ~0x08;
    SEG7_TEN = ~0x08;
    sleep_ms(500);
    
    SEG7_ONE = ~0x10;
    SEG7_TEN = ~0x10;
    sleep_ms(500);
    
    SEG7_ONE = ~0x20;
    SEG7_TEN = ~0x20;
    sleep_ms(500);
    
    SEG7_ONE = ~0x40;
    SEG7_TEN = ~0x40;
    sleep_ms(500);
    
    SEG7_ONE = ~0x80;
    SEG7_TEN = ~0x80;
    sleep_ms(500);
    
    while (1) {
        for (i = 0; i <= 99; i++) {
            seg7_output(i);
            sleep_ms(500);
        }
    }
}

void
sleep_ms (uint16_t ms)
{
    while (ms--)
        _delay_ms (1);
}

void
seg7_output(uint8_t bin)
{
    uint8_t bcd   = 0;
    uint8_t shift = 0;
    
    while (bin > 0) {
        bcd   |= (bin % 10) << shift;
        shift += 4;
        bin   /= 10;
    }
    
    
    /*
    while (bin > 9) {
        bcd += 0x10;
        bin -= 10;
    }
    */
    
    SEG7_ONE = ~seg7_lut[bcd & 0x0f];
    SEG7_TEN = ~seg7_lut[bcd >> 4];
    
    //SEG7_ONE = seg7_lut[bin];
    //SEG7_TEN = seg7_lut[bcd >> 4];
}

