#pragma once
#include "BufferedConsole.h"

#define CX_PI_F (3.1415926535897932384626433832795f)

extern const ATTR ATTR_WHITE;

float ConsoleXF(float world_x, float size = 1);

float ConsoleYF(float world_y, float size = 1);

SHORT ConsoleX(float world_x, float size = 1);

SHORT ConsoleY(float world_y, float size = 1);

float WorldXF(float screen_x, float size = 1);

float WorldYF(float screen_y, float size = 1);

float WorldX(SHORT screen_x, float size = 1);

float WorldY(SHORT screen_y, float size = 1);

void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str = " ");

void DrawOval(float x, float y, float rx, float ry, ATTR Color, int FillFlag, const char* Str = " ");

void DrawCircle(float x, float y, float r, ATTR Color, int FillFlag, const char* Str = " ");

void DrawLine(float x1, float y1, float x2, float y2, ATTR Color, const char* Str = " ");

void DrawDashedLine(float x1, float y1, float x2, float y2, ATTR color, float length, const char* Str = " ");
