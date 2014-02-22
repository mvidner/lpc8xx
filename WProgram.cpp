#include "WProgram.h"
#include "main.h"
#include "LPC8xx.h"

void pinMode(int pin, mode m) {
  if (m == OUTPUT)
    LPC_GPIO_PORT->DIR0 |=  (1 << pin);
  else
    LPC_GPIO_PORT->DIR0 &= ~(1 << pin);
}

void digitalWrite(int pin, int value) {
  LPC_GPIO_PORT->B0[pin] = value;
}

void _delay_ms(int ms) {
  delay_ms(ms);
}

uint8_t * portOutputRegister(int port) {
  return (uint8_t *)(0xA0000000UL + port);
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
