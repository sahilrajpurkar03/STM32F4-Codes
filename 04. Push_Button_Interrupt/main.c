#include <stm32f4xx.h>

void port_init(void);
void eint_init(void);
void read_port(void);
void _delay_ms(uint32_t time);

uint8_t button[8] = {0};  // Button press flags for PA0–PA7

int main(void) {
    port_init();   // Set PA0–PA7 as input
    eint_init();   // Configure external interrupts

    while (1) {
        read_port();  // Check if any button was pressed
    }
}

// GPIOA PA0–PA7 as input
void port_init(void) {
    RCC->AHB1ENR |= (1 << 0);        // Enable GPIOA clock
    GPIOA->MODER &= ~0x0000FFFF;     // Set PA0–PA7 as input (MODER[15:0] = 0)
    GPIOA->PUPDR &= ~0x0000FFFF;     // No pull-up, pull-down
}

// External interrupt configuration for PA0–PA7 (EXTI0–EXTI7)
void eint_init(void) {
    RCC->APB2ENR |= (1 << 14);       // Enable SYSCFG clock

    // Configure EXTI lines for GPIOA (0-3 are in EXTICR[0], etc.)
    SYSCFG->EXTICR[0] = 0x0000;  // EXTI0–3 -> PA0–PA3
    SYSCFG->EXTICR[1] = 0x0000;  // EXTI4–7 -> PA4–PA7

    EXTI->IMR  |= 0xFF;          // Unmask EXTI0–7
    EXTI->RTSR |= 0xFF;          // Rising edge trigger
    EXTI->FTSR &= ~0xFF;         // Disable falling edge

    // Enable IRQs in NVIC
    NVIC_EnableIRQ(EXTI0_IRQn);
    NVIC_EnableIRQ(EXTI1_IRQn);
    NVIC_EnableIRQ(EXTI2_IRQn);
    NVIC_EnableIRQ(EXTI3_IRQn);
    NVIC_EnableIRQ(EXTI4_IRQn);
    NVIC_EnableIRQ(EXTI9_5_IRQn);  // Handles EXTI5–EXTI9 (grouped)
}

// Called in main loop to process button presses
void read_port(void) {
    for (int i = 0; i < 8; i++) {
        if (button[i]) {
            button[i] = 0;  // Reset button flag after handling
            // Action on button[i] press goes here
        }
    }
}

// --- IRQ Handlers for EXTI0–EXTI7 ---

void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1 << 0)) {
        EXTI->PR |= (1 << 0);  // Clear interrupt flag
        button[0] = 1;
    }
}

void EXTI1_IRQHandler(void) {
    if (EXTI->PR & (1 << 1)) {
        EXTI->PR |= (1 << 1);
        button[1] = 1;
    }
}

void EXTI2_IRQHandler(void) {
    if (EXTI->PR & (1 << 2)) {
        EXTI->PR |= (1 << 2);
        button[2] = 1;
    }
}

void EXTI3_IRQHandler(void) {
    if (EXTI->PR & (1 << 3)) {
        EXTI->PR |= (1 << 3);
        button[3] = 1;
    }
}

void EXTI4_IRQHandler(void) {
    if (EXTI->PR & (1 << 4)) {
        EXTI->PR |= (1 << 4);
        button[4] = 1;
    }
}

// Handles EXTI lines 5–9 (grouped together)
void EXTI9_5_IRQHandler(void) {
    for (int i = 5; i <= 7; i++) {
        if (EXTI->PR & (1 << i)) {
            EXTI->PR |= (1 << i);
            button[i] = 1;
        }
    }
}

// Simple delay function (not precise)
void _delay_ms(uint32_t time) {
    for (uint32_t i = 0; i < time * 2000; i++);
}
