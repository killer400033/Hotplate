#include <stdint.h>

#define CURVE_POINT_CNT 13
#define COLD_TEMP 40
#define ERROR_MARGIN 5

#define CLK 170000000
#define PID_COUNTER 4250 // Determines the PID loop and temperature sensor polling frequency (temp sensor has min conversion time of ~0.2s)
#define PID_PRESCALER (9999 + 1)
#define TIMER_INTERRUPT_CNT CLK / (PID_COUNTER * PID_PRESCALER * 2)

enum Mode {
	PWR_STANDBY=0,
	WORKING=1,
	COOLING=2,
	INITIALIZING=3,
};

typedef struct SAVED_STATE {
	uint16_t temp;
	uint16_t cycleCnt;
} SAVED_STATE;

extern float currTemp;
extern uint16_t hotPlatePwr;
extern uint16_t fanPwr;
extern float tempDelta;
extern int16_t tempCurve[CURVE_POINT_CNT];
extern float tempCalib[2];
extern enum Mode controlMode;
extern SAVED_STATE savedState;
extern uint8_t pidLoopBusy;
extern uint16_t timeElapsed;

extern const float defaultTempCalib[2];
extern const int16_t defaultTempCurve[CURVE_POINT_CNT];

void runLogicLoop(void);
void startWorking(void);
void stopWorking(void);
void logicTimeIncrement(void);
void setManualTemp(uint16_t temp);
