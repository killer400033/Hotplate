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
char *calibStr[3] = {"Ready", "Working"};
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
			stopWorking();
		}
		else {
			inputTemp = setTemps[calibSelection];
			setManualTemp(inputTemp);
			updatePending = TRUE;
		}
	}
	else if (input == ENC_NEG || input == ENC_POS) {
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
	setManualTemp(inputTemp);
}

static inline void drawMenu(void) {
	char str[5] = {0};
	clearDisplay();

	if (tempDelta < ERROR_MARGIN && tempDelta > -ERROR_MARGIN) {
		drawString((DISPLAY_X - 8*strlen(calibStr[0])) / 2, 5, font8x8, (char *)calibStr[controlMode]);
	}
	else {
		drawString((DISPLAY_X - 8*strlen(calibStr[1])) / 2, 5, font8x8, (char *)calibStr[controlMode]);
	}

	drawString(4, 28, font8x8, " Set Temp:");
	drawString(4, 40, font8x8, "Meas Temp:");
	drawString(4, 52, font8x8, "Real Temp:");

	sprintf(str, "%dC", setTemps[calibSelection]);
	drawString(92, 28, font8x8, str);

	sprintf(str, "%dC", (int16_t)currTemp);
	drawString(92, 40, font8x8, str);

	sprintf(str, "%dC", inputTemp);
	drawRectangleHollow(90, 50, 3 + strlen(str) * 8, 11, 1);
	drawString(92, 52, font8x8, str);

	writeToDisplay();
}
