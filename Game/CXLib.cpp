#include "CXLib.h"
#include "GameMain.h"
#include "BufferedConsole.h"
#include <string.h>

float ConsoleXF(float world_x)
{
	return world_x * SCREEN_RESOLUTION_X;
}

float ConsoleYF(float world_y)
{
	return world_y * SCREEN_RESOLUTION_Y;
}

SHORT ConsoleX(float world_x)
{
	return (SHORT)ConsoleXF(world_x);
}

SHORT ConsoleY(float world_y)
{
	return (SHORT)ConsoleYF(world_y);
}

void DrawString(float x, float y, const char* String, ATTR Color)
{
	Print({ ConsoleX(x), ConsoleY(y) }, Color, String);
}

void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str)
{
	SHORT cx1 = ConsoleX(x1);
	SHORT cy1 = ConsoleY(y1);
	SHORT cx2 = ConsoleX(x2);
	SHORT cy2 = ConsoleY(y2);

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

void DrawOval(float x, float y, float rx, float ry, ATTR Color, int FillFlag, const char* Str)
{
	SHORT cx = ConsoleX(x);
	SHORT cy = ConsoleY(y);
	SHORT crx = ConsoleX(rx);
	SHORT cry = ConsoleY(ry);
	float crxf = ConsoleXF(rx);
	float cryf = ConsoleYF(ry);

	int str_len = MAX(1, strlen(Str));

	SHORT ix, iy;
	for (iy = -cry; iy <= cry; iy++)
	{
		for (ix = -crx; ix <= crx; ix += str_len)
		{
			float p = (ix*ix) / (crxf*crxf) + (iy*iy) / (cryf*cryf) - 1;
			if (FillFlag ? p <= .08f : -.25f < p && p < .08f)
				Print({ cx + ix, cy + iy }, Color, Str);
		}
	}
}
