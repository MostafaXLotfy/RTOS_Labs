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
void toggle_blue(void *pvParameters);
void toggle_green(void *pvParameters);


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

void toggle_blue(void *pvParameters){
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_2);
    for(;;){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2, GPIO_PIN_2);
        vTaskDelay(2000 / portTICK_RATE_MS);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_2,0);
        vTaskDelay(2000 / portTICK_RATE_MS);        
    }
    vTaskDelete(NULL);
}

void toggle_green(void *pvParameters){
  GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_3);
    for(;;){
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3, GPIO_PIN_3);
        vTaskDelay(3000 / portTICK_RATE_MS);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_3,0);
        vTaskDelay(3000 / portTICK_RATE_MS);        
    }
    vTaskDelete(NULL);
}

int main()
{    
  SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
  while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
		
  xTaskCreate(toggle_red, "toggle_red", 120, NULL, 1, NULL);
  xTaskCreate(toggle_blue, "toggle_blue", 120, NULL, 2, NULL);
  xTaskCreate(toggle_green, "toggle_gree", 120, NULL, 3, NULL);

  vTaskStartScheduler();
  while(true){
    
  }
  return 0;
}
