/*---------------------------------------------------------------------------*
 * utils.c                                                                   *
 * Copyright (C) 2025  Jacques Pelletier                                     *
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

extern UART_HandleTypeDef huart1;

/* Mcurses
void App_putchar(uint8_t c)
{
    putchar(c);
}

char App_getchar(void)
{
    return getchar();
}
*/

bool IsDataAvailable(void)
{
	return (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) == SET);
}

void serialLoopback(void)
{
	//Serial loopback
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // 2. Configure PA9 as Output
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // Push-Pull Output
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    // 3. Configure PA10 as Input
    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT; // Input mode
    GPIO_InitStruct.Pull = GPIO_NOPULL; // Or GPIO_PULLUP/GPIO_PULLDOWN
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    do
    {
		if ((GPIOA->IDR & GPIO_PIN_10) == 0)
		{
			GPIOA->ODR &= ~GPIO_PIN_9;
		}
		else
		{
			GPIOA->ODR |= GPIO_PIN_9;
		}
    }
	while(true);
}

void serial_gets(char *buffer)
{
int result;

	do
	{
		result = serial_getchar();

		switch(result)
		{
		//case '\r':
		case '\n':
			*buffer = 0;
			return;
		default:
			*buffer++ = result & 0xff;
		}
	} while (true);
}

void serial_puts(char *buffer)
{
	while(*buffer) serial_putchar(*buffer++);
}

void serial_putchar(char c) {
    // Correct 4-argument call with pointer casting
    if (HAL_UART_Transmit(&huart1, (uint8_t *)&c, 1, 100) == HAL_OK) {
        // Transmission successful
    }
}

/**
  * @brief  Receives an amount of data in blocking mode.
  * @note   When UART parity is not enabled (PCE = 0), and Word Length is configured to 9 bits (M1-M0 = 01),
  *         the received data is handled as a set of u16. In this case, Size must indicate the number
  *         of u16 available through pData.
  * @param  huart Pointer to a UART_HandleTypeDef structure that contains
  *               the configuration information for the specified UART module.
  * @param  pData Pointer to data buffer (u8 or u16 data elements).
  * @param  Size  Amount of data elements (u8 or u16) to be received.
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
//HAL_StatusTypeDef HAL_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint16_t Size, uint32_t Timeout)

char serial_getchar(void)
{
uint8_t c;
HAL_StatusTypeDef status;

	do
	{
		status = HAL_UART_Receive(&huart1,&c,1,100);
	}
	while(status != HAL_OK);

	return c;
}

char buffer[80];

int serial_printf(char *format, ...)
{
   va_list aptr;
   int ret;

   va_start(aptr, format);
   ret = vsprintf(buffer, format, aptr);
   va_end(aptr);

   serial_puts(buffer);
   return(ret);
}

// https://siongui.github.io/2013/01/09/c-remove-string-trailing-newline-carriage-return/
void removeStringTrailingNewline(char *str)
{
  if (str == NULL)
    return;
  int length = strlen(str);
  if (str[length-1] == '\n' || str[length-1] == '\r')
    str[length-1]  = '\0';
  if (str[length-2] == '\n' || str[length-2] == '\r')
    str[length-2]  = '\0';
}

void GetAnyKey(void)
{
	serial_puts("\r\nPress any key to continue");

	serial_getchar();
	serial_puts("\033[1K\r");
}

void AnsiClearDisplay(void)
{
	serial_puts("\033[2J");
}

void AnsiEraseLine(void)
{
	serial_puts("\033[2K");
}

void AnsiSaveCursorPosition(void)
{
	serial_puts("\033[s");
}

void AnsiRestoreCursorPosition(void)
{
	serial_puts("\033[u");
}

void AnsiHideCursor(void)
{
	serial_puts("\033[?25l");
}

void AnsiShowCursor(void)
{
	serial_puts("\033[?25h");
}

int getBit(int bit, int value)
{
	return (value >> bit) & 1;
}

uint8_t swap_nibble(uint8_t in)
{
	uint8_t temp = in;

	return (temp << 4) | (temp >> 4);
}

uint8_t revert_bits(uint8_t in)
{
	uint8_t temp = 0;

	temp |= (in & 0x80) >> 7;
	temp |= (in & 0x40) >> 5;
	temp |= (in & 0x20) >> 3;
	temp |= (in & 0x10) >> 1;
	temp |= (in & 0x08) << 1;
	temp |= (in & 0x04) << 3;
	temp |= (in & 0x02) << 5;
	temp |= (in & 0x01) << 7;

	return temp;
}

uint16_t revert_word(uint16_t in)
{
	uint8_t h,l;

	h = revert_bits(in & 0xff);
	l = revert_bits(in >> 8);

	return (h << 8) | l;
}

char setword(uint16_t *value)
{
	char c;
	uint16_t temp;

	temp = *value;

	do
	{
		serial_printf("%04X",temp);

		c = toupper(serial_getchar());

		switch(c)
		{
			case '0' ... '9':
				temp = (temp << 4) | (c & 0x0f);
				break;
			case 'A' ... 'F':
				temp = (temp << 4) | (c - 'A' + 0x0A);
				break;
			case '+':
				temp++;
				break;
			case '-':
				temp--;
				break;
			case 'Z':
				temp = 0;
				break;
			case 'R':
				temp = revert_word(temp);
				break;
			case 'S':
				temp = (temp >> 8) | (temp << 8);
				break;
			case BSP_KEY:
				temp >>= 4;
				break;
			case '~': //neg = !(--HL) = (!HL)++
				temp--;
				//fallthrough
			case '!':
				temp ^= 0xFFFF;
				break;
			case 'Q':
				return c;
			case 0x0d:
				*value = temp;
				return c;
			default:
				;
		}
		serial_puts("\b\b\b\b");
	} while (true);

	return 0;
}

char setbyte(uint8_t *value)
{
	char c;
	uint8_t temp;

	temp = *value;

	do
	{
		serial_printf("%02X",temp);

		c = toupper(serial_getchar());

		switch(c)
		{
			case '0' ... '9':
				temp = (temp << 4) | (c & 0x0f);
				break;
			case 'A' ... 'F':
				temp = (temp << 4) | (c - 'A' + 0x0A);
				break;
			case '+':
				temp++;
				break;
			case '-':
				temp--;
				break;
			case 'Z':
				temp = 0;
				break;
			case 'R':
				temp = revert_bits(temp);
				break;
			case 'S':
				temp = swap_nibble(temp);
				break;
			case BSP_KEY:
				temp >>= 4;
				break;
			case '~': //neg = !(--HL) = (!HL)++
				temp--;
				//fallthrough
			case '!':
				temp ^= 0xFF;
				break;
			case 'Q':
				return c;
			case 0x0d:
				*value = temp;	// keep only 6 lowest bits
				return c;
			default:
				;
		}
		serial_puts("\b\b");
	} while (true);

	return 0;
}

/* Edit byte value with some handy transformations
 * z:	reset to 00
 * +:	increment
 * -:	decrement
 * !:	invert
 * ~:	negate (value = -value)
 * R:	revert bits (0..7 -> 7..0)
 * BSP:	remove nibble (3F -> 03)
 */

