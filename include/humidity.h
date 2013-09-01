#ifndef __HUMIDITY_H__
#define __HUMIDITY_H__

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t     humidity;
    int16_t     temperature;
} humidity_t;

void humidity_init(void);
bool humidity_read(humidity_t *humidity);

#endif

