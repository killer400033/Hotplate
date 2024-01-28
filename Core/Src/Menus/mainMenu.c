#include "../UIController.h"
#include "../displayDriver.h"
#include "../displayAPI.h"
#include "../logicControl.h"
#include <string.h>
#include <stdio.h>

#define THIS_MENU MAIN_MENU

static enum Menu nextMenu = THIS_MENU;
static inline void drawMenu(void);
static inline void initialize(void);

char *modeStr[3] = {"Standby", "Heating", "Cooling"};

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
	sprintf(str, "%dC", tempDelta);
	drawString(104 - 8*strlen(str), 25, font8x8, (char *)str);

	drawBitMap(3, 45, heat);
	sprintf(str, "%d%%", hotPlatePwr);
	drawString(23, 50, font8x8, (char *)str);

	drawBitMap(3, 20, temp);
	sprintf(str, "%dC", currTemp);
	drawString(23, 25, font8x8, (char *)str);

	drawString(3, 2, font8x8, (char *)modeStr[currMode]);

	writeToDisplay();
}
