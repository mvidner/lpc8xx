/* 
 * File:   lcd3310.h
 * Author: vtech
 *
 * Created on November 7, 2012, 9:20 AM
 */

#ifndef LCD3310_H
#define	LCD3310_H

#include <stdint.h>        /* For uint8_t definition */

#ifdef	__cplusplus
extern "C" {
#endif

void lcd_init(void);
void lcd_clr(void);    
void lcd_goto(uint8_t x, uint8_t y);
void lcd_wchar(uint8_t c);
void lcd_wdig(uint8_t d);
void lcd_wbyte(uint8_t b);
void lcd_wstr(const char *c);
void lcd_wlong(long int x);
void lcd_wint(int x);
void lcd_wuint(unsigned int x);
void lcd_vline(uint8_t x, uint8_t y1, uint8_t y2);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD3310_H */

