#include "../UIController.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>
#include "../Display/displayAPI.h"
#include "../Display/displayDriver.h"

#define THIS_MENU SELECTION_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

int8_t menuSelection = 0;

enum Menu runMenuSelection(uint8_t doInitialize) {
	if (doInitialize) {
		initialize();
	}

	if (updatePending && displayMode == STANDBY) {
		drawMenu();
		updatePending = FALSE;
	}
	return nextMenu;
}

void inputUpdateSelection(enum Input input) {
	if (input == BUTTON) {
		switch(menuSelection) {
		case 0:
			nextMenu = MAIN_MENU;
			break;
		case 1:
			nextMenu = MAIN_MENU;
			if (controlMode == WORKING || controlMode == INITIALIZING) {
				stopWorking();
			}
			else {
				startWorking();
			}
			break;
		case 2:
			nextMenu = CURVE_MENU;
			break;
		case 3:
			nextMenu = CALIBRATION_MENU;
			break;
		default:
			break;
		}
	}
	else if (input == ENC_NEG || input == ENC_POS) {
		menuSelection += input;
		if (controlMode != WORKING && controlMode != INITIALIZING) {
			if (menuSelection < 0) menuSelection = 3;
			if (menuSelection > 3) menuSelection = 0;
		}
		else {
			if (menuSelection < 0) menuSelection = 1;
			if (menuSelection > 1) menuSelection = 0;
		}
		updatePending = TRUE;
	}
}

static inline void initialize(void) {
	nextMenu = THIS_MENU;
	menuSelection = 0;
}

static inline void drawMenu(void) {
	clearDisplay();

	drawString(8, 3, font8x8, "Selection Menu");

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

	drawRectangleHollow(7, 16, 50, 16, 2);
	drawString(17, 21, font8x8, "Home");

	drawRectangleHollow(7, 42, 50, 16, 2);
	drawString(13, 47, font8x8, "Curve");

	drawRectangleHollow(70, 16, 50, 16, 2);
	if (controlMode != WORKING && controlMode != INITIALIZING) {
		drawString(76, 21, font8x8, "Start");
	}
	else {
		drawString(76, 21, font8x8, "Abort");
	}

	drawRectangleHollow(70, 42, 50, 16, 2);
	drawString(76, 47, font8x8, "Calib");

	writeToDisplay();
}
