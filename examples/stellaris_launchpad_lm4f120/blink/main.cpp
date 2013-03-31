/**
 * Simple example of GPIO usage.
 *
 * This example lets blink the blue LED like a wing tip strobe.
 */

#include <xpcc/architecture.hpp>

// TODO Replace by architecture
#include "inc/lm4f120h5qr.h"

//using namespace xpcc::lm4f;

// ----------------------------------------------------------------------------
MAIN_FUNCTION
{
    // Enable the GPIO port that is used for the on-board LED.
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;

    // Do a dummy read to insert a few cycles after enabling the peripheral.
    volatile unsigned long ulLoop = SYSCTL_RCGC2_R;

    // Enable the GPIO pin for the LED (PF2).  Set the direction as output, and
    // enable the GPIO pin for digital function.
    GPIO_PORTF_DIR_R = 0x04;
    GPIO_PORTF_DEN_R = 0x04;

    // Loop forever.
    while(1)
    {
        // Turn on the LED.
        GPIO_PORTF_DATA_R |= 0x04;

        xpcc::delay_ms(35);

		// Turn off the LED.
		GPIO_PORTF_DATA_R &= ~(0x04);

        xpcc::delay_ms(200);
        GPIO_PORTF_DATA_R |= 0x04;
        xpcc::delay_ms(35);
		GPIO_PORTF_DATA_R &= ~(0x04);

		// Delay for a bit.
		xpcc::delay_ms(975);
    }

    return 0;
}
