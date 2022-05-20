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
#include "TM4C123GH6PM.h"
#include "driverlib/sysctl.h"
#include "inc/hw_gpio.h"
#include "inc/hw_types.h"
#include "driverlib/uart.h"
#include "inc/hw_sysctl.h"
#include "utils.h"

// prototypes
void initTask(void *pvParameters);
void CounterTask(void *pvParameters);
void LedTogglerTask(void *pvParameters);
void push_button_isr(void);

SemaphoreHandle_t xSemaphore;
SemaphoreHandle_t xMutex;


void push_button_isr(void){
	portBASE_TYPE higher_priority_woken = pdFALSE;
	xSemaphoreGiveFromISR(xSemaphore, &higher_priority_woken);
	GPIOIntClear(GPIO_PORTF_BASE,GPIO_INT_PIN_0);
	portEND_SWITCHING_ISR(higher_priority_woken);
}

void CounterTask(void *pvParameters){
	char buffer[3];
	for(;;){
		xSemaphoreTake(xMutex, portMAX_DELAY);
		uint32_t counter = 0;
		uart_send_string("This is the Counter task\0");
		while(counter < 10){
			counter++;
			itoa(counter, buffer);
			uart_send_string(buffer);
		}
		xSemaphoreGive(xMutex);
	}
}


void LedTogglerTask(void *pvParameters){
	char buffer[3];
	xSemaphoreTake(xSemaphore, 0);
	for(;;){
		
		xSemaphoreTake(xSemaphore, portMAX_DELAY);
		xSemaphoreTake(xMutex, portMAX_DELAY);
		uint32_t counter = 0;
		uart_send_string("This is LedToggler Task");
		while(counter < 10){
			counter++;
			itoa(counter, buffer);
			uart_send_string(buffer);
		}
		GPIO_PORTF_DATA_R ^= 0x2;
		xSemaphoreGive(xMutex);
		vTaskDelay(500 / portTICK_RATE_MS);
	}
	
}

void initTask(void *pvParameters){
	
	//portf and push buttons
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
		
	HWREG(GPIO_PORTF_BASE+GPIO_O_LOCK) = GPIO_LOCK_KEY;
	HWREG(GPIO_PORTF_BASE+GPIO_O_CR) |= 0x01;
	GPIOPinTypeGPIOInput(GPIO_PORTF_BASE,GPIO_PIN_0);
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE,GPIO_PIN_1);
	
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU); 
	GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0, GPIO_FALLING_EDGE);
	GPIOIntRegister(GPIO_PORTF_BASE,push_button_isr);
	NVIC_PRI7_R = 6 << 21;
	GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0);
	
		
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






int main()
{    
	
	xSemaphore = xSemaphoreCreateBinary();
	xMutex = xSemaphoreCreateMutex();
	
	if(xSemaphore != NULL || xMutex != NULL){
		xTaskCreate(initTask, "init task", configMINIMAL_STACK_SIZE, NULL, 3, NULL);
		xTaskCreate(LedTogglerTask, "led toggler task", configMINIMAL_STACK_SIZE, NULL, 2, NULL);
		xTaskCreate(CounterTask, "counter task", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
		 vTaskStartScheduler();
	}


  while(true){
    
  }
  return 0;
}
