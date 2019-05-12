#include <msp430.h> //Header File
#include <driverlib.h> //Required for the LCD
#include "myGpio.h" //Required for the LCD
#include "myClocks.h" //Required for the LCD
#include "myLcd.h" //Required for the LCD

void main(void)
{
    int counter; //Initialize variable for counting
    PM5CTL0 = 0xFFFE; //To enable inputs and outputs
    WDTCTL = WDTPW | WDTHOLD; //Watchdog timer stop
    initGPIO(); //Initializes inputs and outputs for LCD
    initClocks(); //Initialize clocks for LCD
    myLCD_init(); //Prepares LCD to receive commands
    TA0CTL |= TASSEL_1 | MC_1 | TACLR; //Set up Timer_A
    TA0CCR0 = 0x8000; //Set the max value for Timer_A

    while (1) //Infinite Loop
    {
        myLCD_showChar('M', 1); //Display "M" in space 1
        myLCD_showChar('A', 2); //Display "A" in space 2
        myLCD_showChar('S', 3); //Display "S" in space 3
        myLCD_showChar('O', 4); //Display "O" in space 4
        myLCD_showChar('N', 5); //Display "N" in space 5

        while (!(TA0CTL & TAIFG)); //If Timer_A has its interrupt flag set
        TA0CTL &= ~BIT0; //Reset Timer_A

        for (counter = 1; counter < 7; counter++) //Create a loop for counting to 6
        {
            myLCD_displayNumber(counter); //Display current number
            while (!(TA0CTL & TAIFG)); //If Timer_A has its interrupt flag set
            TA0CTL &= ~BIT0; //Reset Timer_A
        }

        while (!(TA0CTL & TAIFG)); //If Timer_A has its interrupt flag set
        TA0CTL &= ~BIT0; //Reset Timer_A

        counter = 1; //Reset counter to 1
        myLCD_showChar(' ', 6); //Display " " in space 6, clearing the space
        myLCD_showSymbol(LCD_UPDATE,LCD_BRACKETS,0); //Display Brackets on LCD, Battery setup
        myLCD_showSymbol(LCD_UPDATE, LCD_BATT, 0); //Display BATT on LCD, Battery Setup

        for (counter = 1; counter < 8; counter++) //Create a loop for counting to 7
        {
            while (!(TA0CTL & TAIFG)); //If Timer_A has its interrupt flag set
            switch (counter) //Switch based on counter
            {
                case 1: //If counter is 1
                    myLCD_showSymbol(LCD_UPDATE, LCD_B1, 0); //Enable B1
                    break;
                case 2: //If counter is 2
                    myLCD_showSymbol(LCD_UPDATE, LCD_B2, 0); //Enable B2
                    break;
                case 3: //If counter is 3
                    myLCD_showSymbol(LCD_UPDATE, LCD_B3, 0); //Enable B3
                    break;
                case 4: //If counter is 4
                    myLCD_showSymbol(LCD_UPDATE, LCD_B4, 0); //Enable B4
                    break;
                case 5: //If counter is 5
                    myLCD_showSymbol(LCD_UPDATE, LCD_B5, 0); //Enable B5
                    break;
                case 6: //If counter is 6
                    myLCD_showSymbol(LCD_UPDATE, LCD_B6, 0); //Enable B6
                    break;
                default: //If counter is 7 or Default case for error prevention
                    myLCD_showSymbol(LCD_CLEAR, LCD_BRACKETS, 0); //Disable Brackets
                    myLCD_showSymbol(LCD_CLEAR, LCD_BATT, 0); //Disable Batt
                    myLCD_showSymbol(LCD_CLEAR, LCD_B1, 0); //Disable B1
                    myLCD_showSymbol(LCD_CLEAR, LCD_B2, 0); //Disable B2
                    myLCD_showSymbol(LCD_CLEAR, LCD_B3, 0); //Disable B3
                    myLCD_showSymbol(LCD_CLEAR, LCD_B4, 0); //Disable B4
                    myLCD_showSymbol(LCD_CLEAR, LCD_B5, 0); //Disable B5
                    myLCD_showSymbol(LCD_CLEAR, LCD_B6, 0); //Disable B6
            }
            TA0CTL &= ~BIT0; //Reset Timer_A
        }
        while (!(TA0CTL & TAIFG)); //If Timer_A has its interrupt flag set
        TA0CTL &= ~BIT0; //Reset Timer_A
    }
}
