#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "display.h"
#include "system_common.h"
#include <stdio.h>

struct SemaGroup
{
	SemaphoreHandle_t xMutex1;
	SemaphoreHandle_t xMutex2;
};

struct SemaGroup xSems = {0};

void vDisplay (void *pvParams);
void vCounter (void *pvParams);

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
    TaskHandle_t TaskDisplay = NULL;
    BaseType_t xResult;

    prvHwInit();

    printf("FreeRTOS Hands-on Example Application.\n");

    xSems.xMutex1 = xSemaphoreCreateMutex();
    xSems.xMutex2 = xSemaphoreCreateMutex();
    configASSERT((xSems.xMutex1 != NULL) && (xSems.xMutex2 != NULL));

    xResult = xTaskCreate(vDisplay, "DisplayTask", configMINIMAL_STACK_SIZE, (void *)&xSems, 2, &TaskDisplay);
    configASSERT(xResult == pdPASS);

    vTaskStartScheduler();

    while (1)
    {

    }
}

void vDisplay (void *pvParams)
{
	TaskHandle_t TaskCounter = NULL;
	BaseType_t xResult;
	struct SemaGroup *pxSems = (struct SemaGroup *)pvParams;

	while(1)
	{
		xSemaphoreTake(pxSems->xMutex1, portMAX_DELAY);

		xResult = xTaskCreate(vCounter, "CounterTask", configMINIMAL_STACK_SIZE, (void *)pxSems, 5, &TaskCounter);
		configASSERT(xResult == pdPASS);

		xSemaphoreTake(pxSems->xMutex2, portMAX_DELAY);

		/* Just make a change in the display */
		vDisplaySetNum(8);

		xSemaphoreGive(pxSems->xMutex1);

		vTaskDelay(100);
	}
}

void vCounter (void *pvParams)
{
	struct SemaGroup *pxSems = (struct SemaGroup *)pvParams;

	while(1)
	{
		xSemaphoreTake(pxSems->xMutex2, portMAX_DELAY);

		xSemaphoreTake(pxSems->xMutex1, portMAX_DELAY);

		/* Just update the display so it can say it's alive */
		vDisplaySetNum(1);

		xSemaphoreGive(pxSems->xMutex2);

		vTaskDelay(100);
	}
}
