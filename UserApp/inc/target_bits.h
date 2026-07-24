/*
 * target_bits.h
 */

#ifndef _TARGET_BITS_H_
#define _TARGET_BITS_H_

#define BIT_DMA		0
#define BIT_RESET	1
#define BIT_NMI		2
#define BIT_IRQ		3
#define BIT_FIRQ	4
#define BIT_MRDY	5

#define BIT_RW	 	8
#define BIT_E		9
#define BIT_Q		10
#define BIT_BS		9
#define BIT_BA		10
#define BIT_HALT	11

#define MASK_DMA	(1<<BIT_DMA)
#define MASK_RESET	(1<<BIT_RESET)
#define MASK_NMI	(1<<BIT_NMI)
#define MASK_IRQ	(1<<BIT_IRQ)
#define MASK_FIRQ	(1<<BIT_FIRQ)
#define MASK_MRDY	(1<<BIT_MRDY)

#define MASK_RW		(1<<BIT_RW)
#define MASK_E		(1<<BIT_E)
#define MASK_Q  	(1<<BIT_Q)
#define MASK_BS		(1<<BIT_BS)
#define MASK_BA  	(1<<BIT_BA)
#define MASK_HALT	(1<<BIT_HALT)

#endif /* _TARGET_BITS_H_ */
