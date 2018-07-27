#pragma once
#include "BufferedConsole.h"

SHORT ConsoleX(float world_x);

SHORT ConsoleY(float world_y);

void DrawString(float x, float y, const char* String, ATTR Color = DEFAULT_ATTR);

void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str = " ");

void	DrawOval(float x, float y, float rx, float ry, ATTR Color, int FillFlag, const char* Str = " ");
