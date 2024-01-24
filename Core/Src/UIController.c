#include "displayDriver.h"
#include "displayAPI.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

uint8_t toggle = 0;
char str[10] = {0};

void mainScreen(void);

void begin(void) {
	initializeDisplay();
	drawBitMap(14, 0, logo);
	writeToDisplay();
	// Run fans test
	LL_mDelay(3000);
	clearDisplay();
	writeToDisplay();

	// Begin 1 second timer
	LL_TIM_EnableIT_UPDATE(TIM7);
	LL_TIM_EnableCounter(TIM7);

	// Draw main screen
	mainScreen();
}

void secondlyInterrupt(void) {
	if (toggle) {
		drawCircleHollow(123, 5, 4, 2);
	}
	else {
		drawCircle(123, 5, 2);
	}
	toggle = ~toggle;
	writeToDisplay();
}

void mainScreen(void) {
	drawBitMap(111, 45, fan);
	sprintf(str, "%d%%", 100);
	drawString(108 - 8*strlen(str), 50, font8x8, (char *)str, strlen(str));

	drawBitMap(111, 20, delta);
	sprintf(str, "%dC", 10);
	drawString(108 - 8*strlen(str), 25, font8x8, (char *)str, strlen(str));

	drawBitMap(7, 45, heat);
	sprintf(str, "%d%%", 100);
	drawString(27, 50, font8x8, (char *)str, strlen(str));

	drawBitMap(7, 20, temp);
	sprintf(str, "%dC", 100);
	drawString(27, 25, font8x8, (char *)str, strlen(str));

	sprintf(str, "Standby");
	drawString(7, 2, font8x8, (char *)str, strlen(str));

	writeToDisplay();
}
