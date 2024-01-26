#include "bitmaps.h"

static const uint8_t logo_data[896] =
{
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x8,0x2,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x18,0x6,0x3,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x60,0x18,0x6,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x80,0x20,0x18,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0xc0,0x10,0x8,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x70,0xc,0x6,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x18,0x7,0x3,0x80,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x4,0x1,0x80,0xc0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x80,0x40,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x6,0x1,0x80,0x40,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0xc,0x3,0x0,0x80,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x38,0xe,0x3,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x40,0x10,0xc,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0x0,0x0,
	0x0,0x7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x0,0x0,
	0x0,0x7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x0,0x0,
	0x0,0x3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0x0,0x0,
	0x0,0x0,0x3,0xe0,0x0,0x0,0x0,0x0,0x0,0x0,0xf8,0x0,0x0,0x0,
	0x0,0x3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0x0,0x0,
	0x0,0x7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0xff,0xff,0xff,0xf8,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0xff,0xff,0xff,0xfc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0xf0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0xf0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0xf0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0xf0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0x80,0x0,0x0,0xc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x1,0xff,0xff,0xff,0xfc,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0xff,0xff,0xff,0xf8,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x4,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x2,0x0,0x0,
	0x0,0x7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0x0,0x0,
	0x0,0x3,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0xf,0xe4,0xd,0xf8,0x38,0xdf,0xe7,0xfb,0xfd,0xc3,0xfc,0xff,0x7f,0x80,
	0xc,0xc,0x19,0x98,0x38,0xd8,0x40,0xc3,0x1d,0xc3,0xc,0x1c,0x60,0x0,
	0xc,0xe,0x3b,0x1c,0x30,0x98,0x40,0xc3,0x19,0x83,0xc,0x18,0x60,0x0,
	0x18,0xf,0xfb,0xc,0x71,0xb0,0xc1,0xc7,0x1b,0x87,0x18,0x38,0xe0,0x0,
	0x1f,0xc9,0xb3,0x1c,0x7f,0xb0,0xc1,0xc7,0xf3,0x7,0xf8,0x38,0xfe,0x0,
	0x1,0xd8,0x36,0x70,0xe3,0x39,0x83,0x8e,0x7,0xe,0x38,0x71,0xc0,0x0,
	0x1f,0x98,0x67,0xe0,0xc3,0x1f,0x83,0x8c,0x7,0xec,0x38,0x71,0xfe,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
	0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0
};

Bitmap logo = {.data=(uint8_t *)logo_data, .byte_width=14, .height=64};

static const uint8_t fan_data[51] =
{
	0xc0,0x0,0xc0,
	0x87,0x0,0x40,
	0xf,0x80,0x0,
	0x1f,0x9e,0x0,
	0x1f,0x3f,0x0,
	0x1f,0x7f,0x80,
	0xf,0x7f,0x80,
	0x7,0xff,0x80,
	0x31,0x23,0x0,
	0x7f,0x38,0x0,
	0x7f,0xfc,0x0,
	0x7f,0xbe,0x0,
	0x3f,0x3e,0x0,
	0x1e,0x7c,0x0,
	0x0,0x7c,0x0,
	0x80,0x38,0x40,
	0xc0,0x0,0xc0
};

Bitmap fan = {.data=(uint8_t *)fan_data, .byte_width=3, .height=17};


static const uint8_t heat_data[51] =
{
	0xc0,0x0,0xc0,
	0x8c,0xcc,0x40,
	0x11,0x10,0x0,
	0x11,0x10,0x0,
	0x11,0x10,0x0,
	0x11,0x10,0x0,
	0x10,0x88,0x0,
	0x8,0x84,0x0,
	0x4,0x44,0x0,
	0x4,0x44,0x0,
	0x4,0x44,0x0,
	0x4,0x44,0x0,
	0x19,0x98,0x0,
	0x0,0x0,0x0,
	0x3f,0xff,0x0,
	0x80,0x0,0x40,
	0xc0,0x0,0xc0
};

Bitmap heat = {.data=(uint8_t *)heat_data, .byte_width=3, .height=17};


static const uint8_t temp_data[51] =
{
	0xc0,0xc0,0xc0,
	0x81,0x20,0x40,
	0x2,0x10,0x0,
	0x2,0x10,0x0,
	0x2,0x10,0x0,
	0x2,0x10,0x0,
	0x2,0xd0,0x0,
	0x2,0xd0,0x0,
	0x2,0xd0,0x0,
	0x2,0xd0,0x0,
	0x2,0xd0,0x0,
	0x5,0xe8,0x0,
	0x5,0xe8,0x0,
	0x5,0xe8,0x0,
	0x5,0xe8,0x0,
	0x86,0x18,0x40,
	0xc1,0xe0,0xc0
};

Bitmap temp = {.data=(uint8_t *)temp_data, .byte_width=3, .height=17};

static const uint8_t delta_data[51] =
{
	0xc0,0x0,0xc0,
	0x80,0x0,0x40,
	0x0,0xc0,0x0,
	0x1,0x20,0x0,
	0x2,0x20,0x0,
	0x2,0x90,0x0,
	0x5,0x50,0x0,
	0x5,0x28,0x0,
	0xa,0x28,0x0,
	0xa,0x14,0x0,
	0x14,0x12,0x0,
	0x14,0xa,0x0,
	0x28,0xd,0x0,
	0x2f,0xf9,0x0,
	0x20,0x3,0x0,
	0x9f,0xfc,0x40,
	0xc0,0x0,0xc0
};

Bitmap delta = {.data=(uint8_t *)delta_data, .byte_width=3, .height=17};

static const uint8_t x_axis_data[128] =
{
	0x80,0x0,0x4,0x0,0x0,0x20,0x0,0x1,0x0,0x0,0x8,0x0,0x0,0x40,0x0,0x4,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfc,
	0x80,0x0,0x4,0x0,0x0,0x20,0x0,0x1,0x0,0x0,0x8,0x0,0x0,0x40,0x0,0x4,
	0x70,0x3,0xb8,0x0,0x5d,0xc0,0x2,0xee,0x0,0x71,0x70,0x3,0xbb,0x80,0x3b,0xb8,
	0x50,0x2,0x28,0x0,0x45,0x40,0x2,0xaa,0x0,0x13,0x50,0x0,0xaa,0x80,0xa,0x28,
	0x50,0x3,0xa8,0x0,0x5d,0x40,0x2,0xea,0x0,0x75,0x50,0x3,0xaa,0x80,0x3b,0xa8,
	0x50,0x2,0xa8,0x0,0x51,0x40,0x2,0xaa,0x0,0x47,0x50,0x0,0xaa,0x80,0xa,0xa8,
	0x70,0x3,0xb8,0x0,0x5d,0xc0,0x2,0xee,0x0,0x71,0x70,0x3,0xbb,0x80,0x3b,0xb8
};

Bitmap x_axis = {.data=(uint8_t *)x_axis_data, .byte_width=16, .height=8};

