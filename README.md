# Smart-Sensor-based-Automatic-Lighting-Intrusion-Detection-System

# STM32 Sensor-Based System

This STM32F4-based embedded C project uses three different sensors to detect environmental inputs and respond using GPIO LEDs.
# Three Sensors
  1.PIR Motion Sensor
  2.Ultrasonic Distance Sensor
  3.LDR (Light Dependent Resistor)

## Features

- **Button-based Start** (PA0)
- **PIR Motion Detection** (PA3)
- **Ultrasonic Sensor**:
  - Trigger on PA4
  - Echo on PA5
- **LDR Light Detection** (Analog PA1)
- **Visual Feedback** via LEDs:
  - PA2: PIR
  - PA7: Ultrasonic
  - PA10: LDR

## Components Used

- STM32F4xx Microcontroller
- Passive IR (PIR) Motion Sensor
- Ultrasonic Distance Sensor (HC-SR04)
- LDR with voltage divider
- Push Button
- LEDs

## Pin Configuration

| Function           | GPIO Pin |
|--------------------|----------|
| Push Button (start)| PA0      |
| PIR Input          | PA3      |
| Ultrasonic Trigger | PA4      |
| Ultrasonic Echo    | PA5      |
| LDR Analog Input   | PA1      |
| LED for PIR        | PA2      |
| LED for Ultrasonic | PA7      |
| LED for LDR        | PA10     |

## How it Works

- The system begins only when the push button (PA0) is pressed.
- In the loop:
  - **PIR Sensor**: If motion is detected, LED on PA2 lights up.
  - **Ultrasonic Sensor**: Sends a trigger pulse and listens for echo. If detected, LED on PA7 turns on.
  - **LDR Sensor**: Uses ADC to read light intensity; if below threshold (1024), LED on PA10 turns **ON**.

## Interrupts

- Configured EXTI0 for push-button start (falling edge interrupt).

## How to Build & Flash

1. Use STM32CubeIDE or Keil uVision for compiling and flashing.
2. Alternatively, use ARM GCC toolchain with your makefile and flash using `st-flash`.
