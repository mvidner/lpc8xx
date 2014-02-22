#include "main.h"
#include "adafruit/Adafruit_GFX.h"
#include "adafruit/Adafruit_PCD8544.h"

#define LED1 14

/*
 * 	datasheet: http://www.nxp.com/documents/data_sheet/LPC81XM.pdf
 *  user manual: http://www.nxp.com/documents/user_manual/UM10601.pdf
 *  swtich matrix tool: http://www.lpcware.com/content/nxpfile/nxp-switch-matrix-tool-lpc800 (for pin configuration)
 *
 *
 *  ISP: When pin PIO0_1 is pulled LOW on reset, the part enters ISP mode and the ISP command handler starts up. 
 *       In ISP mode, pin PIO0_0 is connected to function U0_RXD and pin PIO0_4 is connected to function U0_TXD on the USART0 block.
 */

volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
	msTicks++;
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

int main(void ) {
        SysTick_Config(SystemCoreClock / 1000);

	SwitchMatrix_Init();
	LPC_GPIO_PORT->DIR0 |= (1<<LED1);

	// Adafruit_PCD8544 display(3, 17, 16, 10, 11);

	// display.begin();
	while (1)
	{
	  //display.fillCircle(display.width()/2, display.height()/2, 10, BLACK);
	  //display.display();
	  led(1);
	  delay_ms(1000);
	  led(0);
	  delay_ms(1000);
	}

}

