/**
 * Name: Christian Hecht
 * Course: Embedded Systems Section 2
 * Project: Blinking Both LEDS with Buttons (Polling)
 * Description: This program reads the input from the left button using polling.
 * The signal from the button is then used to switch LED1 on/off.
 *
 */

#include <msp430.h>

void main(void)
{
    //Pin Initialization
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P1OUT &= ~BIT0;                          // Set LED1 to 0 (off)
    P1DIR |= BIT0;                          // Set P1.0 (LED1) to output direction

    P4DIR &= ~BIT1;                         // Set P4.1 (L Button) as input
    P4REN |= BIT1;                          // Enable resistor for P4.1
    P4OUT |= BIT1;                          // Enable voltage source

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings


    //Main Loop
    while (1)                               // Test
    {
        if (P4IN & BIT1)                   // if button is pressed
            P1OUT &= ~BIT0;                  // Set LED to 1 (on)
        else
            P1OUT |= BIT0;                     // else reset to 0 (off)
    }
}
