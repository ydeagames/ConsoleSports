#pragma once
#include "BufferedConsole.h"

float ConsoleXF(float world_x, float size = 1);

float ConsoleYF(float world_y, float size = 1);

SHORT ConsoleX(float world_x, float size = 1);

SHORT ConsoleY(float world_y, float size = 1);

float WorldXF(float screen_x, float size = 1);

float WorldYF(float screen_y, float size = 1);

float WorldX(SHORT screen_x, float size = 1);

float WorldY(SHORT screen_y, float size = 1);

void DrawString(float x, float y, const char* String, ATTR Color = DEFAULT_ATTR);

void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str = " ");

void DrawOval(float x, float y, float rx, float ry, ATTR Color, int FillFlag, const char* Str = " ");

void DrawLine(float x1, float y1, float x2, float y2, ATTR Color, const char* Str = " ");
