#include "stdint.h"
#include "stdbool.h"
#include "driverlib/interrupt.h"
#include "BSP.h"

static uint32_t volatile g_ticks_counter = 0;

uint32_t BSP_get_tick_counter(void){
	uint32_t ticks_counter = 0;
	IntMasterDisable();
	ticks_counter = g_ticks_counter;
	IntMasterEnable();
	return ticks_counter;
}

void systick_handler(){
	g_ticks_counter++;
}

void BSP_delay(uint32_t ticks){
	uint32_t start = BSP_get_tick_counter();
	while((BSP_get_tick_counter() - start) < ticks){}
}
