#include <msp430.h> // Header file

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    PM5CTL0 = 0xFFFE; // turn on I/O for FR6989
    P1OUT |= BIT0; // LED on
    P1DIR |= BIT0; // Set LED as output

    P1DIR &= ~BIT1; //Set SW1 (P1.1) to input
    P1REN |= BIT1; //Enable resistor in P1.1
    P1OUT |= BIT1; //Set the resistor as a pull-up resistor
    P1IES |= BIT1; // Select Hi->Lo			
    P1IE |= BIT1; // Enable the interrupt on Port 1
    P1IFG = 0x00; // Reset interrupt flagof port 1

    __enable_interrupt();       // Set GIE bit
    while (1)
    {
        LPM3; // Macro to turn on LPM3
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1(void) // Setting up ISR for port 1
{
    P1OUT ^= BIT0; // toggle LED
    P1IFG &= ~BIT1; // Reset interrupt flag to 0
}
