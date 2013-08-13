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
#include "spi_slave_3.hpp"

#ifndef SPI_CR2_FRF
#define SPI_CR2_FRF               ((uint16_t)0x0010)
#endif

namespace
{
	GPIO__INPUT(NssA15, A, 15);
	GPIO__INPUT(SckB3, B, 3);
	GPIO__INPUT(MisoB4, B, 4);
	GPIO__OUTPUT(MosiB5, B, 5);
	
	GPIO__INPUT(NssA4, A, 4);
	GPIO__INPUT(SckC10, C, 10);
	GPIO__INPUT(MisoC11, C, 11);
	GPIO__OUTPUT(MosiC12, C, 12);
	
}

// ----------------------------------------------------------------------------

void
xpcc::stm32::SpiSlave3::configurePins(Mapping mapping)
{
	// Enable clock
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	
	// Initialize IO pins
#if defined(STM32F4XX)
	if (mapping == REMAP_PA15_PB3_PB4_PB5) {
		NssA4::setAlternateFunction(AF_SPI1);
		SckB3::setAlternateFunction(AF_SPI3);
		MisoB4::setAlternateFunction(AF_SPI3);
		MosiB5::setAlternateFunction(AF_SPI3, xpcc::stm32::PUSH_PULL);
	}
	else {
		NssA4::setAlternateFunction(AF_SPI3);
		SckC10::setAlternateFunction(AF_SPI3);
		MisoC11::setAlternateFunction(AF_SPI3);
		MosiC12::setAlternateFunction(AF_SPI3, xpcc::stm32::PUSH_PULL);
	}
#else
#error "Only STM32F4 is supported and tested. F2 might work, too."
#endif
}

// ----------------------------------------------------------------------------

void
xpcc::stm32::SpiSlave3::initialize(Mode mode, DataSize datasize)
{
	RCC->APB1ENR |= RCC_APB1ENR_SPI3EN;
	
	RCC->APB1RSTR |=  RCC_APB1RSTR_SPI3RST;
	RCC->APB1RSTR &= ~RCC_APB1RSTR_SPI3RST;
	
	// disable peripheral
	SPI3->CR1 &= ~SPI_CR1_SPE;
	
	// reset CR1
	// SPI_CR1_MSTR = 0: slave mode
	// SPI_CR1_SSM = 0: NSS software management disabled
	SPI3->CR1 = 0x00;
	
	// reset CR2
	// SPI_CR2_FRF = 0: motorola mode
	SPI3->CR2 &= ~(SPI_CR2_TXEIE | SPI_CR2_RXNEIE | SPI_CR2_ERRIE | SPI_CR2_FRF | SPI_CR2_SSOE | SPI_CR2_TXDMAEN | SPI_CR2_RXDMAEN);
	
	
	// enable rx interrupt
	SPI3->CR2 |= SPI_CR2_RXNEIE;
	
	// set data size
	// set clock polarity and phase
	SPI3->CR1 |= datasize | mode;
		
	// reenable peripheral
	SPI3->CR1 |= SPI_CR1_SPE;
	
	// clear data register
	SPI3->DR = 0;
}
	
// ----------------------------------------------------------------------------

uint16_t
xpcc::stm32::SpiSlave3::read() 
{
	return SPI3->DR;
}

// ----------------------------------------------------------------------------

bool
xpcc::stm32::SpiSlave3::rxBufferNotEmpty() 
{	
	return SPI3->SR & SPI_SR_RXNE;
}

// ----------------------------------------------------------------------------

void
xpcc::stm32::SpiSlave3::enableInterruptVector(bool enable, uint32_t priority)
{
	if (enable) {
		// Set priority for the interrupt vector
		NVIC_SetPriority(SPI3_IRQn, priority);
		
		// register IRQ at the NVIC
		NVIC_EnableIRQ(SPI3_IRQn);
	}
	else {
		NVIC_DisableIRQ(SPI3_IRQn);
	}
}