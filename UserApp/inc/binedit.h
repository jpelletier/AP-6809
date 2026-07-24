/*
 * binedit.h
 *
 *  Created on: Nov 26, 2022
 *      Author: jpellet
 */

#ifndef _BINEDIT_H_
#define _BINEDIT_H_

void move_cursor(uint16_t hl);
void restore_color(uint16_t address);
uint16_t move_left(uint16_t hl);
uint16_t move_right(uint16_t hl);
uint16_t move_up(uint16_t hl);
uint16_t move_down(uint16_t hl);
void mem_dump(uint16_t address);
void update_dump(uint16_t hl);
void binary_ed(uint16_t address);

#endif /* _BINEDIT_H_ */
