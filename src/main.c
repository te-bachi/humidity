
#include <avr/interrupt.h>
#include <util/delay.h>

#include "util.h"
#include "usart.h"

#include "mirf.h"
#include "nRF24L01.h"

#define NRF24_PORT          PORTB
#define NRF24_PORT_MISO     PB3
#define NRF24_PORT_MOSI     PB2
#define NRF24_PORT_SCK      PB1
#define NRF24_PORT_CSN      PB0
#define NRF24_PORT_CE       PB4

#define NRF24_DDR           DDRB
#define NRF24_DDR_MISO      DDB3
#define NRF24_DDR_MOSI      DDB2
#define NRF24_DDR_SCK       DDB1
#define NRF24_DDR_CSN       DDB0
#define NRF24_DDR_CE        DDB4

#define SEG7_ONE            PORTA
#define SEG7_ONE_DDR        DDRA
#define SEG7_TEN            PORTC
#define SEG7_TEN_DDR        DDRC 

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
    0x6f,   // 9
    0x40,   // A
    0x40,   // B
    0x40,   // C
    0x40,   // D
    0x40,   // E
    0x40    // F
};

void seg7_output(uint8_t bin); 
static void num2hexstr(uint8_t *num, uint8_t size, uint8_t *str, uint8_t len);

int
main(void)
{
    uint8_t tx_addr_reg[5];
    uint8_t rx_addr_p1_reg[5];
    uint8_t str[10+1];
    
    /* NRF24 output (= 1) */
    NRF24_DDR |= (1 << NRF24_DDR_MOSI) | (1 << NRF24_DDR_SCK) |(1 << NRF24_DDR_CSN) |(1 << NRF24_DDR_CE);
    
    /* NRF24 input (= 0) */
    NRF24_DDR &= ~(1 << NRF24_DDR_MISO);
    
    /* SPCR – SPI Control Register:
       Bit 7 – SPIE: SPI Interrupt Enable
       Bit 6 – SPE:  SPI Enable
       Bit 5 – DORD: Data Order (0 = LSB first, 1 = MSB first)
       Bit 4 – MSTR: Master/Slave Select (0 = Slave, 1 = Master)
       Bit 3 – CPOL: Clock Polarity (idle: 0 = clock low, 1 = clock high)
       Bit 2 – CPHA: Clock Phase (0 = sample on leading clock, 1 = sample on trailing clock)
       Bit 1 – SPR1: SPI Clock Rate Select 1
       Bit 0 – SPR0: SPI Clock Rate Select 0     
       SPI2X  SPR1  SPR0
       0      0     0  =  Frequency/4
       0      0     1  =  Frequency/16
       0      1     0  =  Frequency/64
       0      1     1  =  Frequency/128
       
       1      0     0  =  Frequency/2
       1      0     1  =  Frequency/8
       1      1     0  =  Frequency/32
       1      1     1  =  Frequency/64
       
       Bit   7      6      5      4      3      2      1      0
           _______________________________________________________
          |      |      |      |      |      |      |      |      |
          | SPIE | SPE  | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0 |
          |______|______|______|______|______|______|______|______|
    */
    
    SPCR = ((0 << SPIE) |
            (1 << SPE)  |
            (0 << DORD) |
            (1 << MSTR) |
            (0 << CPOL) |
            (0 << CPHA) |
            (0 << SPR1) |
            (1 << SPR0));
                                        
    /* SPSR - SPI Status Register
       Bit 7 – SPIF:  SPI Interrupt Flag
       Bit 6 – WCOL:  Write Collision Flag, when writing to SPDR during a data transfer
       Bit 0 – SPI2X: Double SPI Speed Bit
              
               
       Bit   7      6      5      4      3      2      1      0
           ________________________________________________________
          |      |      |      |      |      |      |      |       |
          | SPIF | WCOL |  -   |  -   |  -   |  -   |  -   | SPI2X |
          |______|______|______|______|______|______|______|_______|
    */
    SPSR = (1 << SPI2X);
    
    
    usart0_init();
    
    /*
    SEG7_ONE_DDR = 0xff;
    SEG7_TEN_DDR = 0xff;
    
    seg7_output(00);
    sleep_ms(400);
    
    
    sleep_ms(400);
    
    seg7_output(tx_addr_reg[0]);
    sleep_ms(1000);
    seg7_output(tx_addr_reg[1]);
    sleep_ms(500);
    seg7_output(tx_addr_reg[2]);
    sleep_ms(500);
    seg7_output(tx_addr_reg[3]);
    sleep_ms(500);
    seg7_output(tx_addr_reg[4]);
    sleep_ms(500);
    */
    sleep_ms(500);
    while (1) {
        /* Read TX_ADDR */
        mirf_read_register(TX_ADDR, tx_addr_reg, sizeof(tx_addr_reg));
        num2hexstr(tx_addr_reg, sizeof(tx_addr_reg), str, sizeof(str));
        
        uart0_puts((char *) str);
        uart0_puts("\r\n");
        sleep_ms(500);
        
        /* Read RX_ADDR_P1 */
        mirf_read_register(RX_ADDR_P1, rx_addr_p1_reg, sizeof(rx_addr_p1_reg));
        num2hexstr(rx_addr_p1_reg, sizeof(rx_addr_p1_reg), str, sizeof(str));
        
        uart0_puts((char *) str);
        uart0_puts("\r\n");
        sleep_ms(500);
        //
    }
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

static void
num2hexstr(uint8_t *num, uint8_t size, uint8_t *str, uint8_t len)
{
    const static uint8_t HEXADECIMAL_LOWER[] = "0123456789abcdef";
    
    uint8_t     tmp;                        /* temporaere variable fuer den num-array */
    uint8_t    *reverse = &(str[len - 1]);  /* pointer auf das letzte zeichen */
    uint16_t    i = 0;
    
    str[len - 1] = '\0';
    
    /* uebersetze array von 8-bit nummer in zwei zeichen pro nummer */
    for (i = 0; size > 0; i++, size--) {
        tmp = num[i];
        *--reverse = HEXADECIMAL_LOWER[tmp & 0x0f];
        tmp >>= 4;
        *--reverse = HEXADECIMAL_LOWER[tmp & 0x0f];
    }
    
    /* fuelle mit nullen auf am linken rand: ex. 000001234\0 */
    for (;i < len; i++) {
        *--reverse = '0';
    }
}

