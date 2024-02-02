#include "../Data/bitmaps.h"
#include "../Data/fonts.h"

void clearDisplay(void);
void drawAngledLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void drawRectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
void drawRectangleHollow(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t thickness);
void drawCircle(uint16_t x, uint16_t y, uint16_t r);
void drawCircleHollow(uint16_t x, uint16_t y, uint16_t r, uint16_t thickness);
void drawString(uint16_t x, uint16_t y, Font font, char *str);
void drawBitMap(uint16_t x, uint16_t y, Bitmap bitmap);
