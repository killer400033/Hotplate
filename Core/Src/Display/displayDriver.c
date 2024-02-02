#include "displayDriver.h"

#include "main.h"

uint8_t displayBuffer[DISPLAY_Y][X_BYTES + 1] = {0};
enum DisplayMode displayMode = STANDBY;

// Function Declarations
void sendInstruction(uint8_t _rWrS, uint8_t* data, uint32_t n);

// Used for initial setup, is slow and wastes CPU time
void sendInstruction(uint8_t _rWrS, uint8_t* data, uint32_t n) {
	while (!LL_SPI_IsActiveFlag_TXE(SPI1));
	LL_SPI_TransmitData8(SPI1, 0xF8 | (_rWrS << 1));

	for (int i = 0; i < n; i++) {
		while (!LL_SPI_IsActiveFlag_TXE(SPI1));
		LL_SPI_TransmitData8(SPI1, data[i] & 0xF0);
		while (!LL_SPI_IsActiveFlag_TXE(SPI1));
		LL_SPI_TransmitData8(SPI1, (data[i] & 0x0F) << 4);
	}
}

void initializeDisplay(void) {
	LL_SPI_Enable(SPI1); // Enable SPI

	uint8_t data[16] = {0};
	data[0] = 0x20;

	LL_mDelay(1000); // Initial Delay

	sendInstruction(0b00, data, 1); // Set Function to Basic Instruction set
	LL_mDelay(1);
	sendInstruction(0b00, data, 1); // Set Function to Basic Instruction set
	LL_mDelay(1);

	data[0] = 0x0C;
	sendInstruction(0b00, data, 1); // Turn Display on
	LL_mDelay(1);

	data[0] = 0x01;
	sendInstruction(0b00, data, 1); // Clear Display
	LL_mDelay(20);

	data[0] = 0x06;
	sendInstruction(0b00, data, 1); // Set Entry Mode
	LL_mDelay(1);

	for (uint16_t i = 0; i < DISPLAY_Y; i++) {
		displayBuffer[i][0] = 0xFA;  // Set first instruction byte
	}

	writeToDisplay();
}

void writeToDisplay(void) {
	if (displayMode == STANDBY) {
		displayMode = INC_ADDRESS;
		LL_TIM_EnableIT_UPDATE(TIM6);
		LL_TIM_EnableCounter(TIM6);
	}
}
