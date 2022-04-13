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
#include "tm4c123gh6pm.h"

void toggle_red(void *pvParameters);
void toggle_blue(void *pvParameters);
void toggle_green(void *pvParameters);

void toggle_red(void *pvParameters){
    for(;;){
				GPIO_PORTF_DATA_R ^= 0x2;
        vTaskDelay(1000 / portTICK_RATE_MS);      
    }
    vTaskDelete(NULL);
}


void toggle_green(void *pvParameters){
    for(;;){
				GPIO_PORTF_DATA_R ^= 0x8;
        vTaskDelay(2000 / portTICK_RATE_MS);   
    }
    vTaskDelete(NULL);
}

void vApplicationIdleHook(){
		GPIO_PORTF_DATA_R ^= 0x4;
}

int main()
{    
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_2);
	GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);


  xTaskCreate(toggle_red, "toggle_red", 120, NULL, 1, NULL);
  xTaskCreate(toggle_green, "toggle_green", 120, NULL, 1
		, NULL);

  vTaskStartScheduler();
  while(true){
    
  }
  return 0;
}
