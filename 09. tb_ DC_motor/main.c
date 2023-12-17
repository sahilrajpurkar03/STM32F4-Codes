#include <stm32f4xx.h>

#define motor_dir_1 GPIOD->ODR
#define motor_dir_2 GPIOC->ODR
#define motor_pwm1 TIM4->CCR1
#define motor_pwm2 TIM4->CCR2

void motor1();
void motor2();
void port_init();
void timer_init();
void pwm_init();
void _delay_ms(uint32_t time);
long limit_var(long in_var, long l_limit, long h_limit);

int js_error = 20;

int main(void) {
  port_init();
  timer_init();
  pwm_init();
  while (1) {
    motor1(50);
    motor2(50);
    _delay_ms(5000);
    motor1(-200);
    motor2(-200);
    _delay_ms(5000);
  }
}

void port_init() {
  RCC->APB2ENR |= (1 << 14);           //enable configure
  RCC->AHB1ENR |= (1 << 3) | (1 << 2); //enable gpiod and gpioc

  GPIOD->MODER |= (1 << 25) | (1 << 27); //PD12 and PD13 set as AF
  GPIOD->PUPDR |= (1 << 25) | (1 << 27);
  GPIOD->AFR[1] |= (1 << 17) | (1 << 21);

  GPIOD->MODER |= (1 << 18) | (1 << 20);  //PD9 and PD10 set as output
  GPIOD->OSPEEDR |= 0x003C0000;           //IO speed= high speed
  GPIOD->OTYPER &= ~(1 << 9) & (1 << 10); //Push-pull reset state

  GPIOC->MODER |= (1 << 2) | (1 << 6);   //PC1 and PC3 set as output
  GPIOD->OSPEEDR |= 0x000000CC;          //IO speed= high speed
  GPIOD->OTYPER &= ~(1 << 1) & (1 << 3); //Push-pull reset state
}

void pwm_init() {
  //SELECT THE OUTPUT COMPARE MODE
  TIM4->CCMR1 |= (1 << 6) | (1 << 5) | (1 << 14) | (1 << 13); //PWM MODE 1

  //SET OUTPUT STATE
  TIM4->CCER |= (1 << 0) | (1 << 4); //OUTPUT ENABLE

  //ENABLE OUTPUT COMPARE PRELOAD FEATURE
  TIM4->CR1 |= (1 << 7);               //Auto-reload preload enable
  TIM4->CCMR1 |= (1 << 3) | (1 << 11); //Preload register enabled

  TIM4->BDTR |= (1 << 15); //MOE=1;
}

void timer_init() {
  RCC->APB1ENR |= (1 << 2); //ENABLE TIMER 4

  //SET AUTO RELOAD VALUE
  TIM4->ARR = 255;

  //SET PRESCALAR VALUE
  TIM4->PSC = 0; //PRESCALAR = 0

  //SET REPETITION COUNTER VALUE
  TIM4->RCR = 0; //REPETITION COUNTER VALUE = 0

  TIM4->EGR |= (1 << 0); //

  TIM4->CR1 |= (1 << 0); //COUNTER ENABLE
}

void motor1(int sp_vect) {
  sp_vect = limit_var(sp_vect, -255, 255);
  if (sp_vect < (-js_error)) {
    motor_dir_1 |= (1 << 9);
    motor_dir_1 &= ~(1 << 10);
    sp_vect = (-sp_vect);
  } else if (sp_vect > js_error) {
    motor_dir_1 &= ~(1 << 9);
    motor_dir_1 |= (1 << 10);
  } else {
    motor_dir_1 |= (1 << 10);
    motor_dir_1 |= (1 << 9);
    sp_vect = 0;
  }
  motor_pwm1 = sp_vect;
}

void motor2(int sp_vect) {
  sp_vect = limit_var(sp_vect, -255, 255);
  if (sp_vect < (-js_error)) {
    motor_dir_2 |= (1 << 1);
    motor_dir_2 &= (1 << 3);
    sp_vect = (-sp_vect);
  } else if (sp_vect > js_error) {
    motor_dir_2 &= (1 << 1);
    motor_dir_2 |= (1 >> 3);
  } else {
    motor_dir_2 |= (1 << 1);
    motor_dir_2 |= (1 >> 3);
    sp_vect = 0;
  }
  motor_pwm2 = sp_vect;
}

long limit_var(long in_var, long l_limit, long h_limit) {
  if (in_var > h_limit) {
    in_var = h_limit;
  } else if (in_var < l_limit) {
    in_var = l_limit;
  }
  return in_var;
}

void _delay_ms(uint32_t time) {
  for (uint32_t i = 0; i < time * 2000; i++)
    ;
}