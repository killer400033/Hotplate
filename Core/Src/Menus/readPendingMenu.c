#include "../UIController.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>
#include "../Display/displayAPI.h"
#include "../Display/displayDriver.h"
#include "../eeprom.h"
#include "main.h"

#define THIS_MENU READ_PENDING_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

uint8_t readCurveData(void);

uint8_t successfulRead;

enum Menu runMenuRead(uint8_t doInitialize) {
	if (doInitialize) {
		initialize();
	}

	if (updatePending && displayMode == STANDBY) {
		drawMenu();
		updatePending = FALSE;
	}
	return nextMenu;
}

void inputUpdateRead(enum Input input) {
	if (input == BUTTON) {
		nextMenu = MAIN_MENU;
	}
}

static inline void initialize(void) {
	nextMenu = THIS_MENU;
	successfulRead = readCurveData();
}

uint8_t readCurveData(void) {
	uint8_t dataOut[CURVE_POINT_CNT * 2] = {0};
	TRANSFER_STATUS transferStatus;

	while (!eepromReadData(0x00, dataOut, CURVE_POINT_CNT * 2, &transferStatus)); // Wait for I2C to be free
	while (transferStatus == EEPROM_PENDING); // Wait for transfer to finish

	if (transferStatus == EEPROM_ERROR) return FALSE;

	// Write Data
	for (uint16_t i = 0; i < CURVE_POINT_CNT * 2; i+=2) {
		uint16_t temp = dataOut[i];
		temp = temp << 8;
		temp |= dataOut[i + 1];
		tempCurve[i/2] = temp;
	}

	return TRUE;
}

static inline void drawMenu(void) {
	clearDisplay();

	if (successfulRead) {
		drawString(16, 24, font8x8, "Successfully");
		drawString(36, 34, font8x8, "Read!!!");
	}
	else {
		drawString(28, 24, font8x8, "Failed To");
		drawString(36, 34, font8x8, "Read...");
	}


	writeToDisplay();
}
