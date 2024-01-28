#include "../UIController.h"
#include "../displayDriver.h"
#include "../displayAPI.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>

#define THIS_MENU CALIBRATION_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

uint8_t calibSelection = 0;
char *calibStr[3] = {"Ready", "Heating", "Cooling"};
int16_t inputTemp = 0;
uint16_t setTemps[3] = {50, 150, 250};

enum Menu runMenuCalibration(uint8_t doInitialize) {
	if (doInitialize) {
		initialize();
	}

	if (updatePending && displayMode == STANDBY) {
		drawMenu();
		updatePending = FALSE;
	}
	return nextMenu;
}

void inputUpdateCalibration(enum Input input) {
	if (input == BUTTON) {
		calibSelection++;
		if (calibSelection == 3) {
			nextMenu = MAIN_MENU;
		}
		else {
			inputTemp = setTemps[calibSelection];
			updatePending = TRUE;
		}
	}
	else {
		inputTemp += input;
		if (inputTemp < TEMP_MIN) inputTemp = TEMP_MIN;
		if (inputTemp > TEMP_MAX) inputTemp = TEMP_MAX;
		updatePending = TRUE;
	}
}

static inline void initialize(void) {
	nextMenu = THIS_MENU;
	calibSelection = 0;
	inputTemp = setTemps[calibSelection];
}

static inline void drawMenu(void) {
	char str[5] = {0};
	clearDisplay();

	drawString((DISPLAY_X - 8*strlen(calibStr[currMode])) / 2, 5, font8x8, (char *)calibStr[currMode]);

	drawString(4, 28, font8x8, " Set Temp:");
	drawString(4, 40, font8x8, "Meas Temp:");
	drawString(4, 52, font8x8, "Real Temp:");

	sprintf(str, "%dC", setTemps[calibSelection]);
	drawString(92, 28, font8x8, str);

	sprintf(str, "%dC", currTemp);
	drawString(92, 40, font8x8, str);

	sprintf(str, "%dC", inputTemp);
	drawRectangleHollow(90, 50, 3 + strlen(str) * 8, 11, 1);
	drawString(92, 52, font8x8, str);

	writeToDisplay();
}
