#include "seg7.h"

#include <avr/io.h>

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

void
seg7_init(void)
{
    SEG7_ONE_DDR = 0xff;
    SEG7_TEN_DDR = 0xff;
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
    
    SEG7_ONE = ~seg7_lut[bcd & 0x0f];
    SEG7_TEN = ~seg7_lut[bcd >> 4];
}

