#include <stm32f4xx.h>

// Function prototypes
void adc_init(void);
void port_init(void);
uint16_t adc_read(void);

// Global variables
uint16_t adc_value = 0;
int test = 0;

int main(void)
{
    port_init();    // Initialize GPIOA (PA0) as analog input
    adc_init();     // Initialize ADC1

    while (1)
    {
        adc_value = adc_read();  // Read ADC value from channel 0
        test++;                  // Dummy increment to check loop
    }
}

// Initialize GPIOA Pin 0 (PA0) as analog input
void port_init(void)
{
    RCC->AHB1ENR |= (1 << 0);        // Enable clock for GPIOA
    GPIOA->MODER |= (3 << 0);        // Set PA0 to analog mode (MODER0 = 1, MODER1 = 1)
    GPIOA->PUPDR &= ~(3 << 0);       // No pull-up, no pull-down on PA0
}

// Initialize ADC1 on channel 0
void adc_init(void)
{
    RCC->APB2ENR |= (1 << 8);        // Enable ADC1 clock
    RCC->CR |= (1 << 0);             // Enable HSI (High Speed Internal) clock

    ADC->CCR |= (3 << 16);           // Set ADC prescaler to PCLK2 / 8
    ADC->CCR &= ~(0x1F);             // Independent mode, other bits cleared

    ADC1->CR1 &= ~(1 << 25);         // Disable resolution bits
    ADC1->CR1 |= (1 << 24);          // Set 8-bit resolution
    ADC1->CR1 |= (1 << 5);           // Enable EOC interrupt (not used here, but enables EOC flag)

    ADC1->CR2 |= (1 << 0);           // Enable ADC
    ADC1->CR2 |= (1 << 1);           // Enable continuous conversion
    ADC1->CR2 |= (1 << 10);          // Enable EOC flag

    ADC1->SQR1 &= ~(0xF << 20);      // Set number of conversions to 1
    ADC1->SQR3 &= ~(0x1F);           // Select channel 0 for conversion

    ADC1->SMPR2 &= ~(7 << 0);        // Set channel 0 sample time to 3 cycles
}

// Read ADC1 value from channel 0
uint16_t adc_read(void)
{
    ADC1->CR2 |= (1 << 30);          // Start conversion
    while (!(ADC1->SR & (1 << 1)));  // Wait until EOC (End of Conversion)
    ADC1->SR &= ~(1 << 1);           // Clear EOC flag

    adc_value = ADC1->DR;            // Read ADC value
    return adc_value;
}
