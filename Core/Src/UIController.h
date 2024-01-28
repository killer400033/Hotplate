#include <stdint.h>

#define TRUE 1
#define FALSE 0
#define TEMP_MAX 270
#define TEMP_MIN 0

enum Menu {
	CALIBRATION_MENU,
	MAIN_MENU,
	SELECTION_MENU,
	CURVE_MENU,
};

enum Input {
	ENC_POS = 1,
	ENC_NEG = -1,
	BUTTON,
};

extern uint8_t updatePending;

void inputInterrupt(enum Input input);
void initializeUI(void);
void secondlyInterrupt(void);
void runUILoop(void);
