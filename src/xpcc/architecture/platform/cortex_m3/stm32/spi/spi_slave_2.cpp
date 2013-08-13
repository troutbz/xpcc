// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file. 
 */
// ----------------------------------------------------------------------------

#include "../gpio.hpp"
#include "spi_slave_2.hpp"

namespace
{
	GPIO__INPUT(NssB12, B, 12);
	GPIO__INPUT(SckB13, B, 13);
	GPIO__INPUT(MisoB14, B, 14);
	GPIO__OUTPUT(MosiB15, B, 15);
	
//	static uint8_t* transmitBuffer(0);
//	static uint8_t* receiveBuffer(0);
//	static uint16_t bufferLength(0);
//	enum
//	{
//		BUFFER_TRANSMIT_INCR_bm = 0x01,
//		BUFFER_RECEIVE_INCR_bm = 0x02,
//		BUFFER_TRANSMIT_IS_NOT_ZERO_bm = 0x04,
//		BUFFER_RECEIVE_IS_NOT_ZERO_bm = 0x08,
//		BUFFER_IS_DUMMY_bm = 0x10,
//		BUFFER_IS_BUSY_SYNC_bm = 0x20
//	};
//	static uint8_t status(0);
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::SpiSlave2::configurePins(Mapping mapping)
{
	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	
	// Initialize IO pins
#if defined(STM32F2XX) || defined(STM32F4XX)
	(void) mapping;		// avoid compiler warning
	
	NssB12::setAlternateFunction(AF_SPI2);
	SckB13::setAlternateFunction(AF_SPI2);
	MisoB14::setAlternateFunction(AF_SPI2);
	MosiB15::setAlternateFunction(AF_SPI2, xpcc::stm32::PUSH_PULL);
#else
	(void) mapping;		// avoid compiler warning
	
	SckB13::setAlternateFunction(xpcc::stm32::FLOATING);
	MisoB14::setInput(xpcc::stm32::FLOATING);
	MosiB15::setAlternateFunction(xpcc::stm32::PUSH_PULL);
#endif
}


// ----------------------------------------------------------------------------
void
xpcc::stm32::SpiSlave2::initialize(Mode mode, DataSize datasize)
{
	RCC->APB1ENR |= RCC_APB1ENR_SPI2EN;
	
	RCC->APB1RSTR |=  RCC_APB1RSTR_SPI2RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI2RST;
	
	// disable all interrupts
	SPI2->CR2 &= ~(SPI_CR2_TXEIE  | SPI_CR2_RXNEIE  | SPI_CR2_ERRIE);
	
	// disable peripheral
	SPI2->CR1 &= ~SPI_CR1_SPE;
	
	// slave mode: CR1_MSTR = '0'
	// NSS not fixed: CR1_SSM = '0'
	// Motorola Mode: CR2_FRF = '0'
	
	// set data size
	SPI2->CR1 |= datasize;
	
	// set slave mode with NSS not fixed
	SPI2->CR1 = mode;
		
	// reenable peripheral
	SPI2->CR1 |= SPI_CR1_SPE;
}
	
// ----------------------------------------------------------------------------

//bool
//xpcc::stm32::SpiSlave2::isFinished()
//{
//	return !(status & BUFFER_IS_BUSY_SYNC_bm);
//}

// ----------------------------------------------------------------------------

uint16_t
xpcc::stm32::SpiSlave2::read() 
{
	return SPI2->DR;
}

// ----------------------------------------------------------------------------
void
xpcc::stm32::SpiSlave2::enableInterruptVector(bool enable,
uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(SPI1_IRQn, priority);
		
		// register IRQ at the NVIC
		NVIC_EnableIRQ(SPI1_IRQn);
	}
	else {
		NVIC_DisableIRQ(SPI1_IRQn);
	}
}


