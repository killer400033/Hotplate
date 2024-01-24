#include "fonts.h"

void clearDisplay(void);
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void drawRectangleHollow(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness);
void drawCircle(uint16_t x, uint16_t y, uint16_t r);
void drawCircleHollow(uint16_t x, uint16_t y, uint16_t r, uint16_t thickness);
void drawString(uint16_t x, uint16_t y, Font font, char *str, uint16_t n);
void drawBitMap(uint16_t x, uint16_t y, uint8_t *bitmap, uint16_t byte_width, uint16_t height);
