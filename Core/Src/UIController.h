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
	WRITE_PENDING_MENU,
	READ_PENDING_MENU,
};

enum Input {
	ENC_POS = 1,
	ENC_NEG = -1,
	BUTTON,
	TIME,
};

union FloatDecrypt {
	float f[2];
	uint8_t bytes[8];
};

extern uint8_t updatePending;

void inputInterrupt(enum Input input);
void initializeUI(void);
void UITimeIncrement(void);
void runUILoop(void);
