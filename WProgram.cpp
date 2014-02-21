#include "WProgram.h"
#include "main.h"
#include "LPC8xx.h"

void *memset(void *s, int c, uint32_t n) {
  uint8_t * p = (uint8_t *) s;
  while (n--) {
    *p++ = c;
  }
  return s;
}

void pinMode(int pin, mode m) {
  if (m == OUTPUT)
    LPC_GPIO_PORT->DIR0 |=  (1 << pin);
  else
    LPC_GPIO_PORT->DIR0 &= ~(1 << pin);
}

void gpio_pin(int value, uint32_t mask) {
    if (value)
	LPC_GPIO_PORT->SET0 = mask;
    else
	LPC_GPIO_PORT->CLR0 = mask;
}

void digitalWrite(int pin, int value) {
  gpio_pin(value, 1UL << pin);
}

void _delay_ms(int ms) {
  delay_ms(ms);
}

uint8_t * portOutputRegister(int port) {
  return (uint8_t *)(0xA000000 + port);
}

int digitalPinToPort(int pin) {
  return pin;
}

uint8_t digitalPinToBitMask(int /*pin*/) {
  return 1;
}

/*
void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
  
}
*/
