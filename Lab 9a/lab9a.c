#include <msp430.h> // Header file

int main(void)
{
    int swDelay; // Variable for software delay
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer
    PM5CTL0 = 0xFFFE; // turn on I/O for FR6989
    P1OUT |= BIT0; // LED on
    P1DIR |= BIT0; // Set LED as output
    TA0CTL |= TASSEL_1 | MC_1 | TACLR; // ACLK, count to CCR0, clear register
    TA0CCR0 = 0x8000; // counter limit 0x8000 = 32768 = 1 second
    TA0CTL |= TAIE; // enable interrupt for Timer A0
    __enable_interrupt();       // Set GIE bit
    while (1)
    {
        for (swDelay = 0; swDelay < 100000; swDelay++)
        {
        }
    }
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer0_ISR(void) // Setting up the interrupt vector
{
    P1OUT ^= BIT0; // toggle LED
    TA0CTL &= ~ TAIFG; // reset the flag
}
