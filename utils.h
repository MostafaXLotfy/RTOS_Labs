#pragma once
#include "stdint.h"


void itoa(uint32_t value, char* buf);
void uart_send_string(char* string);
void delay(uint32_t n);