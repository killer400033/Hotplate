#include "logicControl.h"

uint16_t currTemp = 0;
uint16_t hotPlatePwr = 0;
uint16_t fanPwr = 0;
int16_t tempDelta = 0;
int16_t tempCurve[CURVE_POINT_CNT] = {20, 50, 100, 150, 150, 150, 150, 200, 250, 250, 150, 100, 60};
enum Mode currMode = Standby;

void runLogicLoop(void) {

}
