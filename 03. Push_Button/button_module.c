#include<stm32f4xx.h>

void port_init(void);
void read_port(void);
void _delay_ms(uint32_t time);

uint8_t pres_butt[8]={0,0,0,0,0,0,0,0};
uint8_t prev_butt[8]={0,0,0,0,0,0,0,0};
uint8_t button[8]={0,0,0,0,0,0,0,0};

int main (void)
{
	port_init();
	while(1)
	{
		read_port();
	}
}

void port_init(void)
{
	RCC->APB2ENR|=(1<<14);						//SYSYEM CONFIGURE CLOCK ENABLE
	RCC->AHB1ENR|=(1<<0);							
	
	GPIOA->MODER&=~0xFFFFFF00;				//PA0-PA7 set as Input
}

void read_port(void)
{
	for (int i=0;i<8;i++)
	{
		pres_butt[i]=(GPIOA->IDR & (1<<i));
		if (pres_butt[i])
		{
			_delay_ms(5);
			pres_butt[i]=(GPIOA->IDR<<i & (1<<i));
			if ((pres_butt[i]) && (!prev_butt[i]))
			{
				button[i]=1;
			}
		}
		prev_butt[i]=pres_butt[i];
	}
	
	if (button[0])
	{
		button[0]=0;
	}
	if (button[1])
	{
		button[1]=0;
	}
	if (button[2])
	{
		button[2]=0;
	}
	if (button[3])
	{
		button[3]=0;
	}
	if (button[4])
	{
		button[4]=0;
	}
	if (button[5])
	{
		button[5]=0;
	}
	if (button[6])
	{
		button[6]=0;
	}
	if (button[7])
	{
		button[7]=0;
	}
}

void _delay_ms(uint32_t time)
{
	for(uint32_t i=0;i<time*2000;i++);
}
