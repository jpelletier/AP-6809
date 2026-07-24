/*---------------------------------------------------------------------------*
 * buscycles.c                                                               *
 * Copyright (C) 2022  Jacques Pelletier                                     *
 *                                                                           *
 * This program is free software; you can redistribute it and *or            *
 * modify it under the terms of the GNU General Public License               *
 * as published by the Free Software Foundation; either version 2            *
 * of the License, or (at your option) any later version.                    *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License         *
 * along with this program; if not, write to the Free Software Foundation,   *
 * Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *---------------------------------------------------------------------------*/
#include <target_probe.h>

void wait_half_cycle(void)
{
	asm("NOP");
	asm("NOP");
}

void wait_1_cycle(void)
{
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
	asm("NOP");
}

//A15-0	D15-0
void TargetMemWr(uint16_t address, uint8_t data)
{
	GPIOD->ODR = address;

	//set direction out
//	GPIOD->MODER = 0x55555555;

	// ALE
//	GPIOB->ODR = 0b1111011000000000;	// AS = 0, R/W = 0, B/W = 1
//	wait_half_cycle();
	// MREQ
//	GPIOB->ODR = 0b1111001100000000;	// AS = 1, MREQ = 0

//	wait_half_cycle();

	GPIOC->ODR = (GPIOC->ODR & 0xFF00) | data; // D0-D7

	//Check wait input
//	if ((GPIOE->IDR & MASK_READY) == 0)
//	{
//		wait_1_cycle();
//	}

	//DS
//	GPIOB->ODR = 0b1111000100000000;	// DS = 0, R/W = 0, MREQ = 0

//	wait_1_cycle();

	//INACTIVE
	GPIOC->ODR = 0xFF00;
}

//A15-0	D15-0
uint8_t TargetMemRd(uint16_t address)
{
	uint8_t data_bus;

	GPIOD->ODR = address;

	// ALE
//	GPIOB->ODR = 0b1111111000000000;	// AS = 0, R/W = 1, B/W = 1
//	wait_half_cycle();
	// MREQ
//	GPIOB->ODR = 0b1111101100000000;	// AS = 1, MREQ = 0

	//set data direction in
	GPIOC->MODER = 0x55550000;

	wait_half_cycle();

	// DS
//	GPIOB->ODR = 0b1110100100000000;	// DS = 0, MREQ = 0

	//Check Ready input
//	if ((GPIOE->IDR & MASK_WAIT) == 0)
//	{
//		wait_1_cycle();
//	}

//	wait_1_cycle();

	data_bus = (GPIOC->IDR) & 0xFF;

	//INACTIVE
	GPIOB->ODR = 0xFF00;

	return data_bus;
}
