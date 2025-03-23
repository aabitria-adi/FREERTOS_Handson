#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "display.h"
#include "system_common.h"
#include <stdio.h>

static void task1_handler (void *parameters);
static void task2_handler (void *parameters);

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
	TaskHandle_t task1_handle, task2_handle;
	BaseType_t status;

	prvHwInit();

    printf("FreeRTOS Hands-on Example Application.\n");

    status = xTaskCreate(task1_handler, "Task-1", 200, "Hello world from Taks-1", 2, &task1_handle);
    configASSERT(status == pdPASS);

    status = xTaskCreate(task2_handler, "Task-2", 200, "Hello world from Taks-2", 2, &task2_handle);
    configASSERT(status == pdPASS);

    vTaskStartScheduler();

    while (1)
    {

    }
}

static void task1_handler (void *parameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		vTaskDelay(pdMS_TO_TICKS(500));
	}
}

static void task2_handler (void *parameters)
{
	while(1)
	{
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
		vTaskDelay(pdMS_TO_TICKS(750));
	}
}


