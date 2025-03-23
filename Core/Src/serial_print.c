#include "main.h"
#include "usart.h"
#include "gpio.h"

void serial_write (const char data)
{
    while (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TXE) == RESET);

    huart1.Instance->DR = data;
}

// This is std C; make sure to get fn name correctly
int __io_putchar(int ch)
{
	serial_write(ch);
	return ch;
}


