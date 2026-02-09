#include "main.h"
#include "usart.h"
#include "gpio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "display.h"
#include "system_common.h"
#include <stdio.h>
#include <stdint.h>

#define DWT_CTRL       (*(volatile uint32_t *)0xE0001000)

extern  void SEGGER_UART_init(uint32_t);

TaskHandle_t led_red_task_handle, led_green_task_handle;
TaskHandle_t next_task_handle;


static void led_red_handler (void *parameters);
static void led_green_handler (void *parameters);


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
//	TaskHandle_t xTask1, xTask2;
	BaseType_t xStatus;

	prvHwInit();

	//printf("FreeRTOS Hands-on Example Application.\n");

	DWT_CTRL |= (1 << 0);

	SEGGER_UART_init(576000);

	SEGGER_SYSVIEW_Conf();
	//SEGGER_SYSVIEW_Start();

	xStatus = xTaskCreate(led_green_handler, "LED_Green_Task", 200, NULL, 3, &led_green_task_handle);
	configASSERT(xStatus == pdPASS);

	next_task_handle = led_green_task_handle;

	xStatus = xTaskCreate(led_red_handler, "LED_Red_Task", 200, NULL, 2, &led_red_task_handle);
	configASSERT(xStatus == pdPASS);

	vTaskStartScheduler();

    while (1)
    {

    }
}

void button_interrupt_handler (void)
{
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

	traceISR_ENTER();
	xTaskNotifyFromISR(next_task_handle, 0, eNoAction, &pxHigherPriorityTaskWoken);

	/* Make the unblocked task of higher priority than the interrupted current task to run if true */
	/* If true, it calls portYIELD */
	portYIELD_FROM_ISR(pxHigherPriorityTaskWoken);
	traceISR_EXIT();
}

static void led_red_handler (void* parameters)
{
	BaseType_t status;

	while(1)
	{
		SEGGER_SYSVIEW_PrintfTarget("Toggling red LED");
		HAL_GPIO_TogglePin(RED_GPIO_Port, RED_Pin);
		status = xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(800));
		if(status == pdTRUE) {

			/*
			 * Critical section since next_task_handle is also
			 * shared with interrupt handler.
			 * Disable interrupt for a short time.  If longer is needed
			 * use mutex etc instead.
			 */
			portENTER_CRITICAL();
			next_task_handle = NULL;
			HAL_GPIO_WritePin(RED_GPIO_Port, RED_Pin, GPIO_PIN_SET);
			SEGGER_SYSVIEW_PrintfTarget("Delete red LED task");
			portEXIT_CRITICAL();
			vTaskDelete(NULL);
		}
	}
}

static void led_green_handler (void* parameters)
{
	BaseType_t status;

	while(1)
	{
		SEGGER_SYSVIEW_PrintfTarget("Toggling green LED");
		HAL_GPIO_TogglePin(GREEN_GPIO_Port, GREEN_Pin);
		status = xTaskNotifyWait(0, 0, NULL, pdMS_TO_TICKS(1000));
		if(status == pdTRUE){

			/*
			 * Critical section since next_task_handle is also
			 * shared with interrupt handler.
			 * Disable interrupt for a short time.  If longer is needed
			 * use mutex etc instead.
			 */
			portENTER_CRITICAL();
			next_task_handle = led_red_task_handle;
			HAL_GPIO_WritePin(GREEN_GPIO_Port, GREEN_Pin, GPIO_PIN_SET);
			SEGGER_SYSVIEW_PrintfTarget("Delete green LED task");
			portEXIT_CRITICAL();

			vTaskDelete(NULL);
		}
	}
}

//static void vTask1Handler (void *parameters)
//{
//	while(1)
//	{
//		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
//		vTaskDelay(pdMS_TO_TICKS(5));
//		//printf("%s\n", (char *)parameters);
//	}
//}
//
//static void vTask2Handler (void *parameters)
//{
//	while(1)
//	{
//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
//		vTaskDelay(pdMS_TO_TICKS(8));
//		//printf("%s\n", (char *)parameters);
//	}
//}

void vApplicationIdleHook( void )
{
	while(1);
}
