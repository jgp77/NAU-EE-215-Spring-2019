#include <msp430.h>
#include <msp430fr6989.h>
#define UART_CLK_SEL 0x0080 // Specifies accurate SMCLK clock for UART
#define BR0_FOR_9600 0x34 // Value required to use 9600 baud
#define BR1_FOR_9600 0x00 // Value required to use 9600 baud
#define CLK_MOD 0x4911 // Microcontroller will "clean-up" clock signal
void select_clock_signals(void); // Assigns microcontroller clock signals
void assign_pins_to_uart(void); // P4.2 is for TXD, P4.3 is for RXD
void use_9600_baud(void); // UART operates at 9600 bits/second

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // Stop WDT
    PM5CTL0 = 0xFFFE; // Enable pins on FR6989

    select_clock_signals(); // Assigns microcontroller clock signals
    assign_pins_to_uart(); // P4.2 is for TXD, P4.3 is for RXD
    use_9600_baud(); // UART operates at 9600 bits/second

    P9OUT &= ~BIT7; // turn off LED2
    P9DIR |= BIT7; // set LED2 as output
    P1OUT &= ~BIT0; // LED off
    P1DIR |= BIT0; // Set LED as output

    P1DIR &= ~BIT1; //Set SW1 (P1.1) to input
    P1REN |= BIT1; //Enable resistor in P1.1
    P1OUT |= BIT1; //Set the resistor as a pull-up resistor
    P1IES |= BIT1; // Select Hi->Lo
    P1IE |= BIT1; // Enable the interrupt on Port 1

    P1DIR &= ~BIT2; //Set SW1 (P1.1) to input
    P1REN |= BIT2; //Enable resistor in P1.1
    P1OUT |= BIT2; //Set the resistor as a pull-up resistor
    P1IES |= BIT2; // Select Hi->Lo
    P1IE |= BIT2; // Enable the interrupt on Port 1
    P1IFG = 0x00; // Reset interrupt flagof port 1

    TA0CTL |= TASSEL_1 | MC_1 | TACLR; // ACLK, count to CCR0, clear register
    TA0CCR0 = 0x4000; // counter limit 0x4000 = 16384 = 0.5 second

    UCA0IE = UCRXIE; // Enable interrupt for UCA0
    UCA0IFG &= ~ UCRXIFG; //Reset interrupt flag for UCA0

    __enable_interrupt();// Enable GIE bit

    while (1)// Infinite loop to wait for interrupts
    {
    }
}

/*
 * This is the interrupt vector for UART communication/the USCI. In here
 * we used a switch case like the book said to use. Since we are only using
 * one case (0x02 for receiving) we don't necessarily need to use this.
 * Instead an if statement can probably be used. The switch case is handy
 * for handling receiving, transmitting, and a default case. This was found on
 * page 782 of the 6989 user guide.
 */
#pragma vector=USCI_A0_VECTOR
__interrupt void UARTCOM(void)
{
    switch (__even_in_range(UCA0IV, 18))
    {
        case 0x02:
            if (UCA0RXBUF == 0x56) // If the message is 0x56
            {
                P1OUT |= BIT0; // turn on LED1
                while (!(TA0CTL & TAIFG)){}
                TA0CTL &= ~BIT0;          //Reset Timer_A
                P1OUT &= ~BIT0; // turn off LED2
            }
            else if (UCA0RXBUF == 0xA3) // If the message is 0x56
            {
                P9OUT |= BIT7; // turn on LED2
                while (!(TA0CTL & TAIFG)){}
                TA0CTL &= ~BIT0;          //Reset Timer_A
                P9OUT &= ~BIT7; // turn off LED1
            }
            else // Else, the message is not either
            {
                P9OUT &= ~BIT7; // turn on LED2
                P1OUT &= ~BIT0; // turn on LED2
            }
            UCA0IFG &= (~UCRXIFG); // Reset the UART receive flag
            break;

        default:
            break;
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1(void) // Setting up ISR for port 1
{
    if (P1IFG & BIT1)// If the interrupt came from switch 1
    {
        UCA0TXBUF = 0x56; // Send the UART message 0x56 out pin P4.2
        P1IFG &= ~BIT1; // Reset interrupt flag to 0
    }
    else if (P1IFG & BIT2)// Else if the interrupt came from switch 2
    {
        UCA0TXBUF = 0xA3; // Send the UART message 0x56 out pin P4.2
        P1IFG &= ~BIT2; // Reset interrupt flag to 0
    }
}

void select_clock_signals(void)
{
    CSCTL0 = 0xA500; // "Password" to access clock calibration registers
    CSCTL1 = 0x0046; // Specifies frequency of main clock
    CSCTL2 = 0x0133; // Assigns additional clock signals
    CSCTL3 = 0x0000; // Use clocks at intended frequency, do not slow them down
}

void assign_pins_to_uart(void)
{
    P4SEL1 = 0x00; // 0000 0000
    P4SEL0 = BIT3 | BIT2; // 0000 1100 Assigns p4.2 to transmit and P4.3to recieve
}

void use_9600_baud(void)
{
    UCA0CTLW0 = UCSWRST; // Put UART into SoftWare ReSeT
    UCA0CTLW0 = UCA0CTLW0 | UART_CLK_SEL; // Specifies clock source for UART
    UCA0BR0 = BR0_FOR_9600; // Specifies baud rate of 9600
    UCA0BR1 = BR1_FOR_9600; // Specifies baud rate of 9600
    UCA0MCTLW = CLK_MOD; // "Cleans" clock signal
    UCA0CTLW0 = UCA0CTLW0 & (~UCSWRST); // Takes UART out of SoftWare ReSeT
}
