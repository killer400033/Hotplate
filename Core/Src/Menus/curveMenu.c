#include "../UIController.h"
#include "../displayDriver.h"
#include "../displayAPI.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>

#define THIS_MENU CURVE_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

uint8_t curveSelection = 0;

enum Menu runMenuCurve(uint8_t doInitialize) {
	if (doInitialize) {
		initialize();
	}

	if (updatePending && displayMode == STANDBY) {
		drawMenu();
		updatePending = FALSE;
	}
	return nextMenu;
}

void inputUpdateCurve(enum Input input) {
	if (input == BUTTON) {
		curveSelection++;
		if (curveSelection == CURVE_POINT_CNT) {
			curveSelection = 0;
			nextMenu = MAIN_MENU;
		}
		updatePending = TRUE;
	}
	else {
		tempCurve[curveSelection] += input * 2;
		if (tempCurve[curveSelection] < TEMP_MIN) tempCurve[curveSelection] = TEMP_MIN;
		if (tempCurve[curveSelection] > TEMP_MAX) tempCurve[curveSelection] = TEMP_MAX;
		updatePending = TRUE;
	}
}

static inline void initialize(void) {
	nextMenu = THIS_MENU;
}

static inline void drawMenu(void) {
	char str[5] = {0};
	clearDisplay();
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
	x_pos = 2.0 + (curveSelection * 21.0) / 2.0;
	if (curveSelection == CURVE_POINT_CNT - 1) x_pos-=2; // Adjustment to stop circle going off screen
	drawCircle(x_pos, 57 - tempCurve[curveSelection]/5, 2);

	// Temp readout
	sprintf(str, "%dC", tempCurve[curveSelection]);
	uint8_t str_x_pos = x_pos + 10;
	uint8_t rect_width = 3 + strlen(str) * 5;

	if (str_x_pos + rect_width > 127) {
		str_x_pos = x_pos - 10 - rect_width;
	}
	drawRectangleHollow(str_x_pos - 2, 27, rect_width, 9, 1);
	drawString(str_x_pos, 29, font5x6, str);

	writeToDisplay();
}
