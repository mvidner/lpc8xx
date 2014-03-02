#include "main.h"
#include "adafruit/Adafruit_GFX.h"
#include "adafruit/Adafruit_PCD8544.h"
#include "src/conways_life.h"
#include "src/random.h"

#define LED1 14

volatile uint32_t msTicks = 0;

extern "C" {
void SysTick_Handler(void) {
	msTicks++;
}
}

void delay_ms(uint32_t ms) {
	uint32_t now = msTicks;
	while ((msTicks-now) < ms);
}

/*
SPI OUT       je Pin 1  PIO0_17                 SPI0_MOSI
SPI IN        je Pin 7  SWDIO/PIO0_2            SPI0_MISO
SPI Clock     je Pin 6  SWCLK/PIO0_3            SPI0_SCK
CS pro LCD    je Pin 9  PIO0_10(Open Drain)     PIO0_10  
D/C pro LCD   je Pin 10 PIO0_16                 PIO0_16  
Reset pro LCD je Pin 8  PIO0_11(Open Drain)     PIO0_11   
*/


void led(int value) {
  //    gpio_pin(value, 1 << LED1);
  //  LPC_GPIO_PORT->B0[LED1] = value;
  if (value)
    LPC_GPIO_PORT->SET0 = 1 << LED1;
  else
    LPC_GPIO_PORT->CLR0 = 1 << LED1;
}


extern "C" {
extern void SwitchMatrix_Init(void);
}

// !!
// http://elegantinvention.com/blog/information/smaller-binary-size-with-c-on-baremetal-g/
extern "C" void __cxa_pure_virtual() { while(1); }

double live_p = 0.2;

void random_setup(ConwaysLife &cl) {
  int LIVE_MAX = int (live_p * MY_RAND_MAX);
  for (unsigned y = 0; y < cl.size_y(); ++y) {
    for (unsigned x = 0; x < cl.size_x(); ++x) {
      cl.set(x, y, ConwaysLife::State(myrand() <= LIVE_MAX));
    }
  }
}

void showit(Adafruit_PCD8544& d, const ConwaysLife& cl) {
  for (unsigned y = 0; y < cl.size_y(); ++y) {
    for (unsigned x = 0; x < cl.size_x(); ++x) {
      d.drawPixel(x, y, cl.get(x, y));
    }
  }
  d.display();
}

ConwaysLife cl;

int main(void ) {
  SysTick_Config(SystemCoreClock / 1000);

  SwitchMatrix_Init();
  LPC_GPIO_PORT->DIR0 |= (1<<LED1);

  Adafruit_PCD8544 display(3, 17, 16, 10, 11);
  display.begin();

  mysrand(704);
  while (1) {
    random_setup(cl);

    int generations = 0;
    int period = 0;
    do {
      showit(display, cl);
      cl.next();
      ++generations;
      period = cl.stabilized();
    } while (period == 0);

    led(1);
    delay_ms(1000);
    led(0);
    delay_ms(1000);
  }
}

