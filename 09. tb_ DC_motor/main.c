#include <stm32f4xx.h>

// --- Function Prototypes ---
void port_init(void);
void timer_init(void);
void pwm_init(void);
void motor1(int sp_vect);
void motor2(int sp_vect);
int limit_var(int value, int min, int max);
void _delay_ms(uint32_t time);

// --- Constants ---
#define js_error 15  // Deadband around 0

// --- Global Variables ---
#define motor_dir_1 GPIOD->ODR
#define motor_dir_2 GPIOC->ODR
#define motor_pwm1 TIM4->CCR1
#define motor_pwm2 TIM4->CCR2

// --- Main Function ---
int main(void) {
    port_init();
    timer_init(); 
    pwm_init();

    while (1) {
        motor1(200);    // Forward
        motor2(-150);   // Reverse
        _delay_ms(2000);

        motor1(0);      // Stop
        motor2(0);      // Stop
        _delay_ms(2000);
    }
}

// --- GPIO Port Initialization ---
void port_init(void) {
    RCC->AHB1ENR |= (1 << 3) | (1 << 2);   // Enable GPIOD and GPIOC

    // PD12, PD13 as AF (PWM)
    GPIOD->MODER &= ~((3 << (12 * 2)) | (3 << (13 * 2)));
    GPIOD->MODER |=  ((2 << (12 * 2)) | (2 << (13 * 2)));

    // PD12, PD13 AF2 (TIM4)
    GPIOD->AFR[1] &= ~((0xF << 16) | (0xF << 20));   // Clear
    GPIOD->AFR[1] |=  ((0x2 << 16) | (0x2 << 20));   // AF2

    // PD12, PD13 Speed & Type
    GPIOD->OSPEEDR |= (3 << (12 * 2)) | (3 << (13 * 2));  // High speed
    GPIOD->OTYPER &= ~((1 << 12) | (1 << 13));            // Push-pull

    // PC1, PC3 as output (Motor Direction)
    GPIOC->MODER &= ~((3 << (1 * 2)) | (3 << (3 * 2)));    // Clear
    GPIOC->MODER |=  ((1 << (1 * 2)) | (1 << (3 * 2)));    // Output mode
    GPIOC->OSPEEDR |= (3 << (1 * 2)) | (3 << (3 * 2));     // High speed
    GPIOC->OTYPER &= ~((1 << 1) | (1 << 3));               // Push-pull
}

// --- Timer Initialization (TIM4) ---
void timer_init(void) {
    RCC->APB1ENR |= (1 << 2);  // Enable TIM4

    TIM4->PSC = 15;           // Prescaler (adjust as needed)
    TIM4->ARR = 1000;         // Auto-reload value for 1 kHz (with 16 MHz clk)

    TIM4->CR1 |= (1 << 7);    // Enable auto-reload preload
    TIM4->EGR |= (1 << 0);    // Generate update event
    TIM4->CR1 |= (1 << 0);    // Enable counter
}

// --- PWM Initialization (TIM4 CH1, CH2) ---
void pwm_init(void) {
    // CH1 (PD12) - PWM1
    TIM4->CCMR1 &= ~(0x7 << 4);
    TIM4->CCMR1 |=  (0x6 << 4);      // PWM mode 1
    TIM4->CCMR1 |=  (1 << 3);        // Preload enable

    TIM4->CCER &= ~(1 << 1);         // Polarity high
    TIM4->CCER |=  (1 << 0);         // Enable CH1 output

    // CH2 (PD13) - PWM2
    TIM4->CCMR1 &= ~(0x7 << 12);
    TIM4->CCMR1 |=  (0x6 << 12);     // PWM mode 1
    TIM4->CCMR1 |=  (1 << 11);       // Preload enable

    TIM4->CCER &= ~(1 << 5);         // Polarity high
    TIM4->CCER |=  (1 << 4);         // Enable CH2 output

    TIM4->BDTR |= (1 << 15);         // Main output enable (MOE)
}

// --- Motor 1 Control ---
void motor1(int sp_vect) {
    sp_vect = limit_var(sp_vect, -255, 255);

    if (sp_vect < -js_error) {
        motor_dir_1 |=  (1 << 14);     // DIR1 = High
        motor_dir_1 &= ~(1 << 15);     // DIR2 = Low
        sp_vect = -sp_vect;
    } else if (sp_vect > js_error) {
        motor_dir_1 &= ~(1 << 14);     // DIR1 = Low
        motor_dir_1 |=  (1 << 15);     // DIR2 = High
    } else {
        motor_dir_1 |=  (1 << 14) | (1 << 15);  // Both High = Stop
        sp_vect = 0;
    }

    motor_pwm1 = sp_vect;
}

// --- Motor 2 Control ---
void motor2(int sp_vect) {
    sp_vect = limit_var(sp_vect, -255, 255);

    if (sp_vect < -js_error) {
        motor_dir_2 |=  (1 << 1);      // DIR1 = High
        motor_dir_2 &= ~(1 << 3);      // DIR2 = Low
        sp_vect = -sp_vect;
    } else if (sp_vect > js_error) {
        motor_dir_2 &= ~(1 << 1);      // DIR1 = Low
        motor_dir_2 |=  (1 << 3);      // DIR2 = High
    } else {
        motor_dir_2 |=  (1 << 1) | (1 << 3);    // Both High = Stop
        sp_vect = 0;
    }

    motor_pwm2 = sp_vect;
}

// --- Limit Function ---
int limit_var(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

// --- Crude Delay Function ---
void _delay_ms(uint32_t time) {
    for (uint32_t i = 0; i < time * 2000; i++);
}
