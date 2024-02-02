#include "../UIController.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>
#include "../Display/displayAPI.h"
#include "../Display/displayDriver.h"

#define THIS_MENU CALIBRATION_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

void findLineOfBestFit(void);

uint8_t calibSelection = 0;
char *calibStr[3] = {"Ready", "Working"};
float tempPoins[3][2] = {{50, 50}, {150, 150}, {250, 250}}; // 3 points where temp is calibrated (0 is x, 1 is y)
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
		tempPoins[calibSelection][0] = currTemp;
		calibSelection++;
		if (calibSelection == 3) {
			nextMenu = WRITE_PENDING_MENU;
			stopWorking();
			findLineOfBestFit();
		}
		else {
			setManualTemp(setTemps[calibSelection]);
			updatePending = TRUE;
		}
	}
	else if (input == ENC_NEG || input == ENC_POS) {
		tempPoins[calibSelection][1] += input;
		if (tempPoins[calibSelection][1] < TEMP_MIN) tempPoins[calibSelection][1] = TEMP_MIN;
		if (tempPoins[calibSelection][1] > TEMP_MAX) tempPoins[calibSelection][1] = TEMP_MAX;
		updatePending = TRUE;
	}
	else if (input == TIME) {
		updatePending = TRUE;
	}
}

void findLineOfBestFit(void) {
	// Method of Least Squares
	float sum_x = tempPoins[0][0] + tempPoins[1][0] + tempPoins[2][0];
	float sum_y = tempPoins[0][1] + tempPoins[1][1] + tempPoins[2][1];
	float sum_xy = (tempPoins[0][0]*tempPoins[0][1]) + (tempPoins[1][0]*tempPoins[1][1]) + (tempPoins[2][0]*tempPoins[2][1]);
	float sum_xx = (tempPoins[0][0]*tempPoins[0][0]) + (tempPoins[1][0]*tempPoins[1][0]) + (tempPoins[2][0]*tempPoins[2][0]);

	float _m = (3.0*sum_xy - sum_x*sum_y) / (3.0*sum_xx - sum_x*sum_x);
	float _b = (sum_y - _m*sum_x) / 3.0;

	tempCalib[0] = _m;
	tempCalib[1] = _b;
}

static inline void initialize(void) {
	nextMenu = THIS_MENU;
	calibSelection = 0;

	// Set default temp calibration
	memcpy(tempCalib, defaultTempCalib, 2 * sizeof(float));

	for (uint8_t i = 0; i < 3; i++) {
		tempPoins[i][1] = setTemps[i];
	}

	setManualTemp(setTemps[calibSelection]);
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

	sprintf(str, "%dC", (uint16_t)tempPoins[calibSelection][1]);
	drawRectangleHollow(90, 50, 3 + strlen(str) * 8, 11, 1);
	drawString(92, 52, font8x8, str);

	writeToDisplay();
}
