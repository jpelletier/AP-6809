/*---------------------------------------------------------------------------*
 * menus.c                                                               *
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

void mainMenu(void)
{
	char c;

	//Turns on software flow control
	serial_puts("\021\033[H\033[2J6809 Alien probe\r\n\n");	//TARGET SPECIFIC NAME
	serial_puts(
			"A: Pin Test\r\n"
			"B: Bus transactions\r\n"
			"C: Bus repeated transactions\r\n"
			"D: Memory operations\r\n"
/*
			"D: \r\n"

			//Need 1 input pin for a chip select
			"G: Memory scan Chip Select\r\n"
			"H: IO Scan Chip Select\r\n"
*/
			);
	c = toupper(serial_getchar());
	switch(c)
	{
		case 'A':
			PinTestMenu();
			break;
		case 'B':
			BusTransactionsMenu();
			break;
		case 'C':
			BusRepeatTransactionsMenu();
			break;
		case 'D':
			MemoryOperationsMenu();
			break;
			/*
		case 'E':
			break;
		case 'F':
			MemoryOperationsMenu();
			break;
*/
		default:
			;
	}
}

/*
	A15-0	D15-0
	D7-0	C7-0

	Inputs
	BIT_DMA		E0
	BIT_RESET	E1
	BIT_NMI		E2
	BIT_IRQ		E3
	BIT_FIRQ	E4
	BIT_MRDY	E5

	Outputs
	BIT_RW	 	C8
	BIT_E		C9
	BIT_Q		C10
	BIT_BS		C11
	BIT_BA		C12
	BIT_HALT	C13
*/

