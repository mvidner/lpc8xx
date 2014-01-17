#include "main.h"
#include "lcd3310.h"

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

void lcd_pin_generic(int value, uint32_t mask) {
    if (value)
	LPC_GPIO_PORT->SET0 = mask;
    else
	LPC_GPIO_PORT->CLR0 = mask;
}

void lcd_pin_clock(int value) {
    lcd_pin_generic(value, 1<<3);
}

void lcd_pin_data(int value) {
    lcd_pin_generic(value, 1<<17);
}

void lcd_pin_datanotcmd(int value) {
    lcd_pin_generic(value, 1<<16);
}

void lcd_pin_enab(int value) {
    lcd_pin_generic(value, 1<<10);
}

void lcd_pin_reset(int value) {
    lcd_pin_generic(value, 1<<11);
}

extern void SwitchMatrix_Init(void);

int main(void ) {
        SysTick_Config(SystemCoreClock / 1000);

	SwitchMatrix_Init();
	LPC_GPIO_PORT->DIR0 |= (1<<LED1) |
	    // LCD pins
	    (1<<3) |
	    (1<<10) |
	    (1<<11) |
	    (1<<16) |
	    (1<<17);

        lcd_init();
        lcd_wbyte(42);
	while (1)
	{
		LPC_GPIO_PORT->SET0 = 1<<LED1;    // LED1 output high
		delay_ms(1000);
		LPC_GPIO_PORT->CLR0 = 1<<LED1;    // LED1 output low
		delay_ms(1000);
	}

}

