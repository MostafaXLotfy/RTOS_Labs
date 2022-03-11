#include "stdint.h"
#include "stdbool.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"
#include "BSP.h"

void toggle_blue(void);
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
	IntMasterDisable();
	
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
  // initialize systick counter
  SysTickDisable();
  SysTickPeriodSet(16000 * BSP_SYSTICK_DELAY - 1);
  SysTickEnable();
  SysTickIntRegister(systick_handler);
	
	IntMasterEnable();
	toggle_blue();
  toggle_red();
  
  while(true){
    
  }
  return 0;
}