/* Toggle and monitor each pin individually */
void PinTestMenu(void)
{
	char c;
	uint16_t addr;
	uint8_t data;
	uint8_t ctl;
	uint8_t status;

	//INACTIVE
	GPIOC->ODR = 0xF000;

	serial_puts("\033[H\033[2J6809 Pin test\r\n");	//TARGET SPECIFIC NAME
	serial_puts(
			"F-0: toggle bits A15-A0\r\n"
			"G-N: toggle bits D7-D0\r\n"
			"@: set address\r\n"
			"#: set data\r\n"
			"-: input        +: output\r\n"

			//TARGET SPECIFIC SIGNALS
			"O: toggle R/W̅\r\n"
			"P: toggle E\r\n"
			"Q: toggle Q\r\n"
			"R: toggle BS\r\n"
			"S: toggle BA\r\n"
			"T: toggle HALT\r\n"

			"\033[32mESC: return to previous menu\033[m\r\n\n"
			);

	AnsiSaveCursorPosition();
	AnsiHideCursor();

	//A: 0000: 00\r\n",
	//-------ADDR--------  --DATA---
	//0000 0000 0000 0000  0000 0000
	serial_printf("\033[16;1H-------ADDR--------  --DATA---\r\n");	//TARGET SPECIFIC VERTICAL POSITION
	serial_printf("FEDC BA98 7654 3210  7654 3210\r\n");

	do
	{
		addr = GPIOD->ODR;
		data = (GPIOC->ODR) & 0xFF;
		ctl = ((GPIOC->ODR) >> 8) & 0x3F;
		status = (GPIOE->IDR) & 0x3F;

		//TARGET SPECIFIC VERTICAL POSITION
		serial_printf("\033[15;1H\033[37;44m%04X: %02X\033[m\r\n",addr,data);
		serial_printf("\033[18;1H\033[37;44m%1d%1d%1d%1d %1d%1d%1d%1d %1d%1d%1d%1d %1d%1d%1d%1d  %1d%1d%1d%1d %1d%1d%1d%1d\033[m\r\n",
				getBit(15,addr),getBit(14,addr),getBit(13,addr),getBit(12,addr),
				getBit(11,addr),getBit(10,addr),getBit(9,addr), getBit(8,addr),
				getBit(7,addr),	getBit(6,addr),	getBit(5,addr),	getBit(4,addr),
				getBit(3,addr),	getBit(2,addr), getBit(1,addr),	getBit(0,addr),
				getBit(7,data),	getBit(6,data),	getBit(5,data), getBit(4,data),
				getBit(3,data),	getBit(2,data),	getBit(1,data), getBit(0,data));

		//TARGET SPECIFIC SIGNALS
		//DMA:   0  N̅M̅I̅: 0  MRDY: 0   |   R/W̅:  1  E: 0  BS: 0
		//RESET: 0  I̅R̅Q̅: 0  F̅I̅R̅Q̅: 0   |   H̅A̅L̅T̅: 1  Q: 0  BA: 0
		serial_printf("DMA:   \033[37;44m%1d\033[m  N̅M̅I̅: \033[37;44m%1d\033[m  MRDY: \033[37;44m%1d\033[m   |   "
					  "R/W̅:  \033[37;44m%1d\033[m  E: \033[37;44m%1d\033[m  BS: \033[37;44m%1d\033[m\r\n",
				getBit(BIT_DMA,status),getBit(BIT_NMI,status),getBit(BIT_MRDY,status),
				getBit(BIT_RW,ctl), getBit(BIT_E,ctl), getBit(BIT_BS,ctl));

		//TARGET SPECIFIC SIGNALS
		serial_printf("RESET: \033[37;44m%1d\033[m  I̅R̅Q̅: \033[37;44m%1d\033[m  F̅I̅R̅Q̅: \033[37;44m%1d\033[m   |   "
					  "H̅A̅L̅T̅: \033[37;44m%1d\033[m  Q: \033[37;44m%1d\033[m  BA: \033[37;44m%1d\033[m\r\n",
				getBit(BIT_RESET,status),getBit(BIT_IRQ,status),getBit(BIT_FIRQ,status),
				getBit(BIT_HALT,ctl),getBit(BIT_Q,ctl), getBit(BIT_BA,ctl));

		if (IsDataAvailable()) {
			c = toupper(serial_getchar());
			switch(c)
			{
				case '0':
					addr ^= 0x0001;
					break;
				case '1':
					addr ^= 0x0002;
					break;
				case '2':
					addr ^= 0x0004;
					break;
				case '3':
					addr ^= 0x0008;
					break;
				case '4':
					addr ^= 0x0010;
					break;
				case '5':
					addr ^= 0x0020;
					break;
				case '6':
					addr ^= 0x0040;
					break;
				case '7':
					addr ^= 0x0080;
					break;
				case '8':
					addr ^= 0x0100;
					break;
				case '9':
					addr ^= 0x0200;
					break;
				case 'A':
					addr ^= 0x0400;
					break;
				case 'B':
					addr ^= 0x0800;
					break;
				case 'C':
					addr ^= 0x1000;
					break;
				case 'D':
					addr ^= 0x2000;
					break;
				case 'E':
					addr ^= 0x4000;
					break;
				case 'F':
					addr ^= 0x8000;
					break;
				case 'G':
					data ^= 0x0080;
					break;
				case 'H':
					data ^= 0x0040;
					break;
				case 'I':
					data ^= 0x0020;
					break;
				case 'J':
					data ^= 0x0010;
					break;
				case 'K':
					data ^= 0x0008;
					break;
				case 'L':
					data ^= 0x0004;
					break;
				case 'M':
					data ^= 0x0002;
					break;
				case 'N':
					data ^= 0x0001;
					break;
				case 'O':
					ctl ^= 0x01;
					break;
				case 'P':
					ctl ^= 0x02;
					break;
				case 'Q':
					ctl ^= 0x04;
					break;
				case 'R':
					ctl ^= 0x08;
					break;
				case 'S':
					ctl ^= 0x10;
					break;
				case 'T':
					ctl ^= 0x20;
					break;
				case '-':
					//set data direction in
					GPIOC->MODER = 0x55550000;
					break;
				case '+':
					//set data direction out
					GPIOC->MODER = 0x55555555;
					break;
				case '@':
					serial_puts("\033[15;1H\033[33;45m"); //TARGET SPECIFIC VERTICAL POSITION
					setword(&addr);
					serial_puts("\033[m");
					break;
				case '#':
					serial_puts("\033[15;7H\033[33;45m"); //TARGET SPECIFIC VERTICAL POSITION
					setbyte(&data);
					serial_puts("\033[m");
					break;
				case ESC:
					AnsiShowCursor();
					return;
				default:
					;
			}
			GPIOD->ODR = addr;
			GPIOC->ODR = (((uint16_t) ctl) << 8) | data;
		}
		else
		{
			HAL_Delay(100);
		}
		AnsiRestoreCursorPosition();

	} while (true);
}

