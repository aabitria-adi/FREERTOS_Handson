#include <stdint.h>
#include "main.h"
#include "display.h"


void vDisplaySetNum (uint8_t ucNum)
{
	uint8_t ucTemp;

	switch (ucNum)
	{
	case 0:
		ucTemp = 0x3F;
		break;

	case 1:
		ucTemp = 0x06;
		break;

	case 2:
		ucTemp = 0x5B;
		break;

	case 3:
		ucTemp = 0x4F;
		break;

	case 4:
		ucTemp = 0x66;
		break;

	case 5:
		ucTemp = 0x6D;
		break;

	case 6:
		ucTemp = 0x7D;
		break;

	case 7:
		ucTemp = 0x07;
		break;

	case 8:
		ucTemp = 0x7F;
		break;

	case 9:
		ucTemp = 0x6F;
		break;

	default:
		ucTemp = 0x00;
		break;
	}

	/* Compensate for the display being common anode */
	ucTemp = (~ucTemp) & 0xFF;
	GPIOA->ODR = (uint32_t)ucTemp;

	return;
}

void vDisplayClear (void)
{
	GPIOA->ODR = 0x3F;

	return;
}
