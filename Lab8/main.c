#include <msp430.h> //Header File
#include <driverlib.h> //Required for the LCD
#include "myGpio.h" //Required for the LCD
#include "myClocks.h" //Required for the LCD
#include "myLcd.h" //Required for the LCD

#define ADC12_SHT_16 0x0200
#define ADC12_ON 0x0010
#define ADC12_SHT_SRC_SEL 0x0200
#define ADC12_12BIT 0x0020
#define ADC12_P92 0x000A

int main(void)
{
    const int DARK = 0x0083, NORMAL = 0x0413, BRIGHT = 0x0ACD; // Constants from measuring
    const int darkLimit = NORMAL - (NORMAL-DARK)/2; // Calculate the dark limit
    const int brightLimit = BRIGHT - (BRIGHT-NORMAL)/2; // Calculate the bright limit

    PM5CTL0 = 0xFFFE; //To enable inputs and outputs
    WDTCTL = WDTPW | WDTHOLD; //Watchdog timer stop
    initGPIO(); //Initializes inputs and outputs for LCD
    initClocks(); //Initialize clocks for LCD
    myLCD_init(); //Prepares LCD to receive commands

    ADC12CTL0 = ADC12_SHT_16 | ADC12_ON; // Set sample and hold time and turn on
    ADC12CTL1 = ADC12_SHT_SRC_SEL; //Specifies sample and hold clock source
    ADC12CTL2 = ADC12_12BIT; // 12 bit conversion result
    ADC12MCTL0 = ADC12_P92; // P9.2 analog input
// mode is single source, single conversion

    while (1) // infinite loop
    {
        ADC12CTL0 = ADC12CTL0 | ADC12ENC; // Enable conversion
        ADC12CTL0 = ADC12CTL0 | ADC12SC; // Start conversion
// now ADC12MEM0 holds 12 bit sample

        if (ADC12MEM0 >= darkLimit && ADC12MEM0 <= brightLimit) // If the light level is in between low and high
        {
            myLCD_displayNumber(1); // Display '1'
        }
        else if (ADC12MEM0 < darkLimit && ADC12MEM0 >= 1) // Else if the light level is low
        {
            myLCD_displayNumber(0); // Display '0'
        }
        else if (ADC12MEM0 > brightLimit) // Else if the light level is high
        {
            myLCD_displayNumber(2); // Display '2'
        }
    }

}
