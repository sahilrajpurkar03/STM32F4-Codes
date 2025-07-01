#include <stm32f4xx.h>

// Function Prototypes
void port_init(void);
void _delay_ms(uint32_t time);

int main(void)
{
    port_init();

    while (1)
    {
        // Turn ON LEDs on PD12–PD15
        GPIOD->ODR |= (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15);

        _delay_ms(1000);

        // Turn OFF LEDs (FIXED LOGIC)
        GPIOD->ODR &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));

        _delay_ms(1000);
    }
}

// Initialize GPIOD pins 12–15 as output
void port_init(void)
{
    RCC->AHB1ENR |= (1 << 3);  // Enable GPIOD clock

    // Clear MODER for PD12–15, then set to output (01)
    GPIOD->MODER &= ~(0xFF << 24);  // Clear bits 24–31
    GPIOD->MODER |= (1 << 24) | (1 << 26) | (1 << 28) | (1 << 30);  // Set as output

    // Set high speed for PD12–15
    GPIOD->OSPEEDR |= (0xFF << 24);  // High speed for LEDs

    // Set output type to push-pull
    GPIOD->OTYPER &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));
}

// Simple blocking delay (not accurate)
void _delay_ms(uint32_t time)
{
    for (uint32_t i = 0; i < time * 2000; i++);
}
