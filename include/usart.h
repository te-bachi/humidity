#ifndef __USART_H__
#define __USART_H__

#include <stdint.h>
#include <stdbool.h>

void usart0_init();
void usart0_putc(const char c);
void uart0_puts(const char *s);

#endif

