#include "eeprom.h"
#include "main.h"
#include <stdio.h>

enum TRANSFER_MODE {
	WRITE,
	READ_1,
	READ_2,
};

enum TRANSFER_MODE transferMode;
uint16_t transferCnt = 0;
uint8_t dataBuffer[DATA_BUFFER_SIZE] = {0};
uint8_t *receiveBuffer;
uint16_t receiveBufferSize = 0;

TRANSFER_STATUS *currFinishFlag = NULL;

void eepromInitialize(void) {
	LL_I2C_Enable(I2C3);

	LL_I2C_DisableAutoEndMode(I2C3);

	// Enable Interrupts
  LL_I2C_EnableIT_ERR(I2C3);
  LL_I2C_EnableIT_NACK(I2C3);
	LL_I2C_EnableIT_TC(I2C3);
}

uint8_t eepromWriteData(uint16_t address, uint8_t *data, uint16_t n, TRANSFER_STATUS *finishFlag) {
	if (LL_I2C_IsActiveFlag_BUSY(I2C3) || currFinishFlag != NULL) return 0;

	dataBuffer[1] = address & 0xFF;
	dataBuffer[0] = (address & 0xFF00) >> 8;
	for (uint16_t i = 0; i < n; i++) {
		dataBuffer[i + 2] = data[i];
	}
	currFinishFlag = finishFlag;
	*currFinishFlag = EEPROM_PENDING;

	transferMode = WRITE;

	LL_I2C_EnableIT_TX(I2C3);
	LL_I2C_DisableIT_RX(I2C3);

  LL_I2C_SetMasterAddressingMode(I2C3, LL_I2C_ADDRESSING_MODE_7BIT);
  LL_I2C_SetSlaveAddr(I2C3, EEPROM_SLAVE_ADD);
  LL_I2C_SetTransferRequest(I2C3, LL_I2C_REQUEST_WRITE);
  LL_I2C_SetTransferSize(I2C3, n + 2);

  transferCnt = 0;

  LL_I2C_GenerateStartCondition(I2C3);

  return 1;
}

uint8_t eepromReadData(uint16_t address, uint8_t *data, uint16_t n, TRANSFER_STATUS *finishFlag) {
	if (LL_I2C_IsActiveFlag_BUSY(I2C3) || currFinishFlag != NULL) return 0;

	dataBuffer[1] = address & 0xFF;
	dataBuffer[0] = (address & 0xFF00) >> 8;
	receiveBufferSize = n;
	receiveBuffer = data;
	currFinishFlag = finishFlag;
	*currFinishFlag = EEPROM_PENDING;

	transferMode = READ_1;

	LL_I2C_EnableIT_TX(I2C3);
	LL_I2C_DisableIT_RX(I2C3);

  LL_I2C_SetMasterAddressingMode(I2C3, LL_I2C_ADDRESSING_MODE_7BIT);
  LL_I2C_SetSlaveAddr(I2C3, EEPROM_SLAVE_ADD);
  LL_I2C_SetTransferRequest(I2C3, LL_I2C_REQUEST_WRITE);
  LL_I2C_SetTransferSize(I2C3, 2);

	transferCnt = 0;

	LL_I2C_GenerateStartCondition(I2C3);

	return 1;
}


void eepromInterruptHandler(void) {
	if (LL_I2C_IsActiveFlag_TC(I2C3)) {
		switch (transferMode) {
		case WRITE:
			*currFinishFlag = EEPROM_FINISHED;
			currFinishFlag = NULL;
			LL_I2C_GenerateStopCondition(I2C3);
			break;
		case READ_1:
			transferMode = READ_2;

			LL_I2C_DisableIT_TX(I2C3);
			LL_I2C_EnableIT_RX(I2C3);

		  LL_I2C_SetTransferRequest(I2C3, LL_I2C_REQUEST_READ);
		  LL_I2C_SetTransferSize(I2C3, receiveBufferSize);

		  transferCnt = 0;
			LL_I2C_GenerateStartCondition(I2C3);
			break;
		case READ_2:
			*currFinishFlag = EEPROM_FINISHED;
			currFinishFlag = NULL;
			LL_I2C_GenerateStopCondition(I2C3);
			break;
		}
	}

	if (LL_I2C_IsActiveFlag_TXIS(I2C3)) {
		LL_I2C_TransmitData8(I2C3, dataBuffer[transferCnt]);
		transferCnt++;
	}

	if (LL_I2C_IsActiveFlag_RXNE(I2C3)) {
		receiveBuffer[transferCnt] = LL_I2C_ReceiveData8(I2C3);
		transferCnt++;
	}

	if (LL_I2C_IsActiveFlag_NACK(I2C3)) {
		*currFinishFlag = EEPROM_ERROR;
		currFinishFlag = NULL;
		LL_I2C_GenerateStopCondition(I2C3);
		LL_I2C_ClearFlag_NACK(I2C3);
	}
}

void eepromErrorInterruptHandler(void) {
	if (LL_I2C_IsActiveFlag_ARLO(I2C3)) {
		*currFinishFlag = EEPROM_ERROR;
		currFinishFlag = NULL;
		LL_I2C_GenerateStopCondition(I2C3);
		LL_I2C_ClearFlag_ARLO(I2C3);
	}

	if (LL_I2C_IsActiveFlag_BERR(I2C3)) {
		*currFinishFlag = EEPROM_ERROR;
		currFinishFlag = NULL;
		LL_I2C_GenerateStopCondition(I2C3);
		LL_I2C_ClearFlag_BERR(I2C3);
	}
}
