#ifndef __BSP_H__
#define __BSP_H__
#include "stdint.h"

#define BSP_SYSTICK_DELAY 100U

void systick_handler(void);
void BSP_delay(uint32_t ticks);
uint32_t BSP_get_tick_counter(void);

#endif