#include "displayAPI.h"

#include <stdint.h>

#include "displayDriver.h"

extern uint32_t getSqrt(float input);
void _drawBitMap(uint16_t x, uint16_t y, uint8_t *bitmap, uint16_t byte_width, uint16_t height);
void _drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color);
void _drawCircle(uint16_t x, uint16_t y, uint16_t r, uint8_t color);

void clearDisplay() {
	for (uint16_t i = 0; i < DISPLAY_Y; i++) {
		for (uint16_t j = 1; j < X_BYTES + 1; j++) {
			displayBuffer[i][j] = 0x00;
		}
	}
}

void drawAngledLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
	float gradient = (float)(y2 - y1) / (float)(x2 - x1);
	float accumulator = 0;

	if (gradient <= 1.0 && gradient >= -1.0) {
		if (x2 < x1) {
			uint16_t temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		for (uint16_t i = x1; i < x2; i++) {
			uint8_t x_byte = (i/4) + 1;
			displayBuffer[(uint16_t)(y1 + accumulator)][x_byte] |= 0x80 >> (i%4);
			accumulator += gradient;
		}
	}
	else {
		if (y2 < y1) {
			uint16_t temp = x1;
			x1 = x2;
			x2 = temp;
			temp = y1;
			y1 = y2;
			y2 = temp;
		}
		gradient = 1.0 / gradient;
		for (uint16_t i = y1; i < y2; i++) {
			uint8_t x_byte = (((uint16_t)(x1 + accumulator))/4) + 1;
			displayBuffer[i][x_byte] |= 0x80 >> (((uint16_t)(x1 + accumulator))%4);
			accumulator += gradient;
		}
	}
}

void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
	_drawRectangle(x, y, width, height, 1);
}

void drawRectangleHollow(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness) {
	// Set inside to empty
	_drawRectangle(x+1, y+1, width-2, height-2, 0);

	_drawRectangle(x, y, thickness, height, 1);
	_drawRectangle(x + width - thickness, y, thickness, height, 1);
	_drawRectangle(x, y, width, thickness, 1);
	_drawRectangle(x, y + height - thickness, width, thickness, 1);
}

void drawCircle(uint16_t x, uint16_t y, uint16_t r) {
	_drawCircle(x, y, r, 1);
}

void drawCircleHollow(uint16_t x, uint16_t y, uint16_t r, uint16_t thickness) {
	_drawCircle(x, y, r, 1);
	r -= thickness;
	_drawCircle(x, y, r, 0);
}

void drawString(uint16_t x, uint16_t y, Font font, char *str) {
	uint16_t i = 0;
	while (str[i] != '\0') {
		uint8_t byte_width = (font.width + (8-1)) / 8; // Round up
		_drawBitMap(x, y, &font.data[(str[i] - 0x20) * byte_width * font.height], byte_width, font.height);
		x += font.width;
		i++;
	}
}

void drawBitMap(uint16_t x, uint16_t y, Bitmap bitmap) {
	_drawBitMap(x, y, bitmap.data, bitmap.byte_width, bitmap.height);
}

void _drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t color) {
	uint8_t x_low = x/4;
	uint8_t x_high = (x+width+ (4-1) )/4; // Round up
	// x_low and x_high have been incremented by 1 to account for 1 byte offset in displayBuffer
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
		if (color) {
			for (uint16_t i = x_low; i < x_high; i++) {
				displayBuffer[j][i] = 0xF0;
			}
			displayBuffer[j][x_low] &= x_low_mask | saved_low;
			displayBuffer[j][x_high - 1] &= x_high_mask | saved_high;
		}
		else {
			for (uint16_t i = x_low; i < x_high; i++) {
				displayBuffer[j][i] = 0x00;
			}
			displayBuffer[j][x_low] |= (~x_low_mask & 0xF0) & saved_low;
			displayBuffer[j][x_high - 1] |= (~x_high_mask & 0xF0) & saved_high;
		}
	}
}

void _drawCircle(uint16_t x, uint16_t y, uint16_t r, uint8_t color) {
	for (int16_t y_offset = r; y_offset >= -r; y_offset--) {
		uint16_t x_offset = (uint16_t)getSqrt((float)((r*r) - (y_offset*y_offset)));
		uint16_t x_min = x - x_offset;
		uint16_t x_max = x + x_offset;

		uint8_t x_byte_low = x_min/4;
		uint8_t x_byte_high = (x_max+(4-1))/4; // Round up
		// x_low and x_high have been incremented by 1 to account for 1 byte offset in displayBuffer
		x_byte_low++;
		x_byte_high++;
		uint8_t x_low_mask = ((1 << (4 - x_min%4)) - 1) << 4;
		uint8_t x_high_mask = ~((1 << (4 - (x_max)%4)) - 1) << 4;
		if (x_high_mask == 0x00) x_high_mask = 0xF0;

		uint16_t saved_low = displayBuffer[y + y_offset][x_byte_low];
		uint16_t saved_high = displayBuffer[y + y_offset][x_byte_high - 1];
		if (color) {
			for (uint16_t i = x_byte_low; i < x_byte_high; i++) {
				displayBuffer[y + y_offset][i] = 0xF0;
			}
			displayBuffer[y + y_offset][x_byte_low] &= x_low_mask | saved_low;
			displayBuffer[y + y_offset][x_byte_high - 1] &= x_high_mask | saved_high;
		}
		else {
			for (uint16_t i = x_byte_low; i < x_byte_high; i++) {
				displayBuffer[y + y_offset][i] = 0x00;
			}
			displayBuffer[y + y_offset][x_byte_low] |= (~x_low_mask & 0xF0) & saved_low;
			displayBuffer[y + y_offset][x_byte_high - 1] |= (~x_high_mask & 0xF0) & saved_high;
		}
	}
}

void _drawBitMap(uint16_t x, uint16_t y, uint8_t *bitmap, uint16_t byte_width, uint16_t height) {
	for (uint16_t j = 0; j < height; j++) {
		uint16_t x_byte_low = x/4;
		uint16_t x_byte_high = (x+8*byte_width+(4-1))/4; // Round up
		// x_low and x_high have been incremented by 1 to account for 1 byte offset in displayBuffer
		x_byte_low++;
		x_byte_high++;
		displayBuffer[y + j][x_byte_low] |= (bitmap[j*byte_width] >> x%4) & 0xF0;

		uint16_t shift = (4-(x%4));
		uint8_t rollingBuffer;
		for (uint16_t x_byte = x_byte_low+1; x_byte < x_byte_high; x_byte++) {
			rollingBuffer = (bitmap[j*byte_width + shift/8] << (shift%8));
			if ((j*byte_width + shift/8 + 1)%byte_width != 0) {
				rollingBuffer |= (bitmap[j*byte_width + shift/8 + 1] >> (8 - shift%8));
			}
			displayBuffer[y + j][x_byte] |= rollingBuffer & 0xF0;
			shift += 4;
		}
	}
}
