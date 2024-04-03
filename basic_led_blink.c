#include <msp430.h>

//#define DELAYCYCLES 100000

const unsigned long delayCycles = 100000;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

    P6OUT &= ~BIT6;                         // Clear P1.0 output latch for a defined power-on state
    P6DIR |= BIT6;                          // Set P1.0 to output direction

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    while(1)
    {
        P6OUT ^= BIT6;                      // Toggle P1.0 using exclusive-OR
        __delay_cycles(delayCycles);             // Delay for 100000*(1/MCLK)=0.1s
    }
}
