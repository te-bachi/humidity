
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <avr/interrupt.h>

#define BAUD        9600UL
#define UBRR_VAL    ((F_CPU+BAUD*8)/(BAUD*16)-1)    // clever runden
#define BAUD_REAL   (F_CPU/(16*(UBRR_VAL+1)))       // Reale Baudrate
#define BAUD_ERROR  ((BAUD_REAL*1000)/BAUD)         // Fehler in Promille, 1000 = kein Fehler.

void sleep_ms (uint16_t ms);

/*******************************************************************************
 * USART0
 ******************************************************************************/
 
void usart0_init() {
    /* USART0 Baud Rate Register */
    UBRR0H  = UBRR_VAL >> 8;
    UBRR0L  = UBRR_VAL & 0xFF;
    
    /* USART Control and Status Register B */
    UCSR0B |= (1<<TXEN0);                               // UART TX einschalten
    
    /* USART Control and Status Register C */
    UCSR0C  = (1<<UMSEL01)|(1 << UCSZ01)|(1 << UCSZ00); // Asynchron 8N1 
}

void usart0_putc(const char c)
{
    /* warten bis Senden moeglich */
    while (!(UCSR0A & (1 << UDRE0))) {
        //
    }
    UDR0 = c; 
}

void uart0_puts(const char *s)
{
    /* so lange *s != '\0' */
    while (*s) {
        usart0_putc(*s);
        s++;
    }
}

/*******************************************************************************
 * USART2
 ******************************************************************************/

void usart2_init() {
    /* USART2 Baud Rate Register */
    UBRR2H  = UBRR_VAL >> 8;
    UBRR2L  = UBRR_VAL & 0xFF;
    
    /* AVR Status Register */
    //SREG   |= (1 << I);
    sei();
    
    /* USART Control and Status Register B */
    UCSR2B |= (1 << RXCIE2);                            // UART RX Interrupt einschalten
    UCSR2B |= (1 << TXCIE2);                            // UART TX Interrupt einschalten
    //UCSR2B |= (1 << UDRIE2);                            // UART Data Empty Interrupt einschalten
    UCSR2B |= (1 << TXEN2);                             // UART TX einschalten
    
    /* USART Control and Status Register C */
    UCSR2C  = (1<<UMSEL21)|(1 << UCSZ21)|(1 << UCSZ20); // Asynchron 8N1 
}

void usart2_putc(const char c)
{
    /* warten bis Senden moeglich */
    while (!(UCSR2A & (1 << UDRE2))) {
        //
    }
    UDR2 = c; 
}

void uart2_puts(const char *s)
{
    /* so lange *s != '\0' */
    while (*s) {
        usart2_putc(*s);
        s++;
    }
}

ISR (USART2_RX_vect)
{
    uart0_puts("usart2_rx\r\n");
}

ISR (USART2_TX_vect)
{
    uart0_puts("usart2_tx\r\n");
}

ISR (USART2_UDRE_vect)
{
    uart0_puts("usart2_udre\r\n");
}
