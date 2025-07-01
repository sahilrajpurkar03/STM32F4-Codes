#include <stm32f4xx.h>

// Function prototypes
void port_init(void);
void read_port(void);
void _delay_ms(uint32_t time);

// Button state arrays
uint8_t pres_butt[8] = {0};
uint8_t prev_butt[8] = {0};
uint8_t button[8]    = {0};

int main(void)
{
    port_init();  // Initialize GPIOA as input for buttons

    while (1)
    {
        read_port();  // Continuously check button states
    }
}

// Initialize GPIOA pins PA0–PA7 as inputs
void port_init(void)
{
    RCC->AHB1ENR |= (1 << 0);        // Enable GPIOA clock
    GPIOA->MODER &= ~0x0000FFFF;     // Set PA0–PA7 as input (00)
    GPIOA->PUPDR &= ~0x0000FFFF;     // No pull-up, pull-down for PA0–PA7
}

// Reads button press with basic debounce
void read_port(void)
{
    for (int i = 0; i < 8; i++)
    {
        pres_butt[i] = (GPIOA->IDR & (1 << i)) ? 1 : 0;

        if (pres_butt[i])  // Button potentially pressed
        {
            _delay_ms(5);  // Basic debounce delay

            // Read again after delay to confirm it's still pressed
            if ((GPIOA->IDR & (1 << i)) && !prev_butt[i])
            {
                button[i] = 1;  // Valid new button press detected
            }
        }
        else
        {
            button[i] = 0;  // Reset if button is released
        }

        prev_butt[i] = pres_butt[i];  // Update previous state
    }

    // Handle button press events
    for (int i = 0; i < 8; i++)
    {
        if (button[i])
        {
            // Action for button[i] press goes here
            // Example: toggle LED, send signal, etc.
            button[i] = 0;  // Reset the flag after handling
        }
    }
}

// Crude software delay (not accurate for real-time)
void _delay_ms(uint32_t time)
{
    for (uint32_t i = 0; i < time * 2000; i++);
}