char in_b_k(uint8_t *data)
{
	char c;
	uint8_t temp;

	temp = *data;

	do
	{
		serial_printf("%02X",temp);

		c = toupper(serial_getchar());

		switch(c)
		{
			case '0' ... '9':
				temp = (temp << 4) | (c & 0x0f);
				break;
			case 'A' ... 'F':
				temp = (temp << 4) | (c - 'A' + 0x0A);
				break;
			case '+':
				temp++;
				break;
			case '-':
				temp--;
				break;
			case 'Z':
				temp = 0;
				break;
			case BSP_KEY:
				temp >>= 4;
				break;
			case 'R': // revert bits 0..7 -> 7..0
				temp = revert_bits(temp);
				break;
			case 'S': // revert bits 0..7 -> 7..0
				temp = swap_nibble(temp);
				break;
			case '~': //neg = !(--B) = (!B)++
				temp--;
				//fallthrough
			case '!':
				temp ^= 0xFF;
				break;
			default:
				*data = temp;
				return c;
		}
		serial_puts("\b\b");
	} while (true);

	return 0;
}

/* Edit word value with some handy transformations
 * z:	reset to 00
 * +:	increment
 * -:	decrement
 * *:	add 0x0100
 * /:	sub 0x0100
 * !:	invert
 * ~:	negate (value = -value)
 * R:	revert bits (0..7 -> 7..0)
 * S:	swap high and low bytes
 * BSP:	remove nibble (3F -> 03)
 */

char in_de_k(uint16_t *data)
{
	char c;
	uint16_t temp;

	temp = *data;

	do
	{
		serial_printf("%04X",temp);

		c = toupper(serial_getchar());

		switch(c)
		{
			case '0' ... '9':
				temp = (temp << 4) | (c & 0x0f);
				break;
			case 'A' ... 'F':
				temp = (temp << 4) | (c - 'A' + 0x0A);
				break;
			case '+':
				temp++;
				break;
			case '-':
				temp--;
				break;
			case '*':
				temp += 0x100;
				break;
			case '/':
				temp -= 0x100;
				break;
			case 'Z':
				temp = 0;
				break;
			case BSP_KEY:
				temp >>= 4;
				break;
			case 'R': // revert bits 0..7 -> 7..0
				temp = revert_word(temp);
				break;
			case 'S': // swap bytes
				c = temp & 0xFF;
				temp >>= 8;
				temp |= (c << 8);
				break;
			case '~': //neg = !(--B) = (!B)++
				temp--;
				//fallthrough
			case '!':
				temp ^= 0xFFFF;
				break;
			default:
				*data = temp;
				return c;
		}
		serial_puts("\b\b\b\b");
	} while (true);

	return 0;
}

char setdatabyte(uint8_t *data)
{
	char c;

	do
	{
		c = in_b_k(data);
	} while (!((c == 'Q')||(c == 0x0d)));

	return c;
}

char setdataword(uint16_t *data)
{
	char c;

	do
	{
		c = in_de_k(data);
	} while (!((c == 'Q')||(c == 0x0d)));

	return c;
}

bool Ascii2Hex(char *c)
{
	bool result = false;

	//'0' to '9' or 'A' to 'F'
	switch(toupper(*c))
	{
		case '0' ... '9':
			*c = *c - '0';
			result = true;
			break;

		case 'A' ... 'F':
        	*c = *c - 'A' + 0x0a;
			result = true;
			break;
		default:
		;
	}

	return result;
}

bool SerialGetAsciiByte(uint8_t *data)
{
char c, b;
bool result = false;

	c = serial_getchar();
	if (Ascii2Hex(&c))
	{
		//put the first nibble (MSB) into bits 7-4
		b = c << 4;

		c = serial_getchar();
		if (Ascii2Hex(&c))
		{
			//combine first nibble (MSB) with second nibble (LSB)
			b |= c;
			*data = b;
			result = true;
		}
	}
	return result;
}

bool SerialGetAsciiWord(uint16_t *addr)
{
	uint8_t low, high;
	bool result = false;

	if (SerialGetAsciiByte(&high))
	{
		if (SerialGetAsciiByte(&low))
		{
			*addr = (high << 8) | low;
			result = true;
		}
	}
	return result;
}
