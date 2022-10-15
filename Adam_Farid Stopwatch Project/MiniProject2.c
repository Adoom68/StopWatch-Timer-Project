/*
 * MiniProject2.C
 *
 *  Created on: Sep 14, 2022
 *  Author: Adam Farid
 *  Subject: Mini Project 2
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

unsigned char g_tick=0, g_min1=0 , g_hr1=0;
unsigned char g_sec2=0, g_min2=0 , g_hr2=0;

ISR(INT0_vect)
{
	g_tick = 0;    g_sec2 = 0;
	g_min1 = 0;    g_min2 = 0;
	g_hr1 =0;      g_hr2 =0;
}
ISR(INT1_vect)
{
	TCCR1B&=~(1<<CS10) &~(1<<CS11) &~(1<<CS12);
}
ISR(INT2_vect)
{
	TCCR1B |=(1<<CS12) |(1<<CS10);
}

ISR(TIMER1_COMPA_vect)
{
	if(g_tick==9)
	{
		g_tick=0;
		if(g_sec2==5)
		{
			g_sec2=0;
			if(g_min1==9)
			{
				g_min1=0;
				if(g_min2==5)
				{
					g_min2=0;
					if(g_hr1==9)
					{
						g_hr1=0;
						g_hr2++;
					}
					else{g_hr1++;}
				}
				else{g_min2++;}
			}
			else{g_min1++;}
		}
		else{g_sec2++;}
	}
	else {g_tick++;}
}

void TIMER1_INIT_CTC (void)
{
	TIMSK |= (1<<OCIE1A);   /* Enable Timer1 Compare A Interrupt*/
	TCNT1 = 0;
	OCR1A = 970;     /*Compare Value */
	TCCR1A= (1<<FOC1A) ;
	TCCR1B= (1<<WGM12) | (1<<CS12) | (1<<CS10);
}
void Interrupts_INIT (void)
{
	MCUCSR &=~(1<<ISC2);
	MCUCR =(1<<ISC01)|(1<<ISC11) | (1<<ISC10);
	GICR|= (1<<INT0) | (1<<INT1) | (1<<INT2);  /* OR  GICR =0xE0;  */
}

int main (void){
	SREG |= (1<<7);
	TIMER1_INIT_CTC ();
	Interrupts_INIT ();

	DDRD&=~(1<<PD2)&~(1<<PD3);  /*set button 1 and 2 "interrupt 0 and 1" */
	DDRB&=~(1<<PB2);                   /*set button 3 "interrupt 2" */

	PORTD|=(1<<PD2);            /*to check on falling edge */
	PORTB|=(1<<PB2);            /*to check on falling edge */

	DDRC |= 0x0F;
	DDRA |= 0x3F;
	while (1)
	{
		PORTA =0X01;       /*display g_tick*/
		PORTC = (PORTC & 0xF0) | (g_tick & 0x0F);
		_delay_ms(5);

		PORTA =0X02;       /*display g_sec2*/
		PORTC = (PORTC & 0xF0) | (g_sec2 & 0x0F);
		_delay_ms(5);

		PORTA =0X04;       /*display g_min1*/
		PORTC = (PORTC & 0xF0) | (g_min1 & 0x0F);
		_delay_ms(5);

		PORTA =0X08;      /*display g_min2*/
		PORTC = (PORTC & 0xF0) | (g_min2 & 0x0F);
		_delay_ms(5);

		PORTA =0X10;      /*display g_hr1*/
		PORTC = (PORTC & 0xF0) | (g_hr1 & 0x0F);
		_delay_ms(5);

		PORTA =0X20;       /*display g_ht2*/
		PORTC = (PORTC & 0xF0) | (g_hr2 & 0x0F);
		_delay_ms(5);
	}
}


