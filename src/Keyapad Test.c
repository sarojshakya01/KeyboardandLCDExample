/*
 * Keypad Test.c
 *
 * Created: 7/2/2014 6:53:54 PM
 * Author: Saroj Shakya
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>

#define KEY_PORT PORTC
#define KEY_PIN PINC
#define KEY_DDR DDRC
//---LCD Port Definition---//
#define LCD_DATA_PORT PORTA
#define LCD_DATA_DDR DDRA
#define LCD_RS 2
#define LCD_EN 3
//-------------------------//

unsigned char a;

void delay_us(unsigned int n)
{
	unsigned int i;
	for(i=0;i<n;i++)
	{
		TCNT0=0xFB;
		TCCR0=0x01;
		while((TIFR&(0x1<<TOV0))==0);
		TCCR0=0;
		TIFR=0x1<<TOV0;
	}
}
void delay_ms(unsigned int n)
{
	unsigned int i;
	for(i=0;i<n;i++)
	{
		TCNT0=0xF2;
		TCCR0=0x04;
		while((TIFR&(0x1<<TOV0))==0);
		TCCR0=0;
		TIFR=0x1<<TOV0;
	}
}//---LCD Code starts from here---//
void LCD_cmnd(unsigned char cmnd) //function to send command to LCD Module
{
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmnd & 0xF0); //send upper 4 bit
	LCD_DATA_PORT &= ~(1<<LCD_RS); //0b11111011 //RS = 0
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000 //EN = 1
	_delay_us(50);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111 //EN = 0
	_delay_us(200);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (cmnd << 4); //send lower 4 bit
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000 //EN = 1
	_delay_us(50);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111 //EN = 0
}
void LCD_data(unsigned char data) //Function to send data to LCD Module
{
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data & 0xF0);
	LCD_DATA_PORT |= 1<<LCD_RS; //0b00000100 //RS = 1
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000
	_delay_us(50);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111
	_delay_us(2000);
	LCD_DATA_PORT = (LCD_DATA_PORT & 0x0F) | (data << 4);
	LCD_DATA_PORT |= 1<<LCD_EN; //0b00001000
	_delay_us(2000);
	LCD_DATA_PORT &= ~(1<<LCD_EN); //0b11110111
}
void LCD_initialize(void) //Function to initialize LCD Module
{
	LCD_DATA_DDR = 0xFC;
	LCD_DATA_PORT &= ~(0xC1<<LCD_EN); //0b00110111;
	_delay_ms(200);
	LCD_cmnd(0x33);
	_delay_ms(20);
	LCD_cmnd(0x32);
	_delay_ms(20);
	LCD_cmnd(0x28);
	_delay_ms(20);
	LCD_cmnd(0x0E);
	_delay_ms(20);
	LCD_cmnd(0x01);
	_delay_ms(20);
}
void LCD_clear(void) //Function to clear the LCD Screen
{
	LCD_cmnd(0x01);
	_delay_ms(2);
}
void LCD_print(char * str) //Function to print the string on LCD Screen
{
	unsigned char i=0;
	while(str[i] != 0)
	{
		LCD_data(str[i]);
		i++;
		_delay_ms(5);
	}
}
void LCD_set_curser(unsigned char y, unsigned char x) //Function to move the Curser at (y,x) position
{
	if(y==1)
	LCD_cmnd(0x7F+x);
	else if(y==2)
	LCD_cmnd(0xBF+x);
}
void LCD_num(unsigned char num) //Function to display number
{
	//LCD_data(num/100 + 0x30);
	//num = num%100;
	LCD_data(num/10 + 0x30);
	LCD_data(num%10 + 0x30);
}
//---LCD Code ends here---//
void read_column1()
{
	a=KEY_PIN;
	LCD_data(a);
	KEY_PORT=0xFE;
	if((KEY_PIN&0xF0)==0xE0)
	{
		delay_ms(20);
		LCD_data('1');
	}
	KEY_PORT=0xFD;
	if((KEY_PIN&0xF0)==0xE0)
	{
		delay_ms(20);
		LCD_data('4');
	}
	KEY_PORT=0xFB;
	if((KEY_PIN&0xF0)==0xE0)
	{
		delay_ms(20);
		LCD_data('7');
	}
	KEY_PORT=0xF7;
	if((KEY_PIN&0xF0)==0xE0)
	{
		delay_ms(20);
		LCD_data('*');
	}
}
void read_column2()
{
	KEY_PORT=0xFE;
	if((KEY_PIN&0xF0)==0xD0)
	LCD_data('2');
	KEY_PORT=0xFD;
	if((KEY_PIN&0xF0)==0xD0)
	LCD_data('5');
	KEY_PORT=0xFB;
	if((KEY_PIN&0xF0)==0xD0)
	LCD_data('8');
	KEY_PORT=0xF7;
	if((KEY_PIN&0xF0)==0xD0)
	LCD_data('0');
}
void read_column3()
{
	KEY_PORT=0xFE;
	if((KEY_PIN&0xF0)==0xB0)
	LCD_data('3');
	KEY_PORT=0xFD;
	if((KEY_PIN&0xF0)==0xB0)
	LCD_data('6');
	KEY_PORT=0xFB;
	if((KEY_PIN&0xF0)==0xB0)
	LCD_data('9');
	KEY_PORT=0xF7;
	if((KEY_PIN&0xF0)==0xB0)
	LCD_data('#');
}
void read_column4()
{
	KEY_PORT=0xFE;
	if((KEY_PIN&0xF0)==0x70)
	LCD_data('A');
	KEY_PORT=0xFD;
	if((KEY_PIN&0xF0)==0x70)
	LCD_data('B');
	KEY_PORT=0xFB;
	if((KEY_PIN&0xF0)==0x70)
	LCD_data('C');
	KEY_PORT=0xF7;
	if((KEY_PIN&0xF0)==0x70)
	LCD_data('D');
}
int main()
{
	KEY_DDR=0x0F;
	KEY_PORT=0xFF;
	LCD_initialize();
	while(1)
	{
		KEY_PORT=0xF0;
		if((KEY_PIN&0xF0)==0xE0)
		{
			delay_ms(20);
			read_column1();
			delay_ms(200);
		}
		if((KEY_PIN&0xF0)==0xD0)
		{
			delay_ms(20);
			read_column2();
			delay_ms(200);
		}
		if((KEY_PIN&0xF0)==0xB0)
		{
			delay_ms(20);
			read_column3();
			delay_ms(200);
		}
		if((KEY_PIN&0xF0)==0x70)
		{
			delay_ms(20);
			read_column4();
			delay_ms(200);
		}
	}
}
