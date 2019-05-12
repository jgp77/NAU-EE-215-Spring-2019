#include <msp430.h> 


void main(void)
{
     WDTCTL = WDTPW | WDTHOLD;  //Stop watch dog timer
     PM5CTL0 = 0xFFFE;          //To enable inputs and outputs
     P1DIR|=BIT0;               //Set LED (P1.0) to output
     P1DIR&=~BIT1;              //Set SW1 (P1.1) to input
     P1REN|=BIT1;               //Enable resistor in P1.1
     P1OUT|=BIT1;               //Set the resistor as a pull-up resistor
     while(1)                   //Infinite Loop
     {
         if(~P1IN & BIT1)       //If the button is pressed
         {
             P1OUT|=BIT0;       //Turn on LED
         }
         else
         {
             P1OUT&=~BIT0;      //Turn off LED
         }
     }
}
