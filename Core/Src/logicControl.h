#include <stdint.h>

#define CURVE_POINT_CNT 13

enum Mode {
	Standby=0,
	Heating=1,
	Cooling=2,
};

extern uint16_t currTemp;
extern uint16_t hotPlatePwr;
extern uint16_t fanPwr;
extern int16_t tempDelta;
extern int16_t tempCurve[CURVE_POINT_CNT];
extern enum Mode currMode;

void runLogicLoop(void);
