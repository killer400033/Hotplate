#include "../UIController.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>
#include "../Display/displayAPI.h"
#include "../Display/displayDriver.h"

#define THIS_MENU MAIN_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

char *modeStr[4] = {"Standby", "Working", "Cooling", "Init..."};

enum Menu runMenuMain(uint8_t doInitialize) {
	if (doInitialize) {
		initialize();
	}

	if (updatePending && displayMode == STANDBY) {
		drawMenu();
		updatePending = FALSE;
	}
	return nextMenu;
}

void inputUpdateMain(enum Input input) {
	if (input == BUTTON) {
		nextMenu = SELECTION_MENU;
	}
	else if (input == TIME) {
		updatePending = TRUE;
	}
}

static inline void initialize(void) {
	nextMenu = THIS_MENU;
}

static inline void drawMenu(void) {
	char str[6] = {0};
	clearDisplay();

	drawBitMap(107, 45, fan);
	sprintf(str, "%d%%", fanPwr);
	drawString(104 - 8*strlen(str), 50, font8x8, (char *)str);

	drawBitMap(107, 20, delta);
	sprintf(str, "%dC", (int16_t)-tempDelta);
	drawString(104 - 8*strlen(str), 25, font8x8, (char *)str);

	drawBitMap(3, 45, heat);
	sprintf(str, "%d%%", hotPlatePwr);
	drawString(23, 50, font8x8, (char *)str);

	drawBitMap(3, 20, temp);
	sprintf(str, "%dC", (int16_t)currTemp);
	drawString(23, 25, font8x8, (char *)str);

	drawString(3, 2, font8x8, (char *)modeStr[controlMode]);

	uint8_t seconds = (timeElapsed / 2) % 60;
	uint8_t minutes = (timeElapsed / (2 * 60)) % 100;
	if (timeElapsed % 2) {
		sprintf(str, "%02d %02d", minutes, seconds);
	}
	else {
		sprintf(str, "%02d:%02d", minutes, seconds);
	}

	drawString(126 - 8*strlen(str), 2, font8x8, (char *)str);

	writeToDisplay();
}
