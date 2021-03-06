#ifndef __KERNEL_HARDWARE_POWER_H__
# define __KERNEL_HARDWARE_POWER_H__

#include <stdint.h>
#include <stddef.h>
#include <kernel/bits/union_types.h>
#include <kernel/bits/attributes.h>

struct __sh7305_power_s
{
	volatile long_union(STBCR,		/* Standby Control Register */
		uint32_t const	: 24;
		uint32_t STBY	: 1;
		uint32_t const	: 1;
		uint32_t RSTBY	: 1;
		uint32_t USTBY	: 1;
		uint32_t const	: 4;
	);
	GAPS(12);
	volatile long_union(MSTPCR0,	/* Module Stop Register 0 */
		uint32_t TLB	: 1;	/* TLB */
		uint32_t IC	: 1;	/* Instruction cache */
		uint32_t OC	: 1;	/* Operand cache */
		uint32_t RS	: 1;	/* RS memory */
		uint32_t IL	: 1;	/* IL memory */
		uint32_t SC	: 1;	/* Secondary cache */
		uint32_t const	: 1;	/* All 0 */
		uint32_t FPU	: 1;	/* Floating Point Unit */
		uint32_t const	: 1;	/* All 0*/
		uint32_t INTC	: 1;	/* Interrupt Controller */
		uint32_t DMAC0	: 1;	/* Direct Memory Access Controller 0 */
		uint32_t SuperH	: 1;	/* SuperHyway */
		uint32_t HUID	: 1;	/* User Debugging Interface */
		uint32_t DBG	: 1;	/* Debugging function */
		uint32_t UBC	: 1;	/* User Break Controller */
		uint32_t const	: 1;	/* Reserved (R/W) */
		uint32_t TMU0	: 1;	/* Timer Unit 0 */
		uint32_t CMT	: 1;	/* Compare Match Timer */
		uint32_t RWDT	: 1;	/* Wathdog Timer */
		uint32_t DMAC1	: 1;	/* Direct Memory Access Controller 1 */
		uint32_t const	: 1;	/* All 0 */
		uint32_t TMU1	: 1;	/* Timer Unit 1 */
		uint32_t SCIF0	: 1;	/* Serial Controller Interface 0 */
		uint32_t SCIF1	: 1;	/* Serial Controller Interface 1 (Additionnal timer !!) */
		uint32_t SCIF2	: 1;	/* Serial Controller Interface 2 */
		uint32_t SCIF3	: 1;	/* Serial Controller Interface 3 */
		uint32_t SCIF4	: 1;	/* Serial Controller Interface 4 */
		uint32_t SCIF5	: 1;	/* Serial Controller Interface 5 */
		uint32_t const	: 1;	/* All 0 */
		uint32_t MSIOF0	: 1;	/* Clock-synch. Serial Interface 0 */
		uint32_t MSIOF1	: 1;	/* Clock-synch. Serial Interface 0 */
		uint32_t const	: 1;	/* All 0 */
	);
	volatile long_union(MSTPCR1,	/* Module Stop Register 1 */
		uint32_t const	: 19;	/* All 0 */
		uint32_t KEYSC	: 1;	/* Key Scan Interface */	
		uint32_t RTC	: 1;	/* Real Time Clock */
		uint32_t const 	: 1;	/* All 0 */
		uint32_t I2C0	: 1;	/* I2C 0 */
		uint32_t I2C1	: 1;	/* I2C 1 */
		uint32_t const	: 8;	/* All 0 */
	);
	volatile long_union(MSTPCR2,	/* Module Stop Register 2 */
		uint32_t const	: 2;	/* All 0 */
		uint32_t MMC	: 1;	/* Multi Media Card Interface (?) */
		uint32_t const	: 1;	/* unknwon */
		uint32_t const	: 1;	/* All 0 */
		uint32_t const	: 1;	/* unknown */
		uint32_t TPU	: 1;	/* Timer Pulse Unit */
		uint32_t IrDA	: 1;	/* IrDA interface ? */
		uint32_t const	: 1;	/* All 0 */
		uint32_t const	: 1;	/* unknown */
		uint32_t const	: 1;	/* unknown */
		uint32_t USB0	: 1; 	/* USB 0 */
		uint32_t const	: 1;	/* unknown */
		uint32_t SDHI0	: 1;	/* SD card host 0 ? */
		uint32_t SDHI1	: 1;	/* SD card host 1 ? */
		uint32_t const	: 1;	/* All 0 */
		uint32_t const	: 1;	/* unknown */
		uint32_t const	: 1;	/* All 0 */
		uint32_t const	: 1;	/* unknown */
		uint32_t const	: 1;	/* unknown */
		uint32_t const	: 1;	/* All 0 */
		uint32_t const	: 1;	/* unknown */
		uint32_t SPU	: 1;	/* Sound Processing Unit ? */
		uint32_t const	: 1;	/* all 0 */
		uint32_t const	: 1;	/* unknown */
		uint32_t LCD	: 1;	/* LCD driver */
		uint32_t const	: 1;	/* unknown */
		uint32_t const	: 1;	/* unknown */
		uint32_t const	: 1;	/* unknown */
		uint32_t const	: 1;	/* unknown */
		uint32_t LCDC	: 1;	/* LCD controller ? */
	);
	GAPS(4);
	volatile uint32_t BAR;		/* Boot Address Register */
};

struct __sh7305_power_context
{
	uint32_t stbcr;
	uint32_t mstpcr[3];
	uint32_t bar;
};

#define SH7305_POWER	(*(volatile struct __sh7305_power_s *)0xa4150020)

#endif /*__KERNEL_HARDWARE_POWER_H__*/
