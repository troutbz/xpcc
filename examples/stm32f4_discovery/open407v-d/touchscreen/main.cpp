
#include <xpcc/architecture.hpp>

#include "parallel_tft.hpp"
#include <xpcc/driver/ui/display/image.hpp>
#include <xpcc/driver/ui/touchscreen/ads7843.hpp>
#include <xpcc/architecture/driver/gpio.hpp>

#include "touchscreen_calibrator.hpp"

using namespace xpcc::stm32;


typedef xpcc::stm32::Usart1 uart;


// ----------------------------------------------------------------------------
//GPIO__OUTPUT(LedOrange, D, 13);		// User LED 3
//GPIO__OUTPUT(LedGreen, D, 12);		// User LED 4
//GPIO__OUTPUT(LedRed, D, 14);		// User LED 5 (used as FSMC D0)
//GPIO__OUTPUT(LedBlue, D, 15);		// User LED 6 (used as FSMC D1)

GPIO__OUTPUT(VBusPresent, A, 9);		// green LED (LD7)
//GPIO__OUTPUT(VBusOvercurrent, D, 5);	// red LED   (LD8) (used as FSMC NWE)

GPIO__INPUT(Button, A, 0);


GPIO__OUTPUT(Trigger, D, 6);

// 16bit databus
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
GPIO__OUTPUT(Reset, C, 15);

GPIO__OUTPUT(Rs, D, 14);
GPIO__OUTPUT(Wr, D, 13);
GPIO__OUTPUT(Rd, D, 15);


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

typedef xpcc::TftGpio<Port, Reset, Cs, Wr, Rd, Rs> GpioBus;

GpioBus gpioBus;


xpcc::ParallelTft<GpioBus> tft(gpioBus);



// ----------------------------------------------------------------------------
// Touchscreen
//GPIO__OUTPUT(CsTouchscreen, C, 4);
//GPIO__INPUT(Int, C, 5);

//xpcc::Ads7843<SpiMaster2, CsTouchscreen, Int> ads7843;
//xpcc::TouchscreenCalibrator touchscreen;

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
initUart() {



	uart::configurePins(uart::REMAP_PB6_PB7);
	uart::setBaudrate(9600);

	uart::write('H');
	uart::write('e');
	uart::write('l');
	uart::write('l');
	uart::write('o');
	uart::write('\r');
	uart::write('\n');


}

static void
initDisplay()
{
	Port::setOutput();
	
	Rd::setOutput();
	Wr::setOutput();
	Rs::setOutput();
	
	Reset::setOutput();
	Reset::set();
	
	Cs::setOutput();
	Cs::set();
	
	
	tft.initialize();
}

static void
initTouchscreen()
{
/*	CsTouchscreen::setOutput();
	CsTouchscreen::set();
	
	Int::setInput(PULLUP);

	SpiMaster2::initialize(SpiMaster2::MODE_0, SpiMaster2::PRESCALER_64);
	SpiMaster2::configurePins(SpiMaster2::REMAP_PB13_PB14_PB15);
	*/
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

static void
calibrateTouchscreen(xpcc::GraphicDisplay& display)
{
	xpcc::glcd::Point calibrationPoint[3] = { { 45, 45 }, { 270, 90 }, { 100, 190 } };
	xpcc::glcd::Point sample[3];
	
	for (uint8_t i = 0; i < 3; i++)
	{
		display.clear();
		
		display.setColor(xpcc::glcd::Color::yellow());
		display.setCursor(50, 5);
		display << "Touch crosshair to calibrate";
		
		drawCross(display, calibrationPoint[i]);
		xpcc::delay_ms(500);
		/*
		while (!ads7843.read(&sample[i])) {
			// wait until a valid sample can be taken
		}
		*/
	}
	/*
	touchscreen.calibrate(calibrationPoint, sample);
	*/
	display.clear();
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

	initUart();

//	Port::setOutput();
//

//	while(1) {
//
//		gpioBus.writeData(0xFFFF);
//
//		xpcc::delay_ms(1);
//
//		gpioBus.writeData(0);
//
//		xpcc::delay_ms(1);
//
//	}
	
	uint16_t data1 = 0xCDCD;

	initDisplay();

//	gpioBus.writeRegister(0x0000, 0x0001);
//	xpcc::delay_ms(20);

//
//	while(1) {
//
//
//		//tft.writeIndex(0x0000);
//
//		//tft.writeData(0x9321);
//
//
//
//		//data1 = gpioBus.readRegister(0x0000);
//
//		data1 = gpioBus.statusRead();
//
//		uart::write(data1 >> 8);
//		uart::write(data1 & 0x00FF);
//
//		xpcc::delay_ms(200);
//
//	}


	tft.setColor(xpcc::glcd::Color::lime());
	tft.setBackgroundColor(xpcc::glcd::Color::black());
//	tft.clear();


	xpcc::glcd::Point center(120, 120);




	uint16_t data = 0;
	uint16_t x, y;
	bool dir = true;

	if(tft.setWindow(0, 100, 0, 100)) {
		uart::write('y');
	} else {
		uart::write('n');
	}

	tft.resetWindow();

	while (1)
	{

		xpcc::delay_ms(20);

		//tft.clear();

//		if(data) {
//			tft.setBackgroundColor(xpcc::glcd::Color::red());
//			data = 0;
//		} else {
//			tft.setBackgroundColor(xpcc::glcd::Color::blue());
//			data = 1;
//		}

		x = center.getX();
		y = center.getY();

		//tft.setColor(xpcc::glcd::Color::lime());
		tft.clearArea(x-15, x+15, y-15, y+15);




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

		tft.resetWindow();
		drawCross(tft, center);

//		tft.update();

	}

	return 0;
}

