#include <stm32f4xx.h>

// Function Prototypes
void eint_init(void);
void port_init(void);
void _delay_ms(uint32_t time);

// Global counter (not used in this version)
volatile int eint_count = 0;

int main(void)
{
    port_init();   // Initialize ports
    eint_init();   // Initialize external interrupt

    while (1)
    {
        // Main loop does nothing, waiting for interrupt
    }
}

// Initialize GPIO ports
void port_init(void)
{
    RCC->AHB1ENR |= (1 << 0) | (1 << 3);  // Enable clocks for GPIOA and GPIOD

    // Set PD12–PD15 as output
    GPIOD->MODER &= ~(0xFF << 24);                       // Clear MODER12–15
    GPIOD->MODER |= (1 << 24) | (1 << 26) | (1 << 28) | (1 << 30);  // Set to output
    GPIOD->OTYPER &= ~((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15));  // Push-pull
    GPIOD->OSPEEDR |= (0xFF << 24);  // High speed

    // Set PA0 as input
    GPIOA->MODER &= ~((1 << 0) | (1 << 1));  // Input mode for PA0
}

// Configure EXTI0 (PA0) for rising edge interrupt
void eint_init(void)
{
    RCC->APB2ENR |= (1 << 14);  // Enable SYSCFG clock

    SYSCFG->EXTICR[0] &= ~0xF;  // EXTI0 mapped to PA0

    EXTI->IMR |= (1 << 0);      // Unmask EXTI0
    EXTI->RTSR |= (1 << 0);     // Enable rising edge trigger

    // Optional: EXTI->EMR |= (1 << 0);  // Event mask (not needed for interrupts)

    NVIC_EnableIRQ(EXTI0_IRQn); // Enable interrupt in NVIC
}

// Interrupt Service Routine for EXTI0 (PA0)
void EXTI0_IRQHandler(void)
{
    if (EXTI->PR & (1 << 0))  // Check if EXTI0 triggered
    {
        EXTI->PR |= (1 << 0);  // Clear pending interrupt

        // Toggle LEDs on PD12–PD15
        GPIOD->ODR ^= (1 << 12) | (1 << 13) | (1 << 14) | (1 << 15);
    }
}

// Optional delay (not used here)
void _delay_ms(uint32_t time)
{
    for (uint32_t i = 0; i < time * 2000; i++);
}
