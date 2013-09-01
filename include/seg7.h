#ifndef __SEG7_H__
#define __SEG7_H__

#include <stdint.h>

#define SEG7_ONE        PORTA
#define SEG7_ONE_DDR    DDRA
#define SEG7_TEN        PORTC
#define SEG7_TEN_DDR    DDRC

void seg7_init(void);
void seg7_output(uint8_t bin);

#endif

