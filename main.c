#include "stm32f4xx.h"

// Function prototypes
void init_gpio(void);
void init_adc(void);
void configure_interrupt(void);
void EXTI0_IRQHandler(void); // Declare the interrupt handler

volatile int start_system = 0; // Flag to indicate when the system should start
volatile int adcData; // Make adcData volatile to ensure it's updated in the debugger
const int threshold = 1024;

void delay(int time) {
    for (int i = 0; i < time * 1000; i++);
}

int main(void) {
    init_gpio();       // Initialize GPIOs
    init_adc();        // Initialize ADC
    configure_interrupt(); // Configure external interrupt

    // Wait for the button press to start the system
    while (!start_system) {
        // Optionally, you can toggle an LED to indicate waiting state
        delay(100); // Delay for visibility
    }

    // Main loop
    while (1) {
        int pir_detected = 0;
        int ultrasonic_detected = 0;
        int ldr_detected = 0;

        // Check Motion Sensor (PIR) on PA3
        if (GPIOA->IDR & (1U << 3)) { // If motion detected on PA3
            pir_detected = 1;
        }

        // Ultrasonic Sensor: Send trigger pulse
        GPIOA->ODR |= (1U << 4);  // Set PA4 high (Trigger)
        delay(10);                 // Short delay ~10 µs
        GPIOA->ODR &= ~(1U << 4); // Set PA4 low (Trigger)

        // Check for echo on PA5
        delay(1); // Adding a delay to wait for the echo response
        if (GPIOA->IDR & (1U << 5)) { // If echo detected on PA5
            ultrasonic_detected = 1;
        }

        // LDR Sensor on PA1 (Analog)
        ADC1->CR2 |= (1 << 30); // Start ADC conversion
        while (!(ADC1->SR & (1 << 1))) {}  // Wait until ADC conversion is complete
        adcData = ADC1->DR;  // Read ADC value

        // Check LDR threshold
        if (adcData <= threshold) {
            ldr_detected = 1;
        }

        // Set or clear the LED on PA10 based on LDR detection
        if (ldr_detected) {
            GPIOA->ODR &=~ (1U << 10); // Turn on LED on PA10
        } else {
            GPIOA->ODR |= (1U << 10); // Turn off LED on PA10
        }

        // Set or clear the LED on PA2 based on PIR detection
        if (pir_detected) {
            GPIOA->ODR |= (1U << 2); // Turn on LED on PA2
        } else {
            GPIOA->ODR &= ~(1U << 2); // Turn off LED on PA2
        }

        // Set or clear the LED on PA7 based on Ultrasonic detection
        if (ultrasonic_detected) {
            GPIOA->ODR |= (1U << 7); // Turn on LED on PA7
        } else {
            GPIOA->ODR &= ~(1U << 7); // Turn off LED on PA7
        }

        delay(10); // Optional small delay to debounce
    }
}

void init_gpio(void) {
    RCC->AHB1ENR |= (1U << 0); // Enable GPIOA clock

    // Configure PA0 as input (Push button)
    GPIOA->MODER &= ~(3U << (2 * 0)); // Clear mode bits for PA0
    GPIOA->PUPDR &= ~(3U << (2 * 0)); // Clear pull-up/pull-down bits for PA0
    GPIOA->PUPDR |= (1U << (2 * 0)); // Set pull-up resistor for PA0

    // Motion Sensor (PIR) on PA3
    GPIOA->MODER &= ~(3U << (2 * 3)); // Set PA3 as input

    // Ultrasonic Sensor
    GPIOA->MODER |= (1U << (2 * 4));  // Set PA4 as output (Trigger)
    GPIOA->MODER &= ~(3U << (2 * 5)); // Set PA5 as input (Echo)

    // LDR Sensor
    GPIOA->MODER |= (3U << (2 * 1)); // Set PA1 to analog mode

    // LEDs
    GPIOA->MODER |= (1U << (2 * 2)); // Set PA2 as output for PIR LED
    GPIOA->MODER |= (1U << (2 * 7)); // Set PA7 as output for Ultrasonic LED
    GPIOA->MODER |= (1U << (2 * 10)); // Set PA10 as output for LDR LED
}

void init_adc(void) {
    RCC->APB2ENR |= (1 << 8); // Enable ADC1 clock

    ADC1->SMPR2 = 0; // Set sampling time to 3 cycles for channel 1 (PA1)
    ADC->CCR = 0; // Set ADC prescaler to PCLK2/2 (default)
    ADC1->CR1 = 0; // Reset CR1 register
    ADC1->CR2 = 0; // Reset CR2 register
    ADC1->CR2 |= (1U << 1); // Enable continuous conversion mode
    ADC1->SQR3 = 1; // Choose channel 1 (PA1) for ADC
    ADC1->SQR1 = 0; // Set conversion sequence length to 1
    ADC1->CR2 |= 1; // Enable ADC

    while((ADC1->CR2 & 1) == 0) {} // Wait until ADC is ready
    ADC1->CR2 |= (1 << 30); // Start the conversion
}

// EXTI0 interrupt handler
void EXTI0_IRQHandler(void) {
    if (EXTI->PR & (1U << 0)) { // Check if EXTI line 0 is pending
        EXTI->PR |= (1U << 0); // Clear the pending bit
        start_system = 1; // Set flag to start the system
    }
}

void configure_interrupt(void) {
    RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN; // Enable SYSCFG clock

    SYSCFG->EXTICR[0] &= ~(0x0F); // Clear EXTI0 configuration
    SYSCFG->EXTICR[0] |= (0x00); // Connect EXTI0 line to PA0

    EXTI->IMR |= (1U << 0); // Unmask interrupt line 0
    EXTI->FTSR |= (1U << 0); // Falling edge trigger for EXTI line 0

    NVIC_EnableIRQ(EXTI0_IRQn); // Enable EXTI0 interrupt in NVIC
}
