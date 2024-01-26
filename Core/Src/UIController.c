#include "displayDriver.h"
#include "displayAPI.h"
#include "main.h"
#include "logicControl.h"
#include <stdio.h>
#include <string.h>

uint8_t toggle = 0;
char str[8] = {0};
uint8_t menuSelection = 0;
uint8_t calibSelection = 12;
char *modeStr[3] = {"Standby", "Heating", "Cooling"};
char *calibStr[3] = {"Ready", "Heating", "Cooling"};

void updateMainScreen(void);
void updateSelectionScreen(void);
void updateCurveScreen(void);
void updateCalibrationScreen(void);

void initializeUI(void) {
	initializeDisplay();
	drawBitMap(10, 0, logo);
	writeToDisplay();
	// Run fans test
	LL_mDelay(3000);
	clearDisplay();
	writeToDisplay();

	// Begin 1 second timer
	LL_TIM_EnableIT_UPDATE(TIM7);
	LL_TIM_EnableCounter(TIM7);

	// Draw main screen
	updateCurveScreen();
}

void secondlyInterrupt(void) {

}

void updateMainScreen(void) {
	clearDisplay();

	drawBitMap(107, 45, fan);
	sprintf(str, "%d%%", fanPwr);
	drawString(104 - 8*strlen(str), 50, font8x8, (char *)str);

	drawBitMap(107, 20, delta);
	sprintf(str, "%dC", tempDelta);
	drawString(104 - 8*strlen(str), 25, font8x8, (char *)str);

	drawBitMap(3, 45, heat);
	sprintf(str, "%d%%", hotPlatePwr);
	drawString(23, 50, font8x8, (char *)str);

	drawBitMap(3, 20, temp);
	sprintf(str, "%dC", currTemp);
	drawString(23, 25, font8x8, (char *)str);

	drawString(3, 2, font8x8, (char *)modeStr[currMode]);

	if (toggle) {
		drawCircleHollow(123, 5, 4, 2);
	}
	else {
		drawCircle(123, 5, 2);
	}
	toggle = ~toggle;

	writeToDisplay();
}

void updateSelectionScreen(void) {
	clearDisplay();

	drawString(8, 3, font8x8, "Selection Menu");

	drawRectangleHollow(7, 16, 50, 16, 2);
	drawString(13, 21, font8x8, "Start");

	drawRectangleHollow(7, 42, 50, 16, 2);
	drawString(13, 47, font8x8, "Curve");

	drawRectangleHollow(70, 16, 50, 16, 2);
	drawString(80, 21, font8x8, "Home");

	drawRectangleHollow(70, 42, 50, 16, 2);
	drawString(76, 47, font8x8, "Calib");

	switch(menuSelection) {
	case 0:
		drawRectangleHollow(5, 14, 54, 20, 1);
		break;
	case 1:
		drawRectangleHollow(68, 14, 54, 20, 1);
		break;
	case 2:
		drawRectangleHollow(5, 40, 54, 20, 1);
		break;
	case 3:
		drawRectangleHollow(68, 40, 54, 20, 1);
		break;
	default:
		break;
	}

	writeToDisplay();
}

void updateCurveScreen(void) {
	// Draw Graph axis
	drawRectangle(1, 0, 1, 56);
	drawBitMap(1, 56, x_axis);

	// Graph line and nodes
	float x_pos;
	for (uint16_t i = 0; i < CURVE_POINT_CNT - 1; i++) {
		x_pos = 2.0 + (i * 21.0) / 2.0;
		drawAngledLine(x_pos, 57 - tempCurve[i]/5, x_pos + 10.5, 57 - tempCurve[i + 1]/5);
		drawCircleHollow(x_pos, 57 - tempCurve[i]/5, 2, 1);
	}
	drawCircleHollow(126, 57 - tempCurve[12]/5, 2, 1);

	// Selected Node
	x_pos = 2.0 + (calibSelection * 21.0) / 2.0;
	if (calibSelection == CURVE_POINT_CNT - 1) x_pos-=2; // Adjustment to stop circle going off screen
	drawCircle(x_pos, 57 - tempCurve[calibSelection]/5, 2);

	// Temp readout
	sprintf(str, "%dC", 100);
	uint8_t str_x_pos = x_pos + 10;
	uint8_t rect_width = 3 + strlen(str) * 5;

	if (str_x_pos + rect_width > 127) {
		str_x_pos = x_pos - 10 - rect_width;
	}
	drawRectangleHollow(str_x_pos - 2, 27, rect_width, 9, 1);
	drawString(str_x_pos, 29, font5x6, str);
}

void updateCalibrationScreen(void) {
	clearDisplay();

	drawString((DISPLAY_X - 8*strlen(calibStr[currMode])) / 2, 5, font8x8, (char *)calibStr[currMode]);

	drawString(4, 28, font8x8, " Set Temp:");
	drawString(4, 40, font8x8, "Meas Temp:");
	drawString(4, 52, font8x8, "Real Temp:");

	sprintf(str, "%dC", 100);
	drawString(92, 28, font8x8, str);

	sprintf(str, "%dC", 100);
	drawString(92, 40, font8x8, str);

	sprintf(str, "%dC", 100);
	drawString(92, 52, font8x8, str);
	drawRectangleHollow(90, 50, 3 + strlen(str) * 8, 11, 1);

	writeToDisplay();
}