void BusTransactionsMenu(void)
{
	char c;
	uint16_t addr = 0;
	uint8_t data = 0;

	serial_puts("\033[H\033[2JBus transactions\r\n");
	serial_puts(
			"                              Byte/Word edit\r\n"
			"@: set address                + increment\r\n"
			"#: set data                   - decrement\r\n"
			"A: do 1 memory read cycle     Z reset to zero\r\n"
			"B: do 1 memory write cycle    ! invert bits\r\n"
			"                              ~ negate          (value -> -value)\r\n"
			"                              R reverse bits    (0..7 -> 7..0)\r\n"
			"                              S swap nibble     (7..4,3..0) -> (3..0,7..4)\r\n"
			"                             ⌫  remove nibble   (3F -> 03)\r\n"
			"                              Q abandon changes\r\n"
			"\033[32mESC: return to previous menu\033[m\r\n\n"
			);

	AnsiSaveCursorPosition();

	do
	{
		serial_printf("\033[14;1H\033[37;44m%04X %02X\033[m\r\n", addr, data);

		c = toupper(serial_getchar());
		switch(c)
		{
			case '@':
				serial_puts("\033[14;1H\033[33;45m"); //Address
				setword(&addr);
				serial_puts("\033[m");
				break;
			case '#':
				serial_puts("\033[14;6H\033[33;45m"); //Data
				setbyte(&data);
				serial_puts("\033[m");
				break;
			case 'A':
				data = TargetMemRd(addr);
				break;
			case 'B':
				TargetMemWr(addr,data);
				break;
			case ESC:
				return;
		}
		AnsiRestoreCursorPosition();

	} while (true);
}

/* hardware diag with scope */
void BusRepeatTransactionsMenu(void)
{
	char c;
	uint16_t addr = 0;
	uint8_t data = 0;

	serial_puts("\033[H\033[2JBus transaction loops\r\n");
	serial_puts(
			"                              Byte/Word edit\r\n"
			"@: set address                + increment\r\n"
			"#: set data                   - decrement\r\n"
			"A: memory read cycles         Z reset to zero\r\n"
			"B: memory write cycles        ! invert bits\r\n"
			"                              ~ negate          (value = -value)\r\n"
			"                              R reverse bits    (0..7 -> 7..0)\r\n"
			"                              S swap            \r\n"

			"\033[32mESC: previous menu\033[m           ⌫  remove nibble   (3F - 03)\r\n"
			"Any: stop loop                Q abandon changes\r\n\n"
			);

	do
	{
		serial_printf("\033[13;1H\033[37;44m%04X %02X\033[m\r\n", addr, data);

		c = toupper(serial_getchar());
		switch(c)
		{
			case '@':
				serial_puts("\033[13;1H\033[33;45m"); //Address
				setword(&addr);
				serial_puts("\033[m");
				break;
			case '#':
				serial_puts("\033[13;6H\033[33;45m"); //Data
				setbyte(&data);
				serial_puts("\033[m");
				break;
			case 'A':
				AnsiSaveCursorPosition();
				AnsiHideCursor();
				do
				{
					data = TargetMemRd(addr);
					serial_printf("Mem read at %04X: %02X\r\n", addr, data);
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				AnsiEraseLine();
				break;
			case 'B':
				AnsiSaveCursorPosition();
				AnsiHideCursor();
				do
				{
					serial_printf("Mem write at %04X: %02X\r\n", addr, data);
					TargetMemWr(addr, data);
					AnsiRestoreCursorPosition();
				} while (!IsDataAvailable());
				AnsiEraseLine();
				break;
			case ESC:
				AnsiShowCursor();
				return;
		}
		// Go to 13:1
		serial_puts("\033[13;1H");
	} while (true);
}

void MemoryOperationsMenu(void)
{
	char c;
//	uint8_t data_byte = 0;
//	uint8_t port = 0;

	AnsiSaveCursorPosition();

	do
	{
		serial_puts("\033[H\033[2JMemory operations\r\n\n");
		serial_puts(
				"E: edit memory\r\n"
//				"L: load Intel hex file\r\n"
				"F: fill memory\r\n"
				"M: move memory\r\n"
//				"I: input\r\n"
//				"O: output\r\n"
				"Q: quit\r\n\n"
				);
		c = toupper(serial_getchar());
		switch(c)
		{
			case 'E':
				binary_ed(0);
				break;
//			case 'L':
//				loadIntelHex();
//				GetAnyKey();
//				break;
			case 'F':
				fillMemory();
				break;
			case 'M':
				moveMemory();
				break;
/*
Implement Input outputs for memory mapped peripherals
			case 'I':
				serial_puts("Input port: ");
				c = setdatabyte(&port);
				if (c != 'Q')
				{
					data_byte = TargetIoRd(0, port);
					serial_printf(" %02X\r\n",data_byte);
				}
				break;
			case 'O':
				serial_puts("Output port: ");
				c = setdatabyte(&port);
				if (c != 'Q')
				{
					putchar(' ');
					c = setdatabyte(&data_byte);
					if (c != 'Q')
					{
						TargetIoWr(0,port, data_byte);
						serial_puts("\r\n");
					}
				}
				break;
*/
			case 'Q':
				return;
		}

		AnsiRestoreCursorPosition();

	} while (true);
}
