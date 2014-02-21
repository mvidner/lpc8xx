#ifndef _WPROGRAM_H
#define _WPROGRAM_H

#include <stdint.h>
#define abs(x) ((x) < 0 ? -(x) : (x))
void *memset(void *s, int c, uint32_t n);

//#include <stdlib.h>
//#include <string.h>
typedef bool boolean;
#define HIGH 1
#define LOW  0
#define _BV(bit) (1<<(bit))
class Print {
};

enum mode { INPUT, OUTPUT, INPUT_PULLUP };
enum endianness { LSBFIRST, MSBFIRST };
void pinMode(int pin, mode m);
void digitalWrite(int pin, int value);
void _delay_ms(int ms);
uint8_t * portOutputRegister(int port);
int digitalPinToPort(int pin);
uint8_t digitalPinToBitMask(int pin);
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);

#endif
