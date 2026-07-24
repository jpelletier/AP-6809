/*
 * utils.h
 *
 *  Created on: 25 nov. 2022
 *      Author: jpellet
 */

#ifndef UTILS_H_
#define UTILS_H_

bool IsDataAvailable(void);

void serialLoopback(void);

void serial_gets(char *buffer);
void serial_puts(char *buffer);
void serial_putchar(char c);
char serial_getchar(void);
int serial_printf(char *format, ...);

void removeStringTrailingNewline(char *str);
void GetAnyKey(void);

void AnsiClearDisplay(void);
void AnsiEraseLine(void);
void AnsiSaveCursorPosition(void);
void AnsiRestoreCursorPosition(void);
void AnsiHideCursor(void);
void AnsiShowCursor(void);

int getBit(int bit, int value);
uint8_t swap_nibble(uint8_t in);
uint8_t revert_bits(uint8_t in);
uint16_t revert_word(uint16_t in);

char setword(uint16_t *address);
char setbyte(uint8_t *segment);
char in_b_k(uint8_t *data);
char in_de_k(uint16_t *data);
char setdatabyte(uint8_t *data);
char setdataword(uint16_t *data);

bool Ascii2Hex(char *c);
bool SerialGetAsciiuint8_t(uint8_t *data);
bool SerialGetAsciiWord(uint16_t *addr);

#endif /* UTILS_H_ */
