#include "UIController.h"
#include "main.h"

#include "Display/displayAPI.h"
#include "Display/displayDriver.h"
#include "logicControl.h"
#include "Menus/mainMenu.h"
#include "Menus/calibrationMenu.h"
#include "Menus/selectionMenu.h"
#include "Menus/curveMenu.h"
#include "Menus/writePendingMenu.h"
#include "Menus/readPendingMenu.h"


enum Menu currMenu = READ_PENDING_MENU;
uint8_t updatePending = TRUE;
uint8_t doInitialize = TRUE;

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

	// Enable input interrupts
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_12);
	LL_EXTI_EnableIT_0_31(LL_EXTI_LINE_15);
}

void UITimeIncrement(void) {
	if (currMenu == MAIN_MENU) {
		enum Input input = TIME;
		inputUpdateMain(input);
	}
	else if (currMenu == CALIBRATION_MENU) {
		enum Input input = TIME;
		inputUpdateMain(input);
	}
}

void runUILoop(void) {
	enum Menu newMenu;
	switch (currMenu) {
	case MAIN_MENU:
		newMenu = runMenuMain(doInitialize);
		break;
	case CALIBRATION_MENU:
		newMenu = runMenuCalibration(doInitialize);
		break;
	case SELECTION_MENU:
		newMenu = runMenuSelection(doInitialize);
		break;
	case CURVE_MENU:
		newMenu = runMenuCurve(doInitialize);
		break;
	case WRITE_PENDING_MENU:
		newMenu = runMenuWrite(doInitialize);
		break;
	case READ_PENDING_MENU:
		newMenu = runMenuRead(doInitialize);
	default:
		break;
	}
	doInitialize = FALSE;
	if (newMenu != currMenu) {
		currMenu = newMenu;
		updatePending = TRUE;
		doInitialize = TRUE;
	}
}

void inputInterrupt(enum Input input) {
	switch (currMenu) {
	case MAIN_MENU:
		inputUpdateMain(input);
		break;
	case CALIBRATION_MENU:
		inputUpdateCalibration(input);
		break;
	case SELECTION_MENU:
		inputUpdateSelection(input);
		break;
	case CURVE_MENU:
		inputUpdateCurve(input);
		break;
	case WRITE_PENDING_MENU:
		inputUpdateWrite(input);
		break;
	case READ_PENDING_MENU:
		inputUpdateRead(input);
		break;
	default:
		break;
	}
}
