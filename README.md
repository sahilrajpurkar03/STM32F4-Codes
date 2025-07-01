# STM32F407 Embedded Control Projects

A comprehensive set of STM32F407-based embedded projects showcasing essential peripherals and control techniques including ADC sampling, pushbutton handling with interrupts, digital input reading, LED blinking, PWM signal generation, and bidirectional DC motor control. Developed using STM32CubeMX for hardware configuration and Keil uVision for firmware development.

---

## Development Environment

- **IDE:** Keil uVision (latest recommended version)  
- **Hardware Abstraction:** STM32CubeMX for peripheral initialization and pin configuration  
- **Microcontroller:** STM32F407VGT6 (ARM Cortex-M4, 168 MHz)  
- **Compiler:** ARM Compiler (packaged with Keil)  
- **Debugger:** ST-Link V2 or compatible  
- **Clock Setup:** Internal and external oscillator with PLL configured for 168 MHz system clock  

---

## Supported Hardware & Components

| Component            | Description                                      | Reference / Datasheet                                |
|----------------------|------------------------------------------------|-----------------------------------------------------|
| STM32F407VGT6 MCU    | High-performance ARM Cortex-M4 MCU              | [STM32F407 Datasheet](https://www.st.com/resource/en/datasheet/stm32f407vg.pdf) |
| Pushbutton Switch    | Tactile switch with external interrupt support | User-supplied hardware                              |
| LEDs (on-board)      | User LEDs connected to GPIOD pins (e.g., PD12) | STM32F4 Discovery or custom board                   |
| DC Motor                  | Brushed DC motor                    | Controlled via PWM and direction pins                       |
| Rotary Encoder   | Incremental Rotary Encoder       | [Rotary Encoder](https://pankaj.com/wp-content/uploads/2016/03/NCP711-S_V.pdf) |

---

## Project Overview

This repository includes modular, well-documented examples illustrating the following functionalities:

### ADC Sampling and Conversion
- Configured ADC1 to sample analog inputs (e.g., potentiometer)
- Conversion of raw 12-bit ADC data to useful control values (e.g., PWM duty cycle)
- Blocking and polling-based ADC read example
  ### 📂 Code  
  - [ADC](./01.%20ADC/adc.c)

### Pushbutton Input Handling
- Digital input reading from GPIO pins for pushbuttons
- Software debouncing techniques
- Interrupt-driven pushbutton detection for responsive control
  ### 📂 Code  
  - [Push_button](./03.%20Push_Button/button_module.c)
  - [Push_button_interrupt](./03.%20Push_Button/button_module.c) 

### External Interrupt Configuration
- Configuration of EXTI lines to trigger on rising edges from pushbuttons or sensors
- EXTI Interrupt Service Routines (ISR) to handle events asynchronously
  ### 📂 Code  
  - [External_interrupt](./06.%20External_interrupt/external_int.c)

### Digital Input/Output Control
- Reading and writing digital pins for sensor inputs and actuator outputs
- Using GPIO registers for fast bit manipulation
  ### 📂 Code  
  - [Digital Input](./05.%20Digital_read/digita_read.c)

### LED Blinking and Status Indication
- Simple LED blinking loops for system status or debugging
- Control via GPIO output pins with delay-based timing
  ### 📂 Code  
  - [LED_blinking](./07.%20Led_blinking/led_blinking.c)

### PWM Generation using TIM4
- Initialization of Timer 4 for PWM mode on output compare channels
- Configuring GPIO pins to alternate function for PWM output
- Controlling duty cycle dynamically to adjust motor speed
  ### 📂 Code  
  - [PWM](./08.%20PWM/main.c)

### Bidirectional DC Motor Control
- H-bridge style direction control via GPIO pins
- Speed control through PWM duty cycle modulation
- Deadband handling and safety stop implementation
  ### 📂 Code  
  - [DC_motor](./09.%20tb_%20DC_motor/main.c)
---

## How to Build & Deploy

1. **Generate project code** using STM32CubeMX: configure clocks, GPIO, ADC, EXTI, TIM4 for PWM.
2. **Open the generated project** in Keil uVision.
3. **Add source files** from this repository into the Keil project.
4. **Build the project** with ARM Compiler.
5. **Flash the firmware** via ST-Link or compatible programmer.
6. **Connect peripherals:** pushbuttons, LEDs, motors, potentiometer as per schematic.
7. **Run and test** functionality as per examples.

---

## References and Tools

- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - Code generation and peripheral configuration  
- [Keil MDK-ARM](https://www.keil.com/download/) - Compiler and IDE for ARM Cortex-M MCUs  
- [STM32F4 Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020-stm32f405-415-stm32f407-417-stm32f427-437-and-stm32f429-439-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)  
- [STM32F407 Datasheet](https://www.st.com/resource/en/datasheet/stm32f407vg.pdf)  
- Application notes from ST for motor control, ADC, EXTI, and timer usage  

---


*Happy embedded programming and motor control with STM32F4!*
