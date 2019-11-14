/*
 * LCD
 * Created: 6/6/2014 4:55:52 AM
 *  Author: Saroj Shakya
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define LCD_DATAPORT PORTA
#define LCD_DATADDR DDRA
#define LCD_CMNDPORT PORTB
#define LCD_CMNDDDR DDRB
#define LCD_RS 0
#define LCD_RW 1
#define LCD_EN 2

void LCD_cmnd(unsigned char cmnd)
{
	LCD_DATAPORT=cmnd;
	LCD_CMNDPORT &= ~(1<<LCD_RS);
	LCD_CMNDPORT &= ~(1<<LCD_RW);
	LCD_CMNDPORT |= (1<<LCD_EN);
	_delay_us(5);
	LCD_CMNDPORT &= ~(1<<LCD_EN);
	_delay_us(200);
}
void LCD_data(unsigned char data)
{
	LCD_DATAPORT = data;
	LCD_CMNDPORT |= (1<<LCD_RS);
	LCD_CMNDPORT &= ~(1<<LCD_RW);
	LCD_CMNDPORT |= (1<<LCD_EN);
	_delay_us(5);
	LCD_CMNDPORT &= ~(1<<LCD_EN);
	_delay_us(200);
}
void LCD_initialize(void)
{
	LCD_DATADDR = 0xFF;
	LCD_CMNDDDR = 0xFF;
	LCD_CMNDPORT &= ~(1<<LCD_EN);
	LCD_cmnd(0x38);
	_delay_ms(2);
	LCD_cmnd(0x0E);
	_delay_ms(2);
	LCD_cmnd(0x01);
	_delay_ms(2);	
}
void LCD_clear(void)
{
	LCD_cmnd(0x01);
	_delay_ms(2);
}
void LCD_print(char * str)
{
	unsigned char i=0;
	while(str[i] != 0)
	{
		LCD_data(str[i]);
		i++;
		_delay_ms(5);
	}
}
void LCD_set_curser(unsigned char y, unsigned char x)
{
	if(y==1)
	LCD_cmnd(0x7F+x);
	else if(y==2)
	LCD_cmnd(0xBF+x);
}
void LCD_num(unsigned char num)
{
	LCD_data(num/100 + 0x30);
	num = num%100;
	LCD_data(num/10 + 0x30);
	LCD_data(num%10 + 0x30);
}