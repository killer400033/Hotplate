#include <stdint.h>

#define EEPROM_SLAVE_ADD 0xA0
#define DATA_BUFFER_SIZE 30

#define CURVE_ADDRESS 0x0000
#define CALIB_ADDRESS 0x0040

typedef enum TRANSFER_STATUS {
	EEPROM_PENDING,
	EEPROM_FINISHED,
	EEPROM_ERROR,
} TRANSFER_STATUS;

void eepromInitialize(void);
void eepromInterruptHandler(void);
void eepromErrorInterruptHandler(void);
uint8_t eepromWriteData(uint16_t address, uint8_t *data, uint16_t n, TRANSFER_STATUS *finishFlag);
uint8_t eepromReadData(uint16_t address, uint8_t *data, uint16_t n, TRANSFER_STATUS *finishFlag);
