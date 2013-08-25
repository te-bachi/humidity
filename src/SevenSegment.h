#ifndef __SEVEN_SEGMENT_H__
#define __SEVEN_SEGMENT_H__

#include <stdint.h>

namespace huminity {
    class SevenSegment {
        private:
            uint8_t *_port;
            
        public:
            SevenSegment(uint8_t *port);
            void display(uint8_t digit);
    };
};

#endif

