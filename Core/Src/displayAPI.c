#include <stdint.h>
#include "displayDriver.h"
#include "displayAPI.h"

extern uint32_t getSqrt(float input);

void clearDisplay() {
	for (uint16_t i = 0; i < DISPLAY_Y; i++) {
		for (uint16_t j = 1; j < X_BYTES + 1; j++) {
			displayBuffer[i][j] = 0x00;
		}
	}
}

void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	// x_low and x_high have been incremented by 1 to account for 1 byte offset in displayBuffer
	uint8_t x_low = x/4;
	uint8_t x_high = (x+width+ (4-1) )/4; // Round up
	x_low++;
	x_high++;
	uint8_t x_low_mask = ((1 << (4 - x%4)) - 1) << 4;
	uint8_t x_high_mask = ~((1 << (4 - (x+width)%4)) - 1) << 4;
	if (x_high_mask == 0x00) x_high_mask = 0xF0;

	uint16_t saved_low;
	uint16_t saved_high;
	for (uint16_t j = y; j < y + height; j++) {
		saved_low = displayBuffer[j][x_low];
		saved_high = displayBuffer[j][x_high - 1];
		for (uint16_t i = x_low; i < x_high; i++) {
			displayBuffer[j][i] = 0xF0;
		}
		displayBuffer[j][x_low] &= x_low_mask | saved_low;
		displayBuffer[j][x_high - 1] &= x_high_mask | saved_high;
	}
}

void drawRectangleHollow(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness) {
	drawRectangle(x, y, thickness, height);
	drawRectangle(x + width - thickness, y, thickness, height);
	drawRectangle(x, y, width, thickness);
	drawRectangle(x, y + height - thickness, width, thickness);
}

void drawCircle(uint16_t x, uint16_t y, uint16_t r) {
	for (int16_t y_offset = r; y_offset >= -r; y_offset--) {
		uint16_t x_offset = (uint16_t)getSqrt((float)((r*r) - (y_offset*y_offset)));
		uint16_t x_min = x - x_offset;
		uint16_t x_max = x + x_offset;

		uint8_t x_byte_low = x_min/4;
		uint8_t x_byte_high = (x_max+(4-1))/4; // Round up
		x_byte_low++;
		x_byte_high++;
		uint8_t x_low_mask = ((1 << (4 - x_min%4)) - 1) << 4;
		uint8_t x_high_mask = ~((1 << (4 - (x_max)%4)) - 1) << 4;
		if (x_high_mask == 0x00) x_high_mask = 0xF0;

		uint16_t saved_low = displayBuffer[y + y_offset][x_byte_low];
		uint16_t saved_high = displayBuffer[y + y_offset][x_byte_high - 1];
		for (uint16_t i = x_byte_low; i < x_byte_high; i++) {
			displayBuffer[y + y_offset][i] = 0xF0;
		}
		displayBuffer[y + y_offset][x_byte_low] &= x_low_mask | saved_low;
		displayBuffer[y + y_offset][x_byte_high - 1] &= x_high_mask | saved_high;
	}
}

void drawCircleHollow(uint16_t x, uint16_t y, uint16_t r, uint16_t thickness) {
	drawCircle(x, y, r);
	r -= thickness;

	for (int16_t y_offset = r; y_offset >= -r; y_offset--) {
		uint16_t x_offset = getSqrt((float)((r*r) - (y_offset*y_offset)));
		uint16_t x_min = x - x_offset;
		uint16_t x_max = x + x_offset;

		uint8_t x_byte_low = x_min/4;
		uint8_t x_byte_high = (x_max+(4-1))/4; // Round up
		x_byte_low++;
		x_byte_high++;
		uint8_t x_low_mask = ((1 << (4 - x_min%4)) - 1) << 4;
		uint8_t x_high_mask = ~((1 << (4 - (x_max)%4)) - 1) << 4;
		if (x_high_mask == 0x00) x_high_mask = 0xF0;

		uint16_t saved_low = displayBuffer[y + y_offset][x_byte_low];
		uint16_t saved_high = displayBuffer[y + y_offset][x_byte_high - 1];
		for (uint16_t i = x_byte_low; i < x_byte_high; i++) {
			displayBuffer[y + y_offset][i] = 0x00;
		}
		displayBuffer[y + y_offset][x_byte_low] |= (~x_low_mask & 0xF0) & saved_low;
		displayBuffer[y + y_offset][x_byte_high - 1] |= (~x_high_mask & 0xF0) & saved_high;
	}
}

void drawString(uint16_t x, uint16_t y, Font font, char *str, uint16_t n) {
	for (uint16_t i = 0; i < n; i++) {
		if (str[i] == '\0') break;
		drawBitMap(x, y, &font.data[(str[i] - 0x20) * font.byte_width * font.height], font.byte_width, font.height);
		x += font.byte_width * 8;
	}
}

void drawBitMap(uint16_t x, uint16_t y, uint8_t *bitmap, uint16_t byte_width, uint16_t height) {
	for (uint16_t j = 0; j < height; j++) {
		uint16_t x_byte_low = x/4;
		uint16_t x_byte_high = (x+8*byte_width+(4-1))/4; // Round up
		displayBuffer[y + j][x_byte_low] |= (bitmap[j*byte_width] >> x%4) & 0xF0;

		uint16_t shift = (4-(x%4));
		for (uint16_t x_byte = x_byte_low+1; x_byte < x_byte_high; x_byte++) {
			displayBuffer[y + j][x_byte] |= (bitmap[j*byte_width + shift/8] << (shift%8)) & 0xF0;
			shift += 4;
		}
	}
}
