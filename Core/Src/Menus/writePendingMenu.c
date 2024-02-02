#include "../UIController.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>
#include "../Display/displayAPI.h"
#include "../Display/displayDriver.h"
#include "../eeprom.h"
#include "main.h"

#define THIS_MENU WRITE_PENDING_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

uint8_t saveCurveData(void);

uint8_t successfulWrite;

enum Menu runMenuWrite(uint8_t doInitialize) {
	if (doInitialize) {
		initialize();
	}

	if (updatePending && displayMode == STANDBY) {
		drawMenu();
		updatePending = FALSE;
	}
	return nextMenu;
}

void inputUpdateWrite(enum Input input) {
	if (input == BUTTON) {
		nextMenu = MAIN_MENU;
	}
}

static inline void initialize(void) {
	nextMenu = THIS_MENU;
	successfulWrite = saveCurveData();
}

uint8_t saveCurveData(void) {
	uint8_t dataOut[CURVE_POINT_CNT * 2] = {0};
	TRANSFER_STATUS transferStatus;

	for (uint16_t i = 0; i < CURVE_POINT_CNT * 2; i+=2) {
		dataOut[i] = (tempCurve[i/2] & 0xFF00) >> 8;
		dataOut[i + 1] = tempCurve[i/2] & 0xFF;
	}


	while (!eepromWriteData(0x00, dataOut, CURVE_POINT_CNT * 2, &transferStatus)); // Wait for I2C to be free
	while (transferStatus == EEPROM_PENDING); // Wait for transfer to finish

	if (transferStatus == EEPROM_ERROR) return FALSE;

	LL_mDelay(5); // Delay for EEPROM write cycle

	while (!eepromReadData(0x00, dataOut, CURVE_POINT_CNT * 2, &transferStatus)); // Wait for I2C to be free
	while (transferStatus == EEPROM_PENDING); // Wait for transfer to finish

	if (transferStatus == EEPROM_ERROR) return FALSE;

	// Verify data
	for (uint16_t i = 0; i < CURVE_POINT_CNT * 2; i+=2) {
		uint16_t temp = dataOut[i];
		temp = temp << 8;
		temp |= dataOut[i + 1];
		if (temp != tempCurve[i/2]) {
			return FALSE;
		}
	}

	return TRUE;
}

static inline void drawMenu(void) {
	clearDisplay();

	if (successfulWrite) {
		drawString(16, 24, font8x8, "Successfully");
		drawString(32, 34, font8x8, "Saved!!!");
	}
	else {
		drawString(28, 24, font8x8, "Failed To");
		drawString(36, 34, font8x8, "Save...");
	}


	writeToDisplay();
}
