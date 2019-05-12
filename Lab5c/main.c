#include <msp430.h> 

void main(void)
{
     WDTCTL = WDTPW | WDTHOLD;      //Stop watch dog timer
     PM5CTL0 = 0xFFFE;              //To enable inputs and outputs
     P1DIR|=BIT0;                   //Set LED (P1.0) to output
     TA0CTL|=TASSEL_1|MC_1|TACLR;   //Set up Timer_A
     TA0CCR0=0x8000;                //Set the max value for Timer_A
     while(1)                       //Infinite Loop
     {
         if(TA0CTL&TAIFG)           //If Timer_A has its interrupt flag set
         {
            P1OUT^=BIT0;            //Toggle LED
            TA0CTL&=~BIT0;          //Reset Timer_A
         }
     }
}
