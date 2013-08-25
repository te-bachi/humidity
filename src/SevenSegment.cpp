#include "SevenSegment.h"

using namespace huminity;

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

SevenSegment::SevenSegment(uint8_t *port)
{
    _port = port;
}

void SevenSegment::display(uint8_t digit)
{
    *_port = seg7_lut[digit];
}

