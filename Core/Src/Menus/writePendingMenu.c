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

uint8_t writeNewData(void);
uint8_t transferAndRead(uint16_t address, uint8_t *data, uint16_t n);

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
	successfulWrite = writeNewData();
}

uint8_t writeNewData(void) {
	uint8_t dataOut[CURVE_POINT_CNT * 2] = {0};

	// Write Curve Data
	for (uint16_t i = 0; i < CURVE_POINT_CNT * 2; i+=2) {
		dataOut[i] = (tempCurve[i/2] & 0xFF00) >> 8;
		dataOut[i + 1] = tempCurve[i/2] & 0xFF;
	}

	if (!transferAndRead(CURVE_ADDRESS, dataOut, CURVE_POINT_CNT * 2)) return FALSE;

	// Verify data
	for (uint16_t i = 0; i < CURVE_POINT_CNT * 2; i+=2) {
		uint16_t temp = dataOut[i];
		temp = temp << 8;
		temp |= dataOut[i + 1];
		if (temp != tempCurve[i/2]) {
			return FALSE;
		}
	}

	// Write Calibration Data
	union FloatDecrypt floatDecrypt;
	floatDecrypt.f[0] = tempCalib[0];
	floatDecrypt.f[1] = tempCalib[1];
	memcpy(dataOut, floatDecrypt.bytes, 2 * sizeof(float));

	if (!transferAndRead(CALIB_ADDRESS, dataOut, 2 * sizeof(float))) return FALSE;

	for (uint8_t i = 0; i < 2 * sizeof(float); i++) {
		if (floatDecrypt.bytes[i] != dataOut[i]) {
			return FALSE;
		}
	}

	return TRUE;
}

uint8_t transferAndRead(uint16_t address, uint8_t *data, uint16_t n) {
	TRANSFER_STATUS transferStatus;

	while (!eepromWriteData(address, data, n, &transferStatus)); // Wait for I2C to be free
	while (transferStatus == EEPROM_PENDING); // Wait for transfer to finish
	if (transferStatus == EEPROM_ERROR) return FALSE;

	LL_mDelay(5); // Delay for EEPROM write cycle

	while (!eepromReadData(address, data, n, &transferStatus)); // Wait for I2C to be free
	while (transferStatus == EEPROM_PENDING); // Wait for transfer to finish
	if (transferStatus == EEPROM_ERROR) return FALSE;

	return TRUE;
}

static inline void drawMenu(void) {
	clearDisplay();

	if (successfulWrite) {
		drawString(48, 18, font8x8, "Data");
		drawString(16, 28, font8x8, "Successfully");
		drawString(32, 38, font8x8, "Saved!!!");
	}
	else {
		drawString(28, 24, font8x8, "Failed To");
		drawString(16, 34, font8x8, "Save Data...");
	}


	writeToDisplay();
}
