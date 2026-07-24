/*---------------------------------------------------------------------------*
 * binedit.c                                                                 *
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

char msg_help[] =
		"\033[46m"
		"\033[2;71H+: incr  "
		"\033[3;71H-: decr  "
		"\033[4;71H!: invert"
		"\033[5;71H~: negate"

		"\033[6;71HR: revers"
		"\033[7;71HS: swap  "
		"\033[8;71HBSP: SHR "
		"\033[9;71HZ: 00    "
		"\033[10;71HX: exit  "

		"\033[11;71HSP: store"
		"\033[12;71HCR: store"

		"\033[45m"
		"\033[13;71HT: L=00  "
		"\033[14;71HH: ->H   "

		"\033[15;71HL: ->L   "
		"\033[16;71HG: jmp   "
		"\033[17;71HU: ->HL  "
		"\033[18;71H         "

		"\033[20;1H"
        "J: Jump relative\r\n"
        "N: Next 256 block\r\n"
        "P: Prev 256 block\r\n\033[m";

void move_cursor(ushort hl)
{
uint8_t row = ((hl & 0xff) >> 4) + 3;
uint8_t column = 3 * (hl & 0x0f) + 8; //6;

	serial_printf("\033[%d;%dH",row,column);
}

void restore_color(ushort address)
{
	serial_printf("\b\b\033[m%02X",TargetMemRd(address));
}

ushort move_left(ushort hl)
{
	restore_color(hl);

	//roll decrement
	hl = ((hl - 1) & 0x0f) | (hl & 0xfff0);
    move_cursor(hl);

    return hl;
}

ushort move_right(ushort hl)
{
	restore_color(hl);

	//roll increment
	hl = ((hl + 1) & 0x0f) | (hl & 0xfff0);
    move_cursor(hl);

    return hl;
}

ushort move_up(ushort hl)
{
	restore_color(hl);

	//roll decrement
	hl = ((hl - 0x10) & 0xff) | (hl & 0xff00);
    move_cursor(hl);

    return hl;
}

ushort move_down(ushort hl)
{
	restore_color(hl);

	//roll increment
	hl = ((hl + 0x10) & 0xff) | (hl & 0xff00);
    move_cursor(hl);

    return hl;
}

void mem_dump(ushort address)
{
	uint8_t a,b;
	ushort hl;

	hl = address & 0xFF00;

	serial_puts("      0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\r\n");

    do
    {
		serial_printf("%04X ",hl);

		b = 16;
		do
		{
			serial_printf("%02X ",TargetMemRd(hl++));
		} while (--b);

		hl -= 16;

		b = 16;
		do
		{
			a = TargetMemRd(hl++);

			if ((a < 0x20) || (a > 0x7f)) a = '.';
			serial_putchar(a);
		} while (--b);

		serial_puts("\r\n");

    } while (hl & 0x00f0);
}

void update_dump(ushort hl)
{
	//home
	serial_puts("\033[m\033[2;1H");

	//redraw
	mem_dump(hl & 0xff00);
	serial_puts(msg_help);
	move_cursor(hl);
}

//uint8_t edit
//+      increment
//-      decrement
//!      invert
//~      negate
//R      reverse bits 7:0 -> 0:7
//S		 swap nibbles
//bsp    remove previous hex
//Z      00

//enter  accept uint8_t and exit uint8_t edit
//Q      scrap uint8_t and exit uint8_t edit
//
//c-A    left
//c-D    right
//c-W    up
//c-X    down
//T      L = 00
//H		 value -> L
//L      value -> L
//P      previous 256-byte block
//N      next 256-byte block
//X      exit binary editor

void binary_ed(uint16_t address)
{
	uint16_t hl = address;
	uint8_t a,b;

	restore_color(hl);

	serial_puts("\033[H\033[2JBinary editor\r\n");
    mem_dump(address);
	serial_puts(msg_help);

    //goto row 3 column 6
    serial_puts("\033[3;6H");

    do
    {
        serial_puts("\033[44m");

        b = TargetMemRd(hl);

        //uint8_t edit
        a = in_b_k(&b);

        switch(a)
        {
        	//c-A arrow left
        	case 0x01:
        		hl = move_left(hl);
				break;

        	//c-D arrow right
        	case 0x04:
        		hl = move_right(hl);
				break;

        	//c-X arrow down
        	case 0x18:
        		hl = move_down(hl);
				break;

        	//c-W arrow up
        	case 0x17:
        		hl = move_up(hl);
				break;

        	case 0x1B:
        		a = serial_getchar();

        		if (a == '[')
        		{
            		a = serial_getchar();

            		switch(a)
            		{
            		case 'A':
                		hl = move_up(hl);
        				break;
            		case 'B':
                		hl = move_down(hl);
        				break;
            		case 'C':
                		hl = move_right(hl);
        				break;
            		case 'D':
                		hl = move_left(hl);
        				break;
            		}
        		}
        		break;

        	// absolute jump
        	case 'G':
        		hl = b | (TargetMemRd(hl+1) << 8);

				update_dump(hl);
				break;

			case 'T':
            	restore_color(hl);
            	hl = hl & 0xff00;
				move_cursor(hl);
				break;

            // high -> H = b
        	case 'H':
				hl = b << 8;
				update_dump(hl);
				break;

			case 'L':
            	restore_color(hl);
            	hl = (hl & 0xff00) | b;
				move_cursor(hl);
				break;

            //relative jump
            case 'J':
            	restore_color(hl);

        		hl = hl + b + 1;
            	if (b > 0x7f) hl -= 0x100;

            	update_dump(hl);
            	break;

            //previous 256-byte block
            case 'P':
            	hl -= 256;
				//keep color
				update_dump(hl);
            	break;

            //next 256-byte block
            case 'N':
            	hl += 256;
				//keep color
				update_dump(hl);
            	break;

            case 'X':
            	//exit editor
            	restore_color(hl);
            	AnsiClearDisplay();
            	return;

            case 0x0d:
            case ' ':
            	TargetMemRd(hl);
            	hl = move_right(hl);
            	break;

            default:
            	;
		}

        serial_putchar('\b');
    	serial_putchar('\b');

    } while (true);
}

