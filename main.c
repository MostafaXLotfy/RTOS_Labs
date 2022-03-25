#include "stdint.h"
#include "stdbool.h"

/*
    free rtos includes 
*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"

/*
    tivaware includes 
*/
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "inc/hw_gpio.h"
#include "driverlib/interrupt.h"

void toggle_red(void *pvParameters);


void toggle_red(void *pvParameters){
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
    for(;;){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, GPIO_PIN_1);
        vTaskDelay(1000 / portTICK_RATE_MS);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1,0);
        vTaskDelay(1000 / portTICK_RATE_MS);        
    }
    vTaskDelete(NULL);
}


int main()
{    
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
		
  xTaskCreate(toggle_red, "toggle_red", 240, NULL, 1, NULL);

  vTaskStartScheduler();
  while(true){
    
  }
  return 0;
}
