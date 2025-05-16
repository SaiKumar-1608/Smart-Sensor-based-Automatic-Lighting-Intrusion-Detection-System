# Smart Sensor-Based Automatic Lighting & Intrusion Detection System

An embedded C project built for the **STM32F4** microcontroller, using three types of sensors to detect environmental changes and provide visual feedback via LEDs.

---

## 📖 Overview

This project demonstrates how embedded systems can integrate multiple sensors to build an automated environment-aware system. It activates upon a push button interrupt and performs continuous monitoring of:

- **Motion** via a PIR sensor
- **Distance** using an Ultrasonic sensor
- **Light level** through an LDR and ADC

Each sensor is mapped to a specific LED output, offering intuitive feedback for real-time detection.

---

##  Features

- **Start Trigger via Button (PA0)**
- **Motion Detection via PIR Sensor (PA3)**
- **Object Detection via Ultrasonic Sensor (Trigger: PA4, Echo: PA5)**
- **Light Intensity Detection via LDR (Analog Input: PA1)**
- **Visual Indication via LEDs**  
  - PIR → PA2  
  - Ultrasonic → PA7  
  - LDR → PA10  

---

##  Sensors & Components Used

- STM32F4xx Microcontroller (e.g., STM32F401, STM32F407)
- Passive Infrared (PIR) Motion Sensor
- HC-SR04 Ultrasonic Distance Sensor
- LDR (Light Dependent Resistor) with voltage divider circuit
- Push Button
- 3 LEDs
- Resistors & Breadboard

---

##  Pin Configuration

| Component          | GPIO Pin |
|--------------------|----------|
| Push Button (Start)| PA0      |
| PIR Sensor         | PA3      |
| Ultrasonic Trigger | PA4      |
| Ultrasonic Echo    | PA5      |
| LDR Analog Input   | PA1      |
| LED for PIR        | PA2      |
| LED for Ultrasonic | PA7      |
| LED for LDR        | PA10     |

---

##  How It Works

1. The system remains in idle state until the **Push Button** (PA0) is pressed (via EXTI0 interrupt).
2. After activation:
   - **PIR Sensor (PA3)**  
     If motion is detected, the LED on **PA2** turns **ON**.
   - **Ultrasonic Sensor (PA4 & PA5)**  
     Sends a 10μs trigger pulse on **PA4** and listens for echo on **PA5**. If echo is detected, LED on **PA7** turns **ON**.
   - **LDR Sensor (PA1 - ADC)**  
     Uses ADC to measure light intensity. If the value is **below 1024** (i.e., it's dark), the LED on **PA10** turns **ON**.

---

## 🛠️ Interrupt Configuration

- **EXTI Line 0** (connected to PA0) is configured to trigger on **falling edge**.
- Enables system start through external interrupt handler.

---

## 🧪 Build and Flash Instructions

### Option 1: STM32CubeIDE
- Open the project in **STM32CubeIDE**
- Build → Flash via **ST-Link debugger**

### Option 2: Keil uVision
- Import source files into a new Keil project.
- Configure target and flash via ULINK or ST-Link.

### Option 3: GCC Toolchain (Command Line)
```bash
# Compile
arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -o main.elf main.c ...

# Flash (using st-flash)
st-flash write main.bin 0x8000000

---

## License
This project is licensed under the MIT License. See the `LICENSE` file for details.
