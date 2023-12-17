#include <stm32f4xx.h>

void eint_init(void);
void EXTI0_IRQHandler(void);
void port_init(void);
void _delay_ms(uint32_t time);
void read_port(void);

uint8_t button[8] = {0, 0, 0, 0, 0, 0, 0, 0};

int main(void) {
  port_init();
  eint_init();

  while (1) {
    read_port();
  }
}

void port_init(void) {
  RCC->APB2ENR |= (1 << 14); //enable configure
  RCC->AHB1ENR |= (1 << 0);  //enable gpioa
  
  GPIOA->MODER &= ~0xFFFF0000; //PA0-7 SET AS INPUT MODE
}

void eint_init(void) {
  SYSCFG->EXTICR[0] &= ~(1 << 0) & (1 << 1) & (1 << 2) & (1 << 3); //SOURCE INPUT FOR EXTERNAL INPUT
  EXTI->IMR |= 0x000000FF;                                         //LINE IS NOT MASKED
  EXTI->EMR |= 0x000000FF;                                         //
  EXTI->RTSR |= 0x000000FF;                                        //RISING EDGE ENABLE
  NVIC_EnableIRQ(EXTI0_IRQn);
}

void read_port(void) {
  if (button[0]) {

    button[0] = 0;
  }
  if (button[1]) {
    button[1] = 0;
  }
  if (button[2]) {
    button[2] = 0;
  }
  if (button[3]) {
    button[3] = 0;
  }
  if (button[4]) {
    button[4] = 0;
  }
  if (button[5]) {
    button[5] = 0;
  }
  if (button[6]) {
    button[6] = 0;
  }
  if (button[7]) {
    button[7] = 0;
  }
}

void EXTI0_IRQHandler(void) {
  if (EXTI->PR & (1 << 0)) {
    EXTI->PR |= (1 << 0); // clear pending interrupt
    button[0] = 1;
  }
  if (EXTI->PR & (1 << 1)) {
    EXTI->PR |= (1 << 1); // clear pending interrupt
    button[1] = 1;
  }
  if (EXTI->PR & (1 << 2)) {
    EXTI->PR |= (1 << 2); // clear pending interrupt
    button[2] = 1;
  }
  if (EXTI->PR & (1 << 3)) {
    EXTI->PR |= (1 << 3); // clear pending interrupt
    button[3] = 1;
  }
  if (EXTI->PR & (1 << 4)) {
    EXTI->PR |= (1 << 4); // clear pending interrupt
    button[4] = 1;
  }
  if (EXTI->PR & (1 << 5)) {
    EXTI->PR |= (1 << 5); // clear pending interrupt
    button[5] = 1;
  }
  if (EXTI->PR & (1 << 6)) {
    EXTI->PR |= (1 << 6); // clear pending interrupt
    button[6] = 1;
  }
  if (EXTI->PR & (1 << 7)) {
    EXTI->PR |= (1 << 7); // clear pending interrupt
    button[7] = 1;
  }
}