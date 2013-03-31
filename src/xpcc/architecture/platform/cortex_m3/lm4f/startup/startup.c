#include <stdint.h>
#include <xpcc/architecture/utils.hpp>

#include "xpcc_config.hpp"

#include <inc/hw_nvic.h>
#include <inc/hw_types.h>

// Size of interrupt table
//#define NR_INTERRUPTS

// ----------------------------------------------------------------------------
/*
 * Provide weak aliases for each Exception handler to defaultHandler.
 * As they are weak aliases, any function with the same name will override
 * this definition.
 */
void Reset_Handler(void);
void NMI_Handler(void)				__attribute__ ((weak, alias("defaultHandler")));
void HardFault_Handler(void);
void MemManage_Handler(void)		__attribute__ ((weak, alias("defaultHandler")));
void BusFault_Handler(void)			__attribute__ ((weak, alias("defaultHandler")));
void UsageFault_Handler(void)		__attribute__ ((weak, alias("defaultHandler")));
void SVC_Handler(void)				__attribute__ ((weak, alias("defaultHandler")));
void DebugMon_Handler(void)			__attribute__ ((weak, alias("defaultHandler")));
void PendSV_Handler(void)			__attribute__ ((weak, alias("defaultHandler")));
void SysTick_Handler(void)			__attribute__ ((weak, alias("defaultHandler")));


// ----------------------------------------------------------------------------
// Interrupt vectors
typedef void (* const FunctionPointer)(void);

// defined in the linkerscript
extern uint32_t __stack_end;

// Forward declaration
// TODO name all interrupt handlers
static void defaultHandler(void);

#if CORTEX_VECTORS_RAM
// Define the vector table
FunctionPointer flashVectors[4]
__attribute__ ((section(".reset"))) =
{
	(FunctionPointer) &__stack_end,	// stack pointer
	Reset_Handler,				// code entry point
	NMI_Handler,				// NMI handler
	HardFault_Handler,			// hard fault handler
};

