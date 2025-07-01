#include <stm32f4xx.h>

void port_init(void);
void _delay_ms(uint32_t time);

int main(void)
{
    port_init();  // Initialize GPIOA and GPIOD

    while (1)
    {
        if (GPIOA->IDR & (1 << 0))  // Check if PA0 is HIGH
        {
            GPIOD->ODR |= (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15);  // Turn on LEDs (PD12–PD15)
        }
        else
        {
            GPIOD->ODR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));  // Turn off LEDs
        }
    }
}

// Initialize PA0 as input, PD12–PD15 as output
void port_init(void)
{
    RCC->AHB1ENR |= (1 << 0) | (1 << 3);  // Enable GPIOA and GPIOD clocks

    // Set PD12–PD15 as output
    GPIOD->MODER &= ~(0xFF << 24);       //
