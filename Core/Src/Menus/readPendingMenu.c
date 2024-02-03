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

uint8_t readStoredData(void);

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
	successfulRead = readStoredData();
}

uint8_t readStoredData(void) {
	// Set default values incase of EEPROM failure
	memcpy(tempCalib, defaultTempCalib, 2 * sizeof(float));
	memcpy(tempCurve, defaultTempCurve, CURVE_POINT_CNT * 2);

	uint8_t dataOut[CURVE_POINT_CNT * 2] = {0};
	TRANSFER_STATUS transferStatus;

	// Curve Data
	while (!eepromReadData(CURVE_ADDRESS, dataOut, CURVE_POINT_CNT * 2, &transferStatus)); // Wait for I2C to be free
	while (transferStatus == EEPROM_PENDING); // Wait for transfer to finish
	if (transferStatus == EEPROM_ERROR) return FALSE;

	for (uint16_t i = 0; i < CURVE_POINT_CNT * 2; i+=2) {
		uint16_t temp = dataOut[i];
		temp = temp << 8;
		temp |= dataOut[i + 1];
		tempCurve[i/2] = temp;
	}

	// Calibration Data
	union FloatDecrypt floatDecrypt;

	while (!eepromReadData(CALIB_ADDRESS, dataOut, 2 * sizeof(float), &transferStatus)); // Wait for I2C to be free
	while (transferStatus == EEPROM_PENDING); // Wait for transfer to finish
	if (transferStatus == EEPROM_ERROR) return FALSE;

	memcpy(floatDecrypt.bytes, dataOut, 2 * sizeof(float));
	tempCalib[0] = floatDecrypt.f[0];
	tempCalib[1] = floatDecrypt.f[1];

	return TRUE;
}

static inline void drawMenu(void) {
	clearDisplay();

	if (successfulRead) {
		drawString(48, 18, font8x8, "Data");
		drawString(16, 28, font8x8, "Successfully");
		drawString(36, 38, font8x8, "Read!!!");
	}
	else {
		drawString(28, 24, font8x8, "Failed To");
		drawString(16, 34, font8x8, "Read Data...");
	}


	writeToDisplay();
}
