#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "display.h"
#include "system_common.h"
#include <stdio.h>

void prvHwInit (void)
{
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
}

int main(void)
{

    prvHwInit();

    printf("FreeRTOS Hands-on Example Application.\n");

    while (1)
    {

    }
}
