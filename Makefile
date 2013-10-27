
#ARDUINO_ROOT                = /c/arduino-1.0.4
ARDUINO_ROOT                = /home/andreas/arduino-1.0.5
PORT                        = /dev/ttyACM0
DEVICE                      = atmega2560
CPU_FREQ                    = 16000000L

PROGRAMS                    = humidity

### ARDUINO ###################################################################

humidity_SOURCE             = main.c seg7.c humidity.c util.c spi.c mirf.c
                              
humidity_CFLAGS             = 
humidity_LDFLAGS            = 

include Makefile.inc

