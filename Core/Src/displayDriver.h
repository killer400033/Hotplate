#include <stdint.h>

#define DISPLAY_X 128
#define DISPLAY_Y 64
#define X_BYTES DISPLAY_X / 4

enum DisplayMode {
	INC_ADDRESS,
	DISPLAYING,
};

extern uint8_t displayBuffer[DISPLAY_Y][X_BYTES + 1];
extern uint8_t incrementInstruction[4];
extern enum DisplayMode displayMode;

void initializeDisplay(void);
void writeToDisplay(void);
