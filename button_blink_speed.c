/*
 * Program Name
 *
 * Program Description
 *
 * Ports Used
 *
 *
 * Author Name
 * Last Edited
 */

#include <msp430.h>

// Function Declarations
void gpioInit();                // Used to setup Pins
void enableButtonInterrupts();  // Enable Interrupts on Buttons
void timerB0Init();             // Used to setup TimerB0

// Global Variables
volatile unsigned int BlinkPeriod = 30000;
const unsigned int RateAdjustment = 5000;

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    gpioInit();
    timerB0Init();
    enableButtonInterrupts();
        PM5CTL0 &= ~LOCKLPM5;       // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

     // Put CPU to Sleep and never run main() again
    __bis_SR_register(LPM0_bits | GIE);           // Enter LPM3 w/ interrupts
    __no_operation();                             // For debug
    return 0;
}

void gpioInit(){
        // Input Config
        P4DIR &= ~BIT1; // Set as Input
        P4REN |= BIT1; // Enable Pullup
        P4OUT |= BIT1;
        P4SEL0 &= ~BIT1; // Ensure Sel Reg
        P4SEL1 &= ~BIT1;

        P2DIR &= ~BIT3; // Set as Input
        P2REN |= BIT3; // Enable Pullup
        P2OUT |= BIT3;
        P2SEL0 &= ~BIT3; //Ensure Sel Reg
        P2SEL1 &= ~BIT3;

        //Output Config
        P1DIR |= BIT0; //Set as Output
        P1OUT &= ~BIT0; // Clear Output latch
        P1SEL0 &= ~BIT0; // Ensure Sel Reg
        P1SEL1 &= ~BIT0;

        P6DIR |= BIT6; // Set as Output
        P6OUT &= ~BIT6;  // Clear Output latch
        P6SEL0 &= ~BIT6; // Ensure Sel reg
        P6SEL1 &= ~BIT6;

}

void enableButtonInterrupts(){
    P4IES |= BIT1; // High to Low Edge Select
    P4IE |= BIT1; // Interrupts Enabled
    P4IFG &= ~BIT1; // Clear IFG

    P2IES |= BIT3; // High to Low Edge Select
    P2IE |= BIT3;
    P2IFG &= ~BIT3;
}

void timerB0Init(){
    TB0CCTL0 |= CCIE;                                       // TBCCR0 interrupt enabled
    TB0CCR0 = 30000;                                        // Initial count of 30000
    TB0CTL |= TBSSEL__SMCLK | MC__CONTINUOUS | ID__4;       // SMCLK, continuous mode, ID = 4
    TB0EX0 |= TBIDEX_3;                                     // Extended Divder set to 4, total Div of 16
    TB0CTL |= TBCLR;                                        // Clear Timer register per User Guide recommendation
}

#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1; // Clear interuppt flag
    if (BlinkPeriod < 55000){ // Blink Period cannot exceed this
        BlinkPeriod += RateAdjustment; // Increase Period, Decrease Speed
    }
    else
        BlinkPeriod+=0; // Blink Period does not change if threshold is met
}


#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3; // Clear Interrupt Flag
    if (BlinkPeriod > 5000){ // Blink Period can not go below this
        BlinkPeriod -= RateAdjustment; // Decrease period, increase speed
    }
    else
        BlinkPeriod+=0; // Blink Period does not change if threshold is met
}


// Timer B0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
{
    P1OUT ^= BIT0;              // Toggle LED
    TB0CCR0 += BlinkPeriod;                // Offset is the Period
    }