FunctionPointer ramVectors[] __attribute__ ((section(".vectors"))) =
#else
FunctionPointer flashVectors[] __attribute__ ((section(".reset"))) =
#endif
{
	(FunctionPointer) &__stack_end,         // The initial stack pointer
    Reset_Handler,                          // The reset handler
    NMI_Handler,                            // The NMI handler
    HardFault_Handler,                      // The hard fault handler
    MemManage_Handler,                      // The MPU fault handler
    BusFault_Handler,                       // The bus fault handler
    UsageFault_Handler,                     // The usage fault handler
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    SVC_Handler,                            // SVCall handler
    DebugMon_Handler,                       // Debug monitor handler
    0,                                      // Reserved
    PendSV_Handler,                         // The PendSV handler
    SysTick_Handler,                        // The SysTick handler

    // Peripheral interrupts (LM4F specific)
    defaultHandler,                      // GPIO Port A
    defaultHandler,                      // GPIO Port B
    defaultHandler,                      // GPIO Port C
    defaultHandler,                      // GPIO Port D
    defaultHandler,                      // GPIO Port E
    defaultHandler,                      // UART0 Rx and Tx
    defaultHandler,                      // UART1 Rx and Tx
    defaultHandler,                      // SSI0 Rx and Tx
    defaultHandler,                      // I2C0 Master and Slave
    defaultHandler,                      // PWM Fault
    defaultHandler,                      // PWM Generator 0
    defaultHandler,                      // PWM Generator 1
    defaultHandler,                      // PWM Generator 2
    defaultHandler,                      // Quadrature Encoder 0
    defaultHandler,                      // ADC Sequence 0
    defaultHandler,                      // ADC Sequence 1
    defaultHandler,                      // ADC Sequence 2
    defaultHandler,                      // ADC Sequence 3
    defaultHandler,                      // Watchdog timer
    defaultHandler,                      // Timer 0 subtimer A
    defaultHandler,                      // Timer 0 subtimer B
    defaultHandler,                      // Timer 1 subtimer A
    defaultHandler,                      // Timer 1 subtimer B
    defaultHandler,                      // Timer 2 subtimer A
    defaultHandler,                      // Timer 2 subtimer B
    defaultHandler,                      // Analog Comparator 0
    defaultHandler,                      // Analog Comparator 1
    defaultHandler,                      // Analog Comparator 2
    defaultHandler,                      // System Control (PLL, OSC, BO)
    defaultHandler,                      // FLASH Control
    defaultHandler,                      // GPIO Port F
    defaultHandler,                      // GPIO Port G
    defaultHandler,                      // GPIO Port H
    defaultHandler,                      // UART2 Rx and Tx
    defaultHandler,                      // SSI1 Rx and Tx
    defaultHandler,                      // Timer 3 subtimer A
    defaultHandler,                      // Timer 3 subtimer B
    defaultHandler,                      // I2C1 Master and Slave
    defaultHandler,                      // Quadrature Encoder 1
    defaultHandler,                      // CAN0
    defaultHandler,                      // CAN1
    defaultHandler,                      // CAN2
    defaultHandler,                      // Ethernet
    defaultHandler,                      // Hibernate
    defaultHandler,                      // USB0
    defaultHandler,                      // PWM Generator 3
    defaultHandler,                      // uDMA Software Transfer
    defaultHandler,                      // uDMA Error
    defaultHandler,                      // ADC1 Sequence 0
    defaultHandler,                      // ADC1 Sequence 1
    defaultHandler,                      // ADC1 Sequence 2
    defaultHandler,                      // ADC1 Sequence 3
    defaultHandler,                      // I2S0
    defaultHandler,                      // External Bus Interface 0
    defaultHandler,                      // GPIO Port J
    defaultHandler,                      // GPIO Port K
    defaultHandler,                      // GPIO Port L
    defaultHandler,                      // SSI2 Rx and Tx
    defaultHandler,                      // SSI3 Rx and Tx
    defaultHandler,                      // UART3 Rx and Tx
    defaultHandler,                      // UART4 Rx and Tx
    defaultHandler,                      // UART5 Rx and Tx
    defaultHandler,                      // UART6 Rx and Tx
    defaultHandler,                      // UART7 Rx and Tx
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    defaultHandler,                      // I2C2 Master and Slave
    defaultHandler,                      // I2C3 Master and Slave
    defaultHandler,                      // Timer 4 subtimer A
    defaultHandler,                      // Timer 4 subtimer B
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    0,                                      // Reserved
    defaultHandler,                      // Timer 5 subtimer A
    defaultHandler,                      // Timer 5 subtimer B
    defaultHandler,                      // Wide Timer 0 subtimer A
    defaultHandler,                      // Wide Timer 0 subtimer B
    defaultHandler,                      // Wide Timer 1 subtimer A
    defaultHandler,                      // Wide Timer 1 subtimer B
    defaultHandler,                      // Wide Timer 2 subtimer A
    defaultHandler,                      // Wide Timer 2 subtimer B
    defaultHandler,                      // Wide Timer 3 subtimer A
    defaultHandler,                      // Wide Timer 3 subtimer B
    defaultHandler,                      // Wide Timer 4 subtimer A
    defaultHandler,                      // Wide Timer 4 subtimer B
    defaultHandler,                      // Wide Timer 5 subtimer A
    defaultHandler,                      // Wide Timer 5 subtimer B
    defaultHandler,                      // FPU
    defaultHandler,                      // PECI 0
    defaultHandler,                      // LPC 0
    defaultHandler,                      // I2C4 Master and Slave
    defaultHandler,                      // I2C5 Master and Slave
    defaultHandler,                      // GPIO Port M
    defaultHandler,                      // GPIO Port N
    defaultHandler,                      // Quadrature Encoder 2
    defaultHandler,                      // Fan 0
    0,                                      // Reserved
    defaultHandler,                      // GPIO Port P (Summary or P0)
    defaultHandler,                      // GPIO Port P1
    defaultHandler,                      // GPIO Port P2
    defaultHandler,                      // GPIO Port P3
    defaultHandler,                      // GPIO Port P4
    defaultHandler,                      // GPIO Port P5
    defaultHandler,                      // GPIO Port P6
    defaultHandler,                      // GPIO Port P7
    defaultHandler,                      // GPIO Port Q (Summary or Q0)
    defaultHandler,                      // GPIO Port Q1
    defaultHandler,                      // GPIO Port Q2
    defaultHandler,                      // GPIO Port Q3
    defaultHandler,                      // GPIO Port Q4
    defaultHandler,                      // GPIO Port Q5
    defaultHandler,                      // GPIO Port Q6
    defaultHandler,                      // GPIO Port Q7
    defaultHandler,                      // GPIO Port R
    defaultHandler,                      // GPIO Port S
    defaultHandler,                      // PWM 1 Generator 0
    defaultHandler,                      // PWM 1 Generator 1
    defaultHandler,                      // PWM 1 Generator 2
    defaultHandler,                      // PWM 1 Generator 3
    defaultHandler                       // PWM 1 Fault
};

