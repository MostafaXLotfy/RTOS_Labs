#include "utils.h"
#include "stdbool.h"
#include "driverlib/uart.h"
#include "inc/hw_memmap.h"




void itoa(uint32_t value, char* buf){
	
	int i = 0;
	while(value != 0){
			buf[i] =  (char) ('0' + (value % 10));
			value /= 10;
			i++;
	}
	// reverse the string
	int start = 0, end = i - 1;
	char temp;
	while (start < end){
		temp = buf[start];
		buf[start] = buf[end];
		buf[end] = temp;
		start ++;
		end --;
	}
	
	if (i == 0){
		buf[i] = '0';
		i++;
	}
	buf[i] = '\0' ;
}

void delay(uint32_t n){
	for (uint32_t i = 0; i < n; i++){
		for (uint32_t j = 0; j < 3180; j++){
		}
	}
}

void uart_send_string(char* string){
			int i = 0;
			while (string[i] != '\0'){
				UARTCharPut(UART0_BASE,string[i]);
				i++;
			}
			 UARTCharPut(UART0_BASE,'\n');
			 UARTCharPut(UART0_BASE,'\r');
	}
