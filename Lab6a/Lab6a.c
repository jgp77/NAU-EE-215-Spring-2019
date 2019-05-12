#include <msp430.h> 

void main(void)
{
     WDTCTL = WDTPW | WDTHOLD;      //Stop watch dog timer
     PM5CTL0 = 0xFFFE;              //To enable inputs and outputs

     P1DIR|=BIT0;                   //Set LED1 (P1.0) to output
     P9DIR|=BIT7;                   //Set LED2 (P9.2) to output

     P1DIR&=~BIT1;                  //Set SW1 (P1.1) to input
     P1REN|=BIT1;                   //Enable resistor in P1.1
     P1OUT|=BIT1;                   //Set the resistor as a pull-up resistor
     P1DIR&=~BIT2;                  //Set SW2 (P1.2) to input
     P1REN|=BIT2;                   //Enable resistor in P1.2
     P1OUT|=BIT2;                   //Set the resistor as a pull-up resistor

     TA0CTL|=TASSEL_1|MC_1|TACLR;   //Set up Timer_A
     TA0CCR0=0x8000;                //Set the max value for Timer_A
     while(1)                       //Infinite Loop
     {
         if(TA0CTL&TAIFG)           //If Timer_A has its interrupt flag set
         {
            P1OUT^=BIT0;            //Toggle LED1
            TA0CTL&=~BIT0;          //Reset Timer_A
         }

         if((~P1IN & BIT1)|(~P1IN&BIT2))       //If the button is pressed
         {
             P9OUT|=BIT7;       //Turn on LED2
         }
         else
         {
             P9OUT&=~BIT7;      //Turn off LED2
         }
     }
}
