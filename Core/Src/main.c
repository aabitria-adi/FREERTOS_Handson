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

#define NUM_TASKS      (3)

static SemaphoreHandle_t xBinSem;
static SemaphoreHandle_t xDoneSem;
static SemaphoreHandle_t xCS[NUM_TASKS];

TaskHandle_t xTask[NUM_TASKS];

static void vDine (void *parameters);
//static void vTask2Handler (void *parameters);

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
	char ucTaskName[20];

	prvHwInit();

	printf("FreeRTOS Hands-on Example Application.\n");

	DWT_CTRL |= (1 << 0);

	SEGGER_SYSVIEW_Conf();
	SEGGER_SYSVIEW_Start();

	xBinSem = xSemaphoreCreateBinary();
	xDoneSem = xSemaphoreCreateBinary();

	for (int i = 0; i < NUM_TASKS; i++)
	{
		xCS[i] = xSemaphoreCreateBinary();

		/* sema's must be given first after creation before it can be taken */
		xSemaphoreGive(xCS[i]);
	}

	for (int i = 0; i < NUM_TASKS; i++)
	{
		sprintf(ucTaskName, "Task%i", i);

		xTaskCreate(vDine, ucTaskName, 256, (void *)i, 2, &xTask[i]);
	}

	vTaskStartScheduler();

    while (1)
    {

    }
}

static void vDine (void *parameters)
{
	int lNum = (int)parameters;

	while(1)
	{
		/* Take left chopstick */
		xSemaphoreTake(xCS[lNum], portMAX_DELAY);

		/* Do work here */
		vDisplaySetNum(lNum);

		vTaskDelay(10*portTICK_PERIOD_MS);

		/* Take right chopstick */
		xSemaphoreTake(xCS[(lNum + 1) % NUM_TASKS], portMAX_DELAY);

		/* Do work here */
		vDisplaySetNum(lNum + NUM_TASKS);

		vTaskDelay(10*portTICK_PERIOD_MS);

		xSemaphoreGive(xCS[lNum]);

		xSemaphoreGive(xCS[(lNum + 1) % NUM_TASKS]);
	}
}

