#include <msp430.h> 

unsigned int counter;           //Create a variable to act as a counter

void main(void)
{
     WDTCTL = WDTPW | WDTHOLD;  //Stop watch dog timer
     PM5CTL0 = 0xFFFE;          //To enable inputs and outputs
     P1DIR|=BIT0;               //Set LED (P1.0) to output
     P1OUT&=~BIT0;              //Turn off LED
     while(1)                   //Infinite Loop
        {
         counter=0;             //Set counter to 0
         while(counter<50000)   //While counter < large delay
         {
              counter+=1;       //Increment counter
         }
          P1OUT^=BIT0;          //Toggle LED
     }
}
