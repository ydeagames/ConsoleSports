#include "BufferedConsole.h"
#include <stdio.h>
#include <string.h>
#include "GameMain.h"
#include "Console.h"

typedef struct {
	const char* str;
	SHORT size;
} STR;

typedef struct {
	STR str;
	ATTR attributes;
} PIXEL, *PPIXEL;

const ATTR DEFAULT_ATTRIBUTES = { COLOR_WHITE, COLOR_BLACK };

static const PIXEL default_pixel = { { " ", 1 },{ COLOR_WHITE, COLOR_BLACK } };

static PIXEL buffer1[SCREEN_HEIGHT*SCREEN_WIDTH];
static PIXEL buffer2[SCREEN_HEIGHT*SCREEN_WIDTH];

static COORD last_coord;
static ATTR last_attributes;
static BOOL swap_flag;

void BufferedConsole_Initialize(void)
{
	last_attributes = DEFAULT_ATTRIBUTES;

	int i;
	for (i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++)
	{
		buffer1[i] = default_pixel;
		buffer2[i] = default_pixel;
	}

	last_coord = { 0, 0 };
	swap_flag = FALSE;
}

static void SwapScreen(void)
{
	swap_flag = !swap_flag;
}

static PPIXEL GetScreen(void)
{
	return swap_flag ? buffer1 : buffer2;
}

static PPIXEL GetOffScreen(void)
{
	return swap_flag ? buffer2 : buffer1;
}

static PPIXEL GetPixel(const PPIXEL screen, COORD coord)
{
	return &screen[coord.Y*SCREEN_WIDTH + coord.X];
}

void Clear(void)
{
	PPIXEL screen = GetOffScreen();

	int i;
	for (i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++)
	{
		screen[i] = default_pixel;
	}
}

void Print(COORD coord, ATTR attributes, const char* format)
{
	PPIXEL screen = GetOffScreen();

	SHORT ix, iy, size;
	for (iy = coord.Y; iy < SCREEN_HEIGHT; iy++)
	{
		for (ix = coord.X;; ix++, format++)
		{
			if (*format == '\0')
				return;
			if (*format == '\n')
			{
				format++;
				break;
			}
			if (ix >= SCREEN_WIDTH)
			{
				format = strchr(format, '\n') + 1;
				break;
			}

			size = (SHORT)(strchr(format, '\n') - format);
			*GetPixel(screen, { ix, iy }) = { {format, MIN(size, SCREEN_RIGHT - ix) }, attributes };
		}
	}
}

static void FlushPixel(COORD coord, PPIXEL pixel_before, PPIXEL pixel_after)
{
	if (pixel_before->attributes.background != pixel_after->attributes.background && last_attributes.background != pixel_after->attributes.background)
		SetBackColor(pixel_after->attributes.background);
	if (pixel_before->attributes.foreground != pixel_after->attributes.foreground && last_attributes.foreground != pixel_after->attributes.foreground)
		SetTextColor(pixel_after->attributes.foreground);
	if (pixel_before->str.size != pixel_after->str.size || strcmp(pixel_before->str.str, pixel_after->str.str) == 0)
	{
		if (coord.X != last_coord.X || coord.Y != last_coord.Y)
			SetCursorPosition(coord.X, coord.Y);
		printf("%s", pixel_after->str.str);
		last_coord = { coord.X + pixel_after->str.size, coord.Y };
	}
}

void BufferedConsole_Flush(void)
{
	PPIXEL screen_before = GetScreen();
	PPIXEL screen_after = GetOffScreen();

	SHORT ix, iy;
	for (iy = 0; iy < SCREEN_HEIGHT; iy++)
	{
		for (ix = 0; ix < SCREEN_WIDTH;)
		{
			COORD coord = { ix, iy };

			PPIXEL pixel_before = GetPixel(screen_after, coord);
			PPIXEL pixel_after = GetPixel(screen_after, coord);

			FlushPixel(coord, pixel_before, pixel_after);

			ix += MAX(1, pixel_after->str.size);
		}
	}

	SwapScreen();
}
