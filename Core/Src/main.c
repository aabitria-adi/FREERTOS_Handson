#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "display.h"
#include "system_common.h"
#include <stdio.h>

void vCounter (void *pvParam);
void vDisplay (void *pvParam);

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
    TaskHandle_t TaskCounter = NULL, TaskDisplay = NULL;
    QueueHandle_t QueueDisplay = NULL;
    BaseType_t xStat;

    prvHwInit();

    printf("FreeRTOS Hands-on Example Application.\n");

    QueueDisplay = xQueueCreate(1, sizeof(uint8_t));
    configASSERT(QueueDisplay != NULL);

    xStat = xTaskCreate(vCounter, "CounterTask", configMINIMAL_STACK_SIZE, (void *)QueueDisplay, 3, &TaskCounter);
    configASSERT(xStat == pdPASS);

    xStat = xTaskCreate(vDisplay, "DisplayTask", configMINIMAL_STACK_SIZE, (void *)QueueDisplay, 2, &TaskDisplay);
    configASSERT(xStat == pdPASS);

    vTaskStartScheduler();

    while (1)
    {

    }
}


void vCounter (void *pvParam)
{
	/* Producer */
	QueueHandle_t xQueueNumProd = (QueueHandle_t)pvParam;
    uint8_t ucCount = 0;

	while (1)
	{
		if (xQueueSend(xQueueNumProd, &ucCount, 0) == pdPASS)
		{
			ucCount++;

			ucCount = ucCount % 10;

			vTaskDelay(pdMS_TO_TICKS(1000));
		}
	}
}


void vDisplay (void *pvParam)
{
	/* Consumer */
	QueueHandle_t xQueueNumCon = (QueueHandle_t)pvParam;
	uint8_t ucNumber = 0;

	vDisplayClear();

	while (1)
	{
		(void)xQueueReceive(xQueueNumCon, &ucNumber, portMAX_DELAY);

		vDisplaySetNum(ucNumber);
	}
}
