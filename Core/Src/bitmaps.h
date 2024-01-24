#include <stdint.h>

typedef struct Bitmap {
	uint8_t *data;
	uint8_t byte_width;
	uint8_t height;
} Bitmap;

extern Bitmap logo;
extern Bitmap fan;
extern Bitmap heat;
extern Bitmap temp;
extern Bitmap delta;