// ----------------------------------------------------------------------------
// defined in the linkerscript
extern uint32_t __fastcode_load;
extern uint32_t __fastcode_start;
extern uint32_t __fastcode_end;

extern uint32_t __data_load;
extern uint32_t __data_start;
extern uint32_t __data_end;

extern uint32_t __bss_start;
extern uint32_t __bss_end;

// Application's main function
int
main(void);

// calls CTORS of static objects
void
__libc_init_array(void);

extern void
exit(int) __attribute__ ((noreturn, weak));

// ----------------------------------------------------------------------------
void
Reset_Handler(void)
{
	// Copy functions to RAM (.fastcode)
	uint32_t* src = &__fastcode_load;
	uint32_t* dest = &__fastcode_start;
	while (dest < &__fastcode_end)
	{
		*(dest++) = *(src++);
	}

	// Copy the data segment initializers from flash to RAM (.data)
	src = &__data_load;
	dest = &__data_start;
	while (dest < &__data_end)
	{
		*(dest++) = *(src++);
	}

	// Fill the bss segment with zero (.bss)
	dest = &__bss_start;
	while (dest < &__bss_end)
	{
		*(dest++) = 0;
	}

    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).  Any configuration of the floating-point unit using DriverLib
    // APIs must be done here prior to the floating-point unit being enabled.
    //
    // Note that this does not use DriverLib since it might not be included in
    // this project.
    //
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) &
                         ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) |
                        NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);

	// Enable GPIO clock
    // TODO


	// Setup NVIC
	// Set vector table
//	const uint32_t offset = 0;
//	SCB->VTOR = 0x08000000 | (offset & 0x1FFFFF80);

	// Lower priority level for all peripheral interrupts to lowest possible
//	for (uint32_t i = 0; i < NR_INTERRUPTS; i++) {
//		const uint32_t priority = 0xF;
//		NVIC->IP[i] = (priority & 0xF) << 4;
//	}

	// Set the PRIGROUP[10:8] bits to
	// - 4 bits for pre-emption priority,
	// - 0 bits for subpriority
//	SCB->AIRCR = 0x05FA0000 | 0x300;

	// Enable fault handlers
	/*SCB->SHCSR |=
			SCB_SHCSR_BUSFAULTENA_Msk |
			SCB_SHCSR_USGFAULTENA_Msk |
			SCB_SHCSR_MEMFAULTENA_Msk;*/

	// Call CTORS of static objects
	__libc_init_array();

	// Call the application's entry point
	main();

	exit(1);

	while (1)
	{
	}
}

// ----------------------------------------------------------------------------
/**
 * @brief	Default interrupt handler
 *
 * This functions gets called if an interrupt handler is not defined. It just
 * enters an infinite loop leaving the processor state intact for a debugger
 * to be examined.
*/
void
defaultHandler(void)
{
	while (1)
	{
	}
}
