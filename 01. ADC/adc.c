#include <stm32f4xx.h>

void adc_init(void);
void port_init(void);
uint16_t adc_read(void);

uint16_t adc_value=0;
int test=0;

int main (void)
{
		port_init();
		adc_init();
		while(1)
		{
			adc_value=adc_read();	
			test++;	
		}
}

void port_init(void)
{
	RCC->APB2ENR|=(1<<14);				//ENABLE CONFIGURE
	RCC->AHB1ENR|=(1<<0);						//ENABLE GPIOA
	GPIOA->MODER|=(1<<0)|(1<<1);		//PA0 SET ANALOG MODE
	GPIOA->PUPDR =0X00000000;				//NO PULL UP, NO PULL DOWN
}

void adc_init(void)
{
	RCC->APB2ENR|=(1<<8);			  	//ENABLE ADC CLOCK used for register read/write access
	RCC->CR|=(1<<0);							//INTERNAL HIGH SPEED CLOCK ENABLE
	ADC1->CR2|=(1<<0)|(1<<10);		//ENABLE ADC AND EOC IS ENABLE 
	ADC1->CR2|=(1<<1);						//CONTINUOUS CONVERSION MODE IS SELECTED
	ADC->CCR |=(1<<17)|(1<<16);		//PRESCALAR IS FSC/8
	ADC->CCR &=~(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4); 		//SET INDEPENDENT MODE 
	ADC1->CR1&=~(1<<25); 					//
	ADC1->CR1|=(1<<24);					//SET ADC 8 BIT
	ADC1->CR1|=(1<<5);						//ENABLE EOC
	ADC1->SQR1&=~(1<<20)|(1<<21)|(1<<22)|(1<<23);	//CONVERSION 1
	ADC1->SQR1&=~(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);	//	
	ADC1->SMPR2&=~(1<<0)|(1<<1)|(1<<2);				//CHANEL 0 SAMPLING TIME 3 CYCLES
}

uint16_t adc_read(void)
{
	ADC1->CR1&=~(1<<30);															//RESET STATE
	ADC1->CR1&=~(1<<0)|(1<<1)|(1<<2)|(1<<3)|(1<<4);		//SELECT CHANNEL 0
	ADC1->CR2|=(1<<30);																//START OF CONVERSION
	while(!(ADC1->SR & (1<<1)));											//WAIT FOR END OF CONVERSION OF ADC1 FLAG
	ADC1->SR&=~(1<<1);
	adc_value=ADC1->DR;
	return adc_value;
} 

