/*
 * Christian Hecht
 * Homework: Button Changing Brightness
 * Date Submitted: 2/23/24
 * Description: This program initializes with both LEDs off. The brightness of each button
 * is then adjusted via the buttons. Button 4.1 will change the brightness of an LED,
 * Button 2.3 will change which LED is being targeted by the Button 4.1 Interrupt.
 * The timers for the LEDs have 10 different brightnesses based on their period.
 */

#include <msp430.h>

unsigned int RLED_BLINK_RATE = 0;
unsigned int GLED_BLINK_RATE = 0;
unsigned int LED_SELECT = 2;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                     // Stop WDT

    // Configure GPIO
    //LED enable
    P1DIR |= BIT0;                                // P1.0 output
    P1OUT &= ~BIT0;                                // P1.0 low
    P6DIR |= BIT6;                                // P6.6 output
    P6OUT &= ~BIT6;                                // P6.6 low
    //Button enable
    P4DIR &= ~BIT1;                         // Set P4.1 (L Button) as input
    P4REN |= BIT1;                          // Enable resistor for P4.1
    P4OUT |= BIT1;                          // Enable voltage source
    P4IE |= BIT1;                           // Enable interrupts for this pin
    P4IES |= BIT1;                          // Interrupt on falling edge.


    P2DIR &= ~BIT3;                         // Set P2.3 (R Button) as input
    P2REN |= BIT3;                          // Enable resistor for P23.
    P2OUT |= BIT3;                          // Enable voltage source
    P2IE |= BIT3;                           // Enable interrupts for this pin
    P2IES |= BIT3;                          // Interrupt on falling edge.

    // 0 is low to high, 1 is high to low interrupt

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    //Red LED Timer
    TB0CCTL0 |= CCIE;                             // TBCCR0 interrupt enabled
    TB0CCR0 = RLED_BLINK_RATE;
    TB0CTL |= TBSSEL__SMCLK | MC__CONTINUOUS;     // SMCLK, continuous mode
    //Green LED Timer
    TB1CCTL0 |= CCIE;                             // TBCCR1 interrupt enabled
    TB1CCR0 = GLED_BLINK_RATE;
    TB1CTL |= TBSSEL__SMCLK | MC__CONTINUOUS;     // SMCLK, continuous mode

    __bis_SR_register(LPM0_bits | GIE);           // Enter LPM3 w/ interrupts
    __no_operation();                             // For debug
}

// Timer B0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_R (void)
{
    if(LED_SELECT==0||LED_SELECT==1){
    P1OUT ^= BIT0;
    TB0CCR0 += RLED_BLINK_RATE;                             // Add Offset to TBCCR0
    }
}

// Timer B1 interrupt service routine
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer_G (void)
{
    if(LED_SELECT==0||LED_SELECT==1){
    P6OUT ^= BIT6;
    TB1CCR0 += GLED_BLINK_RATE;                             // Add Offset to TBCCR0
    }
}


// Button 4.1 Interrupt - Speed up timer
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;  //Clear flag so that the button is available for interrupt
    if (!(P4IN & BIT1))                   // if L button is pressed
        if(LED_SELECT==0)
        RLED_BLINK_RATE = RLED_BLINK_RATE + 6455; // Increase timer speed
        else if(LED_SELECT==1)
        GLED_BLINK_RATE = GLED_BLINK_RATE + 6455; // Increase timer speed
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}

// Button  2.3 change LED
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;  //Clear flag so that the button is available for interrupt
    if (!(P2IN & BIT3))                   // if R button is pressed
        if(LED_SELECT==0)
            LED_SELECT = 1;
        else if(LED_SELECT==1||LED_SELECT==2)
            LED_SELECT = 0;
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}

