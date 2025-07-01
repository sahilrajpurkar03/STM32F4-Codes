#include <stm32f4xx.h>

// Function prototypes
void port_init(void);
void timer_init(void);
void pwm_init(void);
void _delay_ms(uint32_t time);

int main(void) {
    port_init();
    timer_init(); 
    pwm_init();

    while (1) {
        // Vary duty cycle using CCR1 (0 to 1000 with ARR = 1000 gives 0% to 100%)
        TIM4->CCR1 = 200; // ~20% duty cycle
        GPIOD->ODR |= (1 << 13);  // Turn on PD13
        GPIOD->ODR &= ~(1 << 14); // Turn off PD14
        _delay_ms(2000);

        TIM4->CCR1 = 800; // ~80% duty cycle
        GPIOD->ODR |= (1 << 14);  // Turn on PD14
        GPIOD->ODR &= ~(1 << 13); // Turn off PD13
        _delay_ms(2000);
    }
}

// Configure PD12 (TIM4 CH1) as alternate function, PD13 & PD14 as digital outputs
void port_init(void) {
    RCC->AHB1ENR |= (1 << 3);    // Enable GPIOD clock

    // Set PD12 as Alternate Function (AF) mode
    GPIOD->MODER &= ~(0x3 << (12 * 2));     // Clear mode
    GPIOD->MODER |= (0x2 << (12 * 2));      // Set AF mode for PD12

    // Set PD13, PD14 as output
    GPIOD->MODER &= ~((0x3 << (13 * 2)) | (0x3 << (14 * 2)));
    GPIOD->MODER |=  ((0x1 << (13 * 2)) | (0x1 << (14 * 2)));

    GPIOD->OSPEEDR |= (0x3 << (12 * 2)) | (0x3 << (13 * 2)) | (0x3 << (14 * 2)); // High speed
    GPIOD->OTYPER &= ~((1 << 12) | (1 << 13) | (1 << 14)); // Push-pull
    GPIOD->PUPDR &= ~((0x3 << (12 * 2))); // No pull-up/pull-down

    // Set PD12 to AF2 (TIM4_CH1)
    GPIOD->AFR[1] &= ~(0xF << ((12 - 8) * 4));  // Clear
    GPIOD->AFR[1] |=  (0x2 << ((12 - 8) * 4));  // AF2 = TIM4_CH1
}

// Configure Timer 4 for PWM
void timer_init(void) {
    RCC->APB1ENR |= (1 << 2); // Enable TIM4 clock

    TIM4->PSC = 15;      // Prescaler
    TIM4->ARR = 1000;    // Auto-reload (PWM period)

    TIM4->CR1 &= ~(1 << 4); // Upcounter mode
    TIM4->CR1 |= (1 << 7);  // Enable auto-reload preload
    TIM4->EGR |= (1 << 0);  // Generate an update event to load the registers
    TIM4->CR1 |= (1 << 0);  // Enable the counter
}

// Set up PWM on TIM4 Channel 1 (PD12)
void pwm_init(void) {
    // Set PWM mode 1 on channel 1
    TIM4->CCMR1 &= ~(0x7 << 4);           // Clear OC1M bits
    TIM4->CCMR1 |=  (0x6 << 4);           // OC1M: PWM mode 1
    TIM4->CCMR1 |=  (1 << 3);             // Enable preload for CCR1

    TIM4->CCER  &= ~(1 << 1);             // Output polarity: active high
    TIM4->CCER  |=  (1 << 0);             // Enable output for channel 1

    TIM4->CCR1 = 400;                     // Initial duty cycle (~40%)

    TIM4->BDTR |= (1 << 15);              // Main output enable (MOE)
}

// Crude software delay
void _delay_ms(uint32_t time) {
    for (uint32_t i = 0; i < time * 2000; i++);
}
