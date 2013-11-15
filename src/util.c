#include "util.h"

#include <util/delay.h>

void
sleep_ms(uint16_t ms)
{
    while (ms--) {
        _delay_ms(1);
    }
}

