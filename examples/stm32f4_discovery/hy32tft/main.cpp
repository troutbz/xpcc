
#include <xpcc/architecture.hpp>

#include "parallel_tft.hpp"
#include <xpcc/driver/ui/display/image.hpp>
#include <xpcc/architecture/driver/gpio.hpp>


using namespace xpcc::stm32;


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
	
	
	tft.initialize();
}
// ----------------------------------------------------------------------------

static void
drawCross(xpcc::GraphicDisplay& display, xpcc::glcd::Point center)
{
	display.setColor(xpcc::glcd::Color::red());
	display.drawLine(center.x - 15, center.y, center.x - 2, center.y);
	display.drawLine(center.x + 2, center.y, center.x + 15, center.y);
	display.drawLine(center.x, center.y - 15, center.x, center.y - 2);
	display.drawLine(center.x, center.y + 2, center.x, center.y + 15);

	display.setColor(xpcc::glcd::Color::white());
	display.drawLine(center.x - 15, center.y + 15, center.x - 7, center.y + 15);
	display.drawLine(center.x - 15, center.y + 7, center.x - 15, center.y + 15);

	display.drawLine(center.x - 15, center.y - 15, center.x - 7, center.y - 15);
	display.drawLine(center.x - 15, center.y - 7, center.x - 15, center.y - 15);

	display.drawLine(center.x + 7, center.y + 15, center.x + 15, center.y + 15);
	display.drawLine(center.x + 15, center.y + 7, center.x + 15, center.y + 15);

	display.drawLine(center.x + 7, center.y - 15, center.x + 15, center.y - 15);
	display.drawLine(center.x + 15, center.y - 15, center.x + 15, center.y - 7);
}

void
drawPoint(xpcc::GraphicDisplay& display, xpcc::glcd::Point point)
{
	if (point.x < 0 || point.y < 0) {
		return;
	}
	
	display.drawPixel(point.x, point.y);
	display.drawPixel(point.x + 1, point.y);
	display.drawPixel(point.x, point.y + 1);
	display.drawPixel(point.x + 1, point.y + 1);
}
// ----------------------------------------------------------------------------

MAIN_FUNCTION
{
	initClock();
	initDisplay();


	xpcc::glcd::Point center(120, 120);

	uint16_t x;
	bool dir = true;

	tft.setBackgroundColor(xpcc::glcd::Color::black());

	while (1)
	{

		xpcc::delay_ms(1);

		tft.clear();


		drawCross(tft, center);

		x = center.getX();

		if(x == MAX_X)
			dir = false;

		if(x == 0)
			dir = true;

		if(dir) {
			x++;
		} else {
			x--;
		}
		center.setX(x);

//		tft.update();

	}

	return 0;
}

