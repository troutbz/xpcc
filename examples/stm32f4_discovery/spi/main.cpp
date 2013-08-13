#include <xpcc/architecture.hpp>

// ----------------------------------------------------------------------------
GPIO__OUTPUT(LedOrange, D, 13);		// User LED 3
GPIO__OUTPUT(LedGreen, D, 12);		// User LED 4
GPIO__OUTPUT(LedRed, D, 14);		// User LED 5
GPIO__OUTPUT(LedBlue, D, 15);		// User LED 6

GPIO__OUTPUT(VBusPresent, A, 9);		// green LED (LD7)
GPIO__OUTPUT(VBusOvercurrent, D, 5);	// red LED   (LD8)

GPIO__INPUT(Button, A, 0);

using namespace xpcc::stm32;

static bool
initClock()
{
	// use external 8MHz crystal
	if (!Clock::enableHse(Clock::HseConfig::HSE_CRYSTAL)) {
		return false;
	}
	
	Clock::enablePll(Clock::PllSource::PLL_HSE, 4, 168);
	return Clock::switchToPll();
}

static void
initSpi()
{

	SpiSlave1::configurePins(SpiSlave1::REMAP_PA4_PA5_PA6_PA7);
	SpiSlave1::initialize(SpiSlave1::MODE_0, SpiSlave1::DATA_8_BIT);
	SpiSlave1::enableInterruptVector(true, 10);

	SpiMaster3::configurePins(SpiMaster3::REMAP_PA15_PB3_PB4_PB5);
	SpiMaster3::initialize(SpiMaster3::MODE_0);



}

void SPI1_IRQHandler() {

}

// ----------------------------------------------------------------------------
MAIN_FUNCTION
{
	initClock();

	initSpi();


	LedOrange::setOutput(xpcc::gpio::HIGH);
	LedGreen::setOutput(xpcc::gpio::LOW);
	LedRed::setOutput(xpcc::gpio::HIGH);
	LedBlue::setOutput(xpcc::gpio::HIGH);
	
	while (1)
	{
		LedBlue::toggle();
		LedGreen::toggle();
		xpcc::delay_ms(500);
	}

	return 0;
}
