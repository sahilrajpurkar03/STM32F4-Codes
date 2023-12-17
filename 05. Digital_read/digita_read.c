#include <stm32f4xx.h>

void port_init(void);
void _delay_ms(uint32_t time);

int main(void)
{
	port_init();

	while(1)
	{
		if((GPIOA->IDR) & (1<<0))
		{
			GPIOD->ODR|=(1<<12)|(1<<13)|(1<<14)|(1<<15);
		}
		else
		{
			GPIOD->ODR&=~(1<<12)&(1<<13)&(1<<14)&(1<<15);
		}

	}	
}

void port_init(void)
{
	RCC->APB2ENR|=(1<<14);														//enable configure
	RCC->AHB1ENR|=(1<<3)|(1<<0);															//enable gpiod
	GPIOD->MODER|=(1<<30)|(1<<28)|(1<<26)|(1<<24);		//PD12-15 set as output
	GPIOD->OSPEEDR|=0xFF000000;												//IO speed= high speed
	GPIOD->OTYPER&=~(1<<12)&(1<<13)&(1<<14)&(1<<15);	//Push-pull reset state
	
	GPIOA->MODER&=~(1<<0)&(1<<1);											//PC13 SET AS INPUT MODE
}

void _delay_ms(uint32_t time)
{
	for(uint32_t i=0;i<time*2000;i++);
}
