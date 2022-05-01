#include "stdint.h"
#include "stdbool.h"
/*
    free rtos includes 
*/
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

/*
    tivaware includes 
*/
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/uart.h"
#include "utils.h"

// prototypes
void initTask(void *pvParameters);
void BTN1_CHCK_TASK(void *pvParameters);
void UART_TASK(void *pvParameters);


xQueueHandle xQueue;
SemaphoreHandle_t xSemaphore;
uint32_t counter = 0;


void initTask(void *pvParameters){
	
	//portf and push buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
		
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= 0x01;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0 | GPIO_PIN_4);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); 
	
	//port a
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
	GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	// uart clock
	SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_UART0)){}
		
	// uart config 
	UARTDisable(UART0_BASE);
		
	UARTClockSourceSet(UART0_BASE,UART_CLOCK_SYSTEM);
		
	UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 9600,
  (UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |
	UART_CONFIG_PAR_NONE));
	
	UARTEnable(UART0_BASE);
		
	vTaskDelete(NULL);
}

void BTN1_CHCK_TASK(void *pvParameters){
	for (;;){
		if(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0){
			do{
				delay(20);
			}while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 1);
			
			counter ++;
			xQueueSendToBack(xQueue, &counter, 0);
			xSemaphoreGive(xSemaphore);
			
			while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0){}
			do{
				delay(20);
			}while(GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0) == 0);		
		}
	}
}




void UART_TASK(void *pvParameters){
	uint32_t receivedValue;
	portBASE_TYPE xStatus;
	char buffer[11];
	
	for (;;){
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		
		xQueueReceive(xQueue, &receivedValue, 0);
		itoa(counter, buffer);
		uart_send_string(buffer);
	}
	
}

int main()
{    
	
	xQueue = xQueueCreate(5, sizeof(uint32_t));
	xSemaphore = xSemaphoreCreateCounting(5, 0);
	
	xTaskCreate(initTask, "init task", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
	xTaskCreate(BTN1_CHCK_TASK, "button1 task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
	xTaskCreate(UART_TASK, "uart task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);

  vTaskStartScheduler();
  while(true){
    
  }
  return 0;
}
