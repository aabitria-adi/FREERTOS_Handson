#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "display.h"
#include "system_common.h"
#include <stdio.h>


TaskHandle_t xDisplayCountTask = NULL;

void vDisplayCount (void *pvParam);

void prvHwInit (void)
{
    HAL_Init();

    /* Configure the system clock */
    SystemClock_Config();

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_USART1_UART_Init();
    TIM3_Init();
}

int main(void)
{
    BaseType_t xStatus;

    prvHwInit();

    printf("FreeRTOS Hands-on Example Application.\n");

    xStatus = xTaskCreate(vDisplayCount, "Display/Counter Task", 256, NULL, 5, &xDisplayCountTask);
    configASSERT(xStatus);

    vTaskStartScheduler();

    while (1)
    {

    }
}

void vDisplayCount (void *pvParam)
{
	uint8_t ucCnt = 0;
	uint32_t ulNotification;

	TIM3_Start();

	vDisplayClear();

	while(1)
	{
        xTaskNotifyWait(0x00, 0xFFFFFFFF, &ulNotification, portMAX_DELAY);
		vDisplaySetNum(ucCnt);
		ucCnt = (ucCnt + 1) % 10;

	}
}


/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    if (htim->Instance == TIM4)
    {
        HAL_IncTick();
    }

    if (htim->Instance == TIM3)
    {
	    // Send notify
    	xTaskNotifyFromISR(xDisplayCountTask, 2, eNoAction, &xHigherPriorityTaskWoken);
    }
}

