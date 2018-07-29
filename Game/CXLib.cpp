#include "CXLib.h"
#include "GameMain.h"
#include "BufferedConsole.h"
#include <string.h>
#include "GameUtils.h"

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

void DrawLine(float x1, float y1, float x2, float y2, ATTR Color, const char* Str)
{
	int cx1 = (int)ConsoleX(x1);
	int cy1 = (int)ConsoleY(y1);
	int cx2 = (int)ConsoleX(x2);
	int cy2 = (int)ConsoleY(y2);

	BOOL steep = GetAbs(cy2 - cy1) > GetAbs(cx2 - cx1);
	if (steep)
	{
		Swap(&cx1, &cy1);
		Swap(&cx2, &cy2);
	}

	{
		int deltax = GetAbs(cx2 - cx1);
		int deltay = GetAbs(cy2 - cy1);
		int error = deltax / 2;

		int incx = (cx1 < cx2) ? 1 : -1;
		int incy = (cy1 < cy2) ? 1 : -1;

		SHORT x;
		SHORT y = cy1;
		for (x = cx1; x != cx2; x += incx)
		{
			Print({ steep ? y : x, steep ? x : y }, Color, Str);

			if ((error -= deltay) < 0)
			{
				y += incy;
				error += deltax;
			}
		}
	}
}
