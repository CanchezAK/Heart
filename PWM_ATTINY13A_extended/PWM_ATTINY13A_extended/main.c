/*
 * PWM_ATTINY13A_extended.c
 *
 * Created: 06.01.2022 8:37:59
 * Author : CanchezAK
 */ 
//Defines
#define F_CPU 1200000
//Includes
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

//Array PWM
const uint16_t PWM_table[32]  PROGMEM = {
	0 , 0 , 2 , 3 , 6 , 8 , 12 , 15 , 20 , 24 , 30 , 35 , 42 , 48 , 56 , 63,
	71 , 80 , 89 , 99 , 109 , 120 , 131 , 143 , 155 , 168 , 181 , 195 , 209 , 224 , 239 , 255
};
//Variables
uint8_t start = 0;

//Working function
void working_mode(uint8_t mode)
{
	switch (mode)
	{
		case 0:
		asm("cli");
		break;
		case 1:
		for (uint8_t i = 0; i < 16; i++)
		{
			if (i == 1)
			{
				asm("sei");
			}
			OCR0A = PWM_table[i];
			_delay_us(800);
		}
		for (uint8_t i = 16; i > 0; i--)
		{
			if (i == 1)
			{
				asm("cli");
			}
			OCR0A = PWM_table[i];
			_delay_us(800);
		}
		break;
		case 2:
		for (uint8_t i = 0; i < 32; i++)
		{
			if (i == 1)
			{
				asm("sei");
			}
			OCR0A = PWM_table[i];
			_delay_us(400);
		}
		for (uint8_t i = 32; i > 0; i--)
		{
			if (i == 1)
			{
				asm("cli");
			}
			OCR0A = PWM_table[i];
			_delay_us(400);
		}
		break;
		case 3:
		//2 lights, 50 BPM
		for (uint8_t i = 0; i < 2; i++)
		{

			for (uint8_t i = 0; i < 16; i++)
			{
				if (i > 0)
				{
					asm("sei");
				}
				OCR0A = PWM_table[i];
				_delay_us(10000);
			}
			
			for (uint8_t i = 16; i < 32; i++)
			{
				OCR0A = PWM_table[i];
				_delay_us(5000);
			}
			for (uint8_t i = 32; i > 0; i--)
			{
				if (i == 1)
				{
					asm("cli");
				}
				OCR0A = PWM_table[i];
				_delay_us(5000);
			}
		}
		_delay_us(1000000);
		break;
		case 4:
		//2 lights, 90 BPM
		for (uint8_t i = 0; i < 2; i++)
		{

			for (uint8_t i = 0; i < 16; i++)
			{
				if (i > 0)
				{
					asm("sei");
				}
				OCR0A = PWM_table[i];
				_delay_us(10000);
			}
			
			for (uint8_t i = 16; i < 32; i++)
			{
				OCR0A = PWM_table[i];
				_delay_us(5000);
			}
			for (uint8_t i = 32; i > 0; i--)
			{
				if (i == 1)
				{
					asm("cli");
				}
				OCR0A = PWM_table[i];
				_delay_us(5000);
			}
		}
		_delay_us(666666);
		break;
		
		case 5:
		//2 lights, 180 BPM
		for (uint8_t i = 0; i < 2; i++)
		{

			for (uint8_t i = 0; i < 16; i++)
			{
				if (i > 0)
				{
					asm("sei");
				}
				OCR0A = PWM_table[i];
				_delay_us(10000);
			}
			
			for (uint8_t i = 16; i < 32; i++)
			{
				OCR0A = PWM_table[i];
				_delay_us(5000);
			}
			for (uint8_t i = 32; i > 0; i--)
			{
				if (i == 1)
				{
					asm("cli");
				}
				OCR0A = PWM_table[i];
				_delay_us(5000);
			}
		}
		_delay_us(333333);
		break;
		default:
		mode = 0;
		break;
	}
}

int main(void)
{
    asm("cli");
    DDRB |= 0x01; //PB0 как выход, остальные PB как вход
    PORTB = 0x04;
    TCCR0A = 0x83; // режим ШИМ, неинверсный сигнал на выходе OC0A
    TCCR0B = 0x02;
    TCNT0=0;
    OCR0A=0;
    asm("sei");
    while (1) 
    {
		uint8_t debounce = 0;
		if ((PINB&(1<<2)) == 0)
		{
			while((PINB&(1<<2)) == 0)
			{
				debounce++;
			}
			if (debounce >= 21)
			{
				if (start == 0) start = 1;
				else if (start == 1) start = 2;
				else if (start == 2) start = 3;
				else if (start == 3) start = 4;
				else if (start == 4) start = 5;
				else if (start == 5) start = 0;
			}
		}
		working_mode(start);
    }
}

