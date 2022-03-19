#include "stdint.h"
#include "stdbool.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "BSP.h"

uint32_t toggle_blue_stack[40];
uint32_t *sp_toggle_blue = &toggle_blue_stack[40];
void toggle_blue(void);

uint32_t toggle_red_stack[40];
uint32_t *sp_toggle_red = &toggle_red_stack[40];
void toggle_red(void);


void toggle_red(void){
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);  
  while(true){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
		BSP_delay(500 / BSP_SYSTICK_DELAY);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0);    
		BSP_delay(500 / BSP_SYSTICK_DELAY);
  }
}

void toggle_blue(void){
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);
  while(true){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
    BSP_delay(1000 / BSP_SYSTICK_DELAY);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, 0);    
		BSP_delay(1000 / BSP_SYSTICK_DELAY);
  }
}


int main()
{	
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
  // initialize systick counter
  SysTickDisable();
  SysTickPeriodSet(16000 * BSP_SYSTICK_DELAY - 1);
  SysTickEnable();
  SysTickIntRegister(systick_handler);
	
	IntMasterEnable();
	*(--sp_toggle_blue) = (1U << 24);  /* xPSR */
  *(--sp_toggle_blue) = (uint32_t)&toggle_blue; /* PC */
  *(--sp_toggle_blue) = 0x0000000EU; /* LR  */
  *(--sp_toggle_blue) = 0x0000000CU; /* R12 */
  *(--sp_toggle_blue) = 0x00000003U; /* R3  */
  *(--sp_toggle_blue) = 0x00000002U; /* R2  */
  *(--sp_toggle_blue) = 0x00000001U; /* R1  */
  *(--sp_toggle_blue) = 0x00000000U; /* R0  */
  /* fabricate Cortex-M ISR stack frame for blinky2 */
  *(--sp_toggle_red) = (1U << 24);  /* xPSR */
  *(--sp_toggle_red) = (uint32_t)&toggle_red; /* PC */
  *(--sp_toggle_red) = 0x0000000EU; /* LR  */
  *(--sp_toggle_red) = 0x0000000CU; /* R12 */
  *(--sp_toggle_red) = 0x00000033U; /* R3  */
  *(--sp_toggle_red) = 0x00000022U; /* R2  */
  *(--sp_toggle_red) = 0x00000011U; /* R1  */
  *(--sp_toggle_red) = 0x00000000U; /* R0  */
  
	toggle_red();
	toggle_blue();
  
  while(true){
    
  }
  return 0;
}
