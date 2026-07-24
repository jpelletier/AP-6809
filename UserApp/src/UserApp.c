/*---------------------------------------------------------------------------*
 * UserApp.c                                                                 *
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

//bool Interrupt_Pending = false;
//bool NMI_Pending = false;

void setup(void) {

#ifdef USE_CODE_RING_BUFFER
	Ringbuf_init();
#endif //USE_CODE_RING_BUFFER
	/*
	How to Use printf on STM32
	https://shawnhymel.com/1873/how-to-use-printf-on-stm32/
	*/
}

void loop(void) {
	mainMenu();
}

