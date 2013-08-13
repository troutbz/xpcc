#include <xpcc/architecture.hpp>

// ----------------------------------------------------------------------------
GPIO__OUTPUT(LedGreen, D, 12);		// User LED 4
GPIO__OUTPUT(LedBlue, D, 15);		// User LED 6

GPIO__OUTPUT(VBusPresent, A, 9);		// green LED (LD7)
GPIO__OUTPUT(VBusOvercurrent, D, 5);	// red LED   (LD8)

GPIO__INPUT(Button, A, 0);

using namespace xpcc::stm32;

uint16_t data;
typedef xpcc::stm32::Usart1 uart;


// ----------------------------------------------------------------------------

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
initUart()
{
	uart::setBaudrate(115200);
	uart::configurePins(xpcc::stm32::Usart1::REMAP_PB6_PB7);
}

static void
initSpi()
{
	SpiSlave1::configurePins(SpiSlave1::REMAP_PA4_PA5_PA6_PA7);
	SpiSlave1::initialize(SpiSlave1::MODE_0, SpiSlave1::DATA_16_BIT);
	SpiSlave1::enableInterruptVector(true, 1);

	SpiMaster3::configurePins(SpiMaster3::REMAP_PA15_PB3_PB4_PB5);
	SpiMaster3::initialize(SpiMaster3::MODE_0, SpiMaster3::PRESCALER_8);
}

// ----------------------------------------------------------------------------

extern "C" void
SPI1_IRQHandler() {
	LedBlue::toggle();
	//uart::write('Y');

	data = SPI1->DR;
	uart::write((uint8_t)(data >> 8));
	uart::write((uint8_t)data);
}

// ----------------------------------------------------------------------------

MAIN_FUNCTION
{
	initClock();
	initSpi();
	initUart();

	//SysTickTimer::enable();


	LedGreen::setOutput(xpcc::gpio::LOW);
	LedBlue::setOutput(xpcc::gpio::LOW);
	
	while (1)
	{

		SpiMaster3::write(0xAF);
		SpiMaster3::write(0xCD);

		LedGreen::toggle();

		xpcc::delay_ms(50);

	}

	return 0;
}
