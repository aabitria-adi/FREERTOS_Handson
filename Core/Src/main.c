#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "display.h"
#include "system_common.h"
#include <stdio.h>

#define DWT_CTRL       (*(volatile uint32_t *)0xE0001000)

static void vTask1Handler (void *parameters);
static void vTask2Handler (void *parameters);

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
	TaskHandle_t xTask1, xTask2;
	BaseType_t xStatus;

	prvHwInit();

	printf("FreeRTOS Hands-on Example Application.\n");

	DWT_CTRL |= (1 << 0);

	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	xStatus = xTaskCreate(vTask1Handler, "Task-1", 200, "Hello world from Taks-1", 2, &xTask1);
	configASSERT(xStatus == pdPASS);

	xStatus = xTaskCreate(vTask2Handler, "Task-2", 200, "Hello world from Task-2", 2, &xTask2);
	configASSERT(xStatus == pdPASS);

	vTaskStartScheduler();

    while (1)
    {

    }
}

static void vTask1Handler (void *parameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		vTaskDelay(pdMS_TO_TICKS(5));
		//printf("%s\n", (char *)parameters);
	}
}

static void vTask2Handler (void *parameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
		vTaskDelay(pdMS_TO_TICKS(8));
		//printf("%s\n", (char *)parameters);
	}
}

void vApplicationIdleHook( void )
{
	while(1);
}
