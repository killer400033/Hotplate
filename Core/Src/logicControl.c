#include "logicControl.h"
#include "UIController.h"

enum TEMP_MODE {
	CURVE,
	MANUAL
};

typedef struct PIDState {
	float error;
	float integral;
} PIDState;

// Public Variables
float currTemp = 0;
uint16_t hotPlatePwr = 0;
uint16_t fanPwr = 0;
float tempDelta = 0;
enum Mode controlMode = PWR_STANDBY;

uint16_t timeElapsed = 0; // Each increment is 0.5 seconds

int16_t tempCurve[CURVE_POINT_CNT] = {0};
float tempCalib[2] = {0}; // Line of best fit for temp calibration (m, b)

SAVED_STATE savedState = {.temp = 0, .cycleCnt = 0};
uint8_t pidLoopBusy = FALSE; // Indicates whether there is data to be processed by PID loop

const int16_t defaultTempCurve[CURVE_POINT_CNT] = {20, 50, 100, 150, 150, 150, 150, 200, 250, 250, 150, 100, 60};
const float defaultTempCalib[2] = {1.0, 0.0};

// Private Variables
float manualSetTemp = 0;
enum TEMP_MODE tempSelectMode = CURVE;

PIDState prev_state = {.error = 0, .integral = 0};
float _P = 1;
float _I = 0;
float _D = 0;

void updateOutputs(float input);

float calculatePID(PIDState *prev_state, float curr_error, float time);
float doPIDLoop(void);
float doStandby(void);
float doCooling(void);

void setManualTemp(uint16_t temp) {
	tempSelectMode = MANUAL;
	manualSetTemp = temp;
	controlMode = WORKING;
}

void startWorking(void) {
	prev_state.error = 0;
	prev_state.integral = 0;
	tempSelectMode = CURVE;
	controlMode = INITIALIZING;
	timeElapsed = 0;
}

void stopWorking(void) {
	tempSelectMode = CURVE;
	controlMode = COOLING;
	timeElapsed = 0;
}

void logicTimeIncrement(void) {
	timeElapsed++;
	if (controlMode != WORKING) {
		timeElapsed %= 2;
	}
}

void runLogicLoop(void) {
	float output;

	if (pidLoopBusy) {
		currTemp = tempCalib[1] + tempCalib[0]*((float)(savedState.temp >> 3) / 4.0);

		switch (controlMode) {
		case INITIALIZING:
			output = doPIDLoop();
			break;
		case WORKING:
			output = doPIDLoop();
			break;
		case COOLING:
			output = doCooling();
			break;
		case PWR_STANDBY:
			output = doStandby();
			break;
		}
		pidLoopBusy = FALSE;

		updateOutputs(output);
	}
}

void updateOutputs(float input) {
	if (input > 0) {
		fanPwr = 0;
		hotPlatePwr = input * 100.0;
	}
	else if (input < 0) {
		hotPlatePwr = 0;
		fanPwr = -input * 100.0;
	}
	else {
		hotPlatePwr = 0;
		fanPwr = 0;
	}
}

float doPIDLoop(void) {
	float setTemp;
	float time;
	if (tempSelectMode == CURVE) {
		uint8_t index = timeElapsed / (2.0 * 30.0);

		if (index >= CURVE_POINT_CNT - 1) {
			stopWorking();
			return 0;
		}

		float ratio = (((float)timeElapsed / 2.0) - (30 * index)) / 30.0;
		setTemp = (float)tempCurve[index + 1] * ratio + (float)tempCurve[index] * (1 - ratio);
		tempDelta = setTemp - currTemp;
		if (tempDelta < ERROR_MARGIN && tempDelta > -ERROR_MARGIN) {
			controlMode = WORKING;
		}
	}
	else if (tempSelectMode == MANUAL) {
		setTemp = manualSetTemp;
		tempDelta = setTemp - currTemp;
	}

	time = (float)(savedState.cycleCnt * PID_COUNTER * PID_PRESCALER) / (float)CLK;
	return calculatePID(&prev_state, tempDelta, time);
}

float calculatePID(PIDState *prev_state, float curr_error, float time) {
	float proportional = _P * curr_error;
	float integral = prev_state->integral + _I*time*0.5*(curr_error + prev_state->error);
	float derivative = _D * (curr_error - prev_state->error) / time;
	float output = proportional + integral + derivative;

	prev_state->integral = integral;
	prev_state->error = curr_error;

	if (output > 1.0) output = 1.0;
	if (output < -1.0) output = -1.0;

	return output;
}

float doCooling(void) {
	tempDelta = 0;
	if (currTemp > COLD_TEMP - ERROR_MARGIN) {
		return -1;
	}
	controlMode = PWR_STANDBY;
	return 0;
}

float doStandby(void) {
	tempDelta = 0;
	if (currTemp < COLD_TEMP + ERROR_MARGIN) {
		return 0;
	}
	controlMode = COOLING;
	return -1;
}

