/*
 *
 *
 * Keypad/Pin Declaration
 * - Row 1 P5.0
 * - Row 2 P5.1
 * - Row 3 P5.2
 * - Row 4 P5.3
 * - Col 1 P2.0
 * - Col 2 P2.1
 * - Col 3 P2.2
 * - Col 4 P2.4
 */
#include <msp430.h>
#define ROW1 BIT0
#define ROW2 BIT1
#define ROW3 BIT2
#define ROW4 BIT3
#define COL1 BIT0
#define COL2 BIT1
#define COL3 BIT2
#define COL4 BIT4
#define TARGET_VAL 10
const char rowMask = ROW1 + ROW2 + ROW3 + ROW4;
const char colMask = COL1 + COL2 + COL3 + COL4;
// Function Declarations
void gpioInit();
char readCols();
char readRows();
void checkValue(char val);
char keyValue(char row, char col);
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    gpioInit();
    PM5CTL0 &= ~LOCKLPM5;
    char col = 0;
    char row = 0;
    char val = 0;
    while(1){
        col = readCols();
        row = readRows();
        val = keyValue(row, col);
        checkValue(val);
    }
    return 0;
}
void gpioInit(){
    //Initialize Keypad Pins
    P5REN &= ~rowMask;  // Disable Pulldown Resistor
    P5DIR |= rowMask;   // Set rows outputs
    P5OUT |= rowMask;   // Set all Rows to 1
    P2DIR &= ~colMask;  // Set cols to inputs
    P2REN |= colMask;   // Set cols to use Pulldown resistor
    P2OUT &= ~colMask;
    P1DIR |= BIT0;  // Set LED0 to output
    P1OUT &= ~BIT0; // Turn LED Off
}
char readCols(){
    P5REN &= ~rowMask;  // Disable Pulldown Resistor
    P5DIR |= rowMask;   // Set rows outputs
    P5OUT |= rowMask;   // Set all Rows to 1
    P2DIR &= ~colMask;  // Set cols to inputs
    P2REN |= colMask;   // Enable Pulldown Resistors
    P2OUT &= ~colMask;
    char cols = P2IN & colMask;
    switch(cols){
    case COL1:  return 0;
                break;
    case COL2:  return 1;
                break;
    case COL3:  return 2;
                break;
    case COL4:  return 3;
                break;
    default:    return 0xFF;
                break;
    }
}
char readRows(){
    P2REN &= ~colMask;
    P2DIR |= colMask;   // Set cols outputs
    P2OUT |= colMask;   // Set all cols to 1
    P5DIR &= ~rowMask;  // Set rows to inputs
    P5REN |= rowMask;
    P5OUT &= ~rowMask;
    char rows = P5IN & rowMask;
    switch(rows){
    case ROW1:  return 0;
                break;
    case ROW2:  return 1;
                break;
    case ROW3:  return 2;
                break;
    case ROW4:  return 3;
                break;
    default:    return 0xFF;
                break;
    }
}
char keyValue(char row, char col){
    return (row << 2)+col;
}
void checkValue(char val){
    if (val == TARGET_VAL)
        P1OUT |= BIT0;
    else
        P1OUT &= ~BIT0;
}

