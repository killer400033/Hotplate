#include <stdint.h>
#include "displayDriver.h"

extern float getSqrt(float input);

void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	// x_low and x_high have been incremented by 1 to account for 1 byte offset in displayBuffer
	uint8_t x_low = x/4;
	uint8_t x_high = ((x+width)%4) ? (x+width)/4 + 1 : (x+width)/4;
	x_low++;
	x_high++;
	uint8_t x_low_mask = ((1 << (4 - x%4)) - 1) << 4;
	uint8_t x_high_mask = ~((1 << (4 - (x+width)%4)) - 1) << 4;
	if (x_high_mask == 0x00) x_high_mask = 0xF0;

	for (uint16_t j = y; j < y + height; j++) {
		for (uint16_t i = x_low; i < x_high; i++) {
			displayBuffer[j][i] = 0xF0;
		}
		displayBuffer[j][x_low] &= x_low_mask;
		displayBuffer[j][x_high - 1] &= x_high_mask;
	}
}

void drawRectangleHollow(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness) {
	drawRectangle(x, y, thickness, height);
	drawRectangle(x + width - thickness, y, thickness, height);
	drawRectangle(x, y, width, thickness);
	drawRectangle(x, y + height - thickness, width, thickness);
}

void drawCircle(uint16_t x, uint16_t y, float r) {
	uint16_t y_offset = r;
	float x_offset = getSqrt((r*r) - (float)(y_offset*y_offset));
	uint16_t x_min = x - x_offset;
	uint16_t x_max = x + x_offset;

	uint8_t x_byte_low = x_min/4;
	uint8_t x_byte_high = ((x_max)%4) ? (x_max)/4 + 1 : (x_max)/4;
	x_low++;
	x_high++;
	uint8_t x_low_mask = ((1 << (4 - x_min%4)) - 1) << 4;
	uint8_t x_high_mask = ~((1 << (4 - (x_max)%4)) - 1) << 4;
	if (x_high_mask == 0x00) x_high_mask = 0xF0;

	for (uint16_t i = x_byte_low; i < x_byte_high; i++) {
		displayBuffer[j][i] = 0xF0;
	}
	displayBuffer[j][x_byte_low] &= x_low_mask;
	displayBuffer[j][x_byte_high - 1] &= x_high_mask;
}
