#include <stm32f4xx.h>

void eint_init(void);
void EXTI0_IRQHandler(void);
void port_init(void);
void _delay_ms(uint32_t time);

volatile int eint_count=0;

int main (void)
{
		port_init();
		eint_init();
	
		while(1)
		{
		}
}

void port_init(void)
{
	RCC->APB2ENR|=(1<<14);					//ENABLE CONFIGURE
	RCC->AHB1ENR|=(1<<3)|(1<<0);		//ENABLE GPIOD AND GPIOC
	GPIOD->MODER|=(1<<30)|(1<<28)|(1<<26)|(1<<24);		//PD12-15 set as output
	GPIOD->OSPEEDR|=0xFF000000;		//IO speed= high speed
	GPIOD->OTYPER&=~(1<<12)&(1<<13)&(1<<14)&(1<<15);		//Push-pull reset state

	GPIOA->MODER&=~(1<<0)&(1<<1);											//PC13 SET AS INPUT MODE	
}

void eint_init(void)
{
	SYSCFG->EXTICR[0]&=~(1<<0)&(1<<1)&(1<<2)&(1<<3);															//SOURCE INPUT FOR EXTERNAL INPUT
	EXTI->IMR|=(1<<0);																		//LINE IS NOT MASKED
	EXTI->EMR|=(1<<0);																		//
	EXTI->RTSR|=(1<<0);                 //RISING EDGE ENABLE 
        NVIC_EnableIRQ(EXTI0_IRQn);
}



void EXTI0_IRQHandler(void)
{
	if (EXTI->PR & (1<<0))														// EXTI13 interrupt pending?
	{                          
		EXTI->PR |= (1<<0);     	                      // clear pending interrupt
                  GPIOD->ODR^=(1<<12)|(1<<13)|(1<<14)|(1<<15);
	}
}

