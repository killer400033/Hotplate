#include <stdint.h>

typedef struct Font {
	uint8_t *data;
	uint8_t byte_width;
	uint8_t height;
} Font;

extern Font font8x8;
