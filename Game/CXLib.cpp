#include "CXLib.h"
#include "GameMain.h"
#include "BufferedConsole.h"
#include <string.h>

SHORT ConsoleX(float world_x)
{
	return (SHORT)(world_x * SCREEN_RESOLUTION_X);
}

SHORT ConsoleY(float world_y)
{
	return (SHORT)(world_y * SCREEN_RESOLUTION_Y);
}

void DrawString(float x, float y, const char* String, ATTR Color)
{
	Print({ ConsoleX(x), ConsoleY(y) }, Color, String);
}

void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str)
{
	int cx1 = ConsoleX(x1);
	int cy1 = ConsoleY(y1);
	int cx2 = ConsoleX(x2);
	int cy2 = ConsoleY(y2);

	int str_len = MAX(1, strlen(Str));

	SHORT ix, iy;
	for (iy = cy1; iy <= cy2; iy++)
	{
		for (ix = cx1; ix <= cx2; ix += str_len)
		{
			if (FillFlag || (ix == cx1 || ix == cx2) || (iy == cy1 || iy == cy2))
				Print({ ix, iy }, Color, Str);
		}
	}
}

