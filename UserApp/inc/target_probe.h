/*
 * target_probe.h
 *
 *  Created on: 19 nov. 2025
 *      Author: jpellet
 */

#ifndef _TARGET_PROBE_H_
#define _TARGET_PROBE_H_

#include <stdio.h>
#include <stdlib.h>
//#include <stdbool.h>	//not supported
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

typedef enum { false, true } bool;

//Conditional defines
#include "config.h"


//https://www.youtube.com/watch?v=wj427hpP81s
//https://github.com/controllerstech/stm32-uart-ring-buffer
#ifdef USE_CODE_RING_BUFFER
#include "UartRingbuffer.h"
#endif

#include "buscycles.h"
#include "target_bits.h"
#include "main.h"
#include "retarget.h"
#include "UserApp.h"
#include "binedit.h"
#include "memops.h"
#include "menus.h"
#include "utils.h"

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#endif /* _TARGET_PROBE_H_ */
