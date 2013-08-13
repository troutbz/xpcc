#include <xpcc/architecture.hpp>

#include "parallel_tft.hpp"
#include <xpcc/driver/ui/display/image.hpp>
#include <xpcc/architecture/driver/gpio.hpp>

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
// parallel data bus
GPIO__IO(D0,  E, 7);
GPIO__IO(D1,  E, 8);
GPIO__IO(D2,  E, 9);
GPIO__IO(D3,  E, 10);
GPIO__IO(D4,  E, 11);
GPIO__IO(D5,  E, 12);
GPIO__IO(D6,  E, 13);
GPIO__IO(D7,  E, 14);
GPIO__IO(D8,  E, 15);
GPIO__IO(D9,  B, 10);
GPIO__IO(D10, B, 11);
GPIO__IO(D11, B, 15);
GPIO__IO(D12, D, 10);
GPIO__IO(D13, D, 9);
GPIO__IO(D14, D, 12);
GPIO__IO(D15, D, 11);

GPIO__OUTPUT(Cs, 	C, 14);
GPIO__OUTPUT(Rs, D, 14);
GPIO__OUTPUT(Wr, D, 13);
GPIO__OUTPUT(Rd, D, 15);

GPIO__OUTPUT(nReset, C, 15);

GPIO__INPUT(Dc, D, 2);

typedef xpcc::gpio::DoublePort<D15,
							 D14,
							 D13,
							 D12,
							 D11,
							 D10,
							 D9,
							 D8,
							 D7,
							 D6,
							 D5,
							 D4,
							 D3,
							 D2,
							 D1,
							 D0> Port;


typedef xpcc::TftGpio<Port, nReset, Cs, Wr, Rd, Rs> GpioBus;
GpioBus gpioBus;

xpcc::ParallelTft<GpioBus> tft(gpioBus);

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
	SpiMaster3::initialize(SpiMaster3::MODE_0, SpiMaster3::PRESCALER_4);

	// SPI needs plenty of time to get up :/
	xpcc::delay_ms(500);
}

static void
initDisplay()
{
	Port::setOutput();

	Rd::setOutput();
	Wr::setOutput();
	Rs::setOutput();

	nReset::setOutput();
	nReset::set();

	Cs::setOutput();
	Cs::set();

	Dc::setInput();


	//tft.initialize();
	//tft.clear();
}

// ----------------------------------------------------------------------------

extern "C" void
SPI1_IRQHandler() {
	LedBlue::toggle();
	//uart::write('Y');

	data = SPI1->DR;

//	uart::write('S');
//	uart::write('P');
//	uart::write('I');
//	uart::write(':');
//	uart::write((uint8_t)(data >> 8));
//	uart::write((uint8_t)data);
//	uart::write('\n');
//	if(Dc::read()) {
//		Rs::set();
//	} else {
//		Rs::reset();
//	}

	gpioBus.writeRaw(data);
}
// ----------------------------------------------------------------------------

MAIN_FUNCTION
{
	initClock();
	initSpi();


//	initUart();
	initDisplay();

//	uart::write('H');

//	xpcc::glcd::Point center(120, 120);
	//tft.clear();
	//tft.setBackgroundColor(xpcc::glcd::Color::lime());
	//tft.clear();
	//SysTickTimer::enable();

	//drawCross(tft, center);

	LedGreen::setOutput(xpcc::gpio::LOW);
	LedBlue::setOutput(xpcc::gpio::LOW);
	
	bool col = false;

	while (1)
	{

		/*if(Dc::read()) {
			Rs::set();
		} else {
			Rs::reset();
		}*/

//		if(col) {
//			tft.setBackgroundColor(xpcc::glcd::Color::lime());
//			col = false;
//		} else {
//			tft.setBackgroundColor(xpcc::glcd::Color::red());
//			col = true;
//		}
//
//		tft.clear();

//		SpiMaster3::write(0xAF);
//		SpiMaster3::write(0xCD);

//		LedGreen::toggle();
//
//		xpcc::delay_ms(1000);

		Rs::set(Dc::read());
	}

	return 0;
}
