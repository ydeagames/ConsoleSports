#pragma once
#include "BufferedConsole.h"

// π
#define CX_PI_F (3.1415926535897932384626433832795f)

// 白色塗りつぶし
extern const ATTR ATTR_WHITE;

// ワールド座標をコンソール座標に変換
float WorldToConsoleXF(float world_x, float size = 1);

// ワールド座標をコンソール座標に変換
float WorldToConsoleYF(float world_y, float size = 1);

// ワールド座標をコンソール座標に変換
SHORT WorldToConsoleX(float world_x, float size = 1);

// ワールド座標をコンソール座標に変換
SHORT WorldToConsoleY(float world_y, float size = 1);

// コンソール座標をワールド座標に変換
float ConsoleToWorldXF(float screen_x, float size = 1);

// コンソール座標をワールド座標に変換
float ConsoleToWorldYF(float screen_y, float size = 1);

// コンソール座標をワールド座標に変換
float ConsoleToWorldX(SHORT screen_x, float size = 1);

// コンソール座標をワールド座標に変換
float ConsoleToWorldY(SHORT screen_y, float size = 1);

// 矩形描画関数
void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str = " ");

// 楕円描画関数
void DrawOval(float x, float y, float rx, float ry, ATTR Color, int FillFlag, const char* Str = " ");

// 円描画関数
void DrawCircle(float x, float y, float r, ATTR Color, int FillFlag, const char* Str = " ");

// 線描画関数
void DrawLine(float x1, float y1, float x2, float y2, ATTR Color, const char* Str = " ");

// 破線を描画
void DrawDashedLine(float x1, float y1, float x2, float y2, ATTR color, float length, const char* Str = " ");
