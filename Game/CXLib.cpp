#include "CXLib.h"
#include "Game.h"
#include "BufferedConsole.h"
#include <string.h>
#include <math.h>
#include "GameUtils.h"

// 白色塗りつぶし
const ATTR ATTR_WHITE = CreateATTR(COLOR_BLACK, COLOR_WHITE);

// ワールド座標をコンソール座標に変換
float WorldToConsoleXF(float world_x, float size)
{
	return world_x * (SCREEN_RESOLUTION_X * size);
}

// ワールド座標をコンソール座標に変換
float WorldToConsoleYF(float world_y, float size)
{
	return world_y * (SCREEN_RESOLUTION_Y * size);
}

// ワールド座標をコンソール座標に変換
SHORT WorldToConsoleX(float world_x, float size)
{
	return (SHORT)WorldToConsoleXF(world_x, size);
}

// ワールド座標をコンソール座標に変換
SHORT WorldToConsoleY(float world_y, float size)
{
	return (SHORT)WorldToConsoleYF(world_y, size);
}

// コンソール座標をワールド座標に変換
float ConsoleToWorldXF(float screen_x, float size)
{
	return screen_x / (SCREEN_RESOLUTION_X * size);
}

// コンソール座標をワールド座標に変換
float ConsoleToWorldYF(float screen_y, float size)
{
	return screen_y / (SCREEN_RESOLUTION_Y * size);
}

// コンソール座標をワールド座標に変換
float ConsoleToWorldX(SHORT screen_x, float size)
{
	return ConsoleToWorldXF((float)screen_x, size);
}

// コンソール座標をワールド座標に変換
float ConsoleToWorldY(SHORT screen_y, float size)
{
	return ConsoleToWorldYF((float)screen_y, size);
}

// 矩形描画関数
void DrawBox(float x1, float y1, float x2, float y2, ATTR Color, int FillFlag, const char* Str)
{
	// ワールド座標をコンソール座標に変換
	SHORT cx1 = WorldToConsoleX(x1);
	SHORT cy1 = WorldToConsoleY(y1);
	SHORT cx2 = WorldToConsoleX(x2);
	SHORT cy2 = WorldToConsoleY(y2);

	// 文字の長さ
	int str_len = GetMax(1, (int)strlen(Str));

	// Yループ
	SHORT ix, iy;
	for (iy = cy1; iy <= cy2; iy++)
	{
		// Xループ
		for (ix = cx1; ix <= cx2; ix += str_len)
		{
			// 塗りつぶし時、または縁の場合描画
			if (FillFlag || (ix == cx1 || ix == cx2) || (iy == cy1 || iy == cy2))
				Print({ ix, iy }, Color, Str);
		}
	}
}

// 楕円描画関数
void DrawOval(float x, float y, float rx, float ry, ATTR Color, int FillFlag, const char* Str)
{
	// ワールド座標をコンソール座標に変換
	SHORT cx = WorldToConsoleX(x);
	SHORT cy = WorldToConsoleY(y);
	SHORT crx = WorldToConsoleX(rx);
	SHORT cry = WorldToConsoleY(ry);
	float crxf = WorldToConsoleXF(rx);
	float cryf = WorldToConsoleYF(ry);

	// 文字の長さ
	int str_len = GetMax(1, (int)strlen(Str));

	// Yループ
	SHORT ix, iy;
	for (iy = -cry; iy <= cry; iy++)
	{
		// Xループ
		for (ix = -crx; ix <= crx; ix += str_len)
		{
			// 塗りつぶし時、または縁の場合描画
			float p = (ix*ix) / (crxf*crxf) + (iy*iy) / (cryf*cryf) - 1;
			if (FillFlag ? p <= .08f : -.25f < p && p < .08f)
				Print({ cx + ix, cy + iy }, Color, Str);
		}
	}
}

// 円描画関数
void DrawCircle(float x, float y, float r, ATTR Color, int FillFlag, const char* Str)
{
	// 楕円描画
	DrawOval(x, y, r, r, Color, FillFlag, Str);
}

// 線描画関数
void DrawLine(float x1, float y1, float x2, float y2, ATTR Color, const char* Str)
{
	// ワールド座標をコンソール座標に変換
	int cx1 = (int)WorldToConsoleX(x1);
	int cy1 = (int)WorldToConsoleY(y1);
	int cx2 = (int)WorldToConsoleX(x2);
	int cy2 = (int)WorldToConsoleY(y2);

	// 縦、横、どちらの比が大きいか
	BOOL steep = GetAbs(cy2 - cy1) > GetAbs(cx2 - cx1);
	// 縦が大きかったら縦横反転
	if (steep)
	{
		Swap(&cx1, &cy1);
		Swap(&cx2, &cy2);
	}

	{
		// 差を取得
		int deltax = GetAbs(cx2 - cx1);
		int deltay = GetAbs(cy2 - cy1);
		// ズレを計算
		int error = deltax / 2;

		// インクリメント
		int incx = (cx1 < cx2) ? 1 : -1;
		int incy = (cy1 < cy2) ? 1 : -1;

		// Xループ
		SHORT x;
		SHORT y = cy1;
		for (x = cx1; x != cx2; x += incx)
		{
			// 描画
			Print({ steep ? y : x, steep ? x : y }, Color, Str);

			// ズレを修正&チェック
			if ((error -= deltay) < 0)
			{
				y += incy;
				error += deltax;
			}
		}
	}
}

// 破線を描画
void DrawDashedLine(float x1, float y1, float x2, float y2, ATTR color, float length, const char* Str)
{
	// 差
	float vx = x2 - x1;
	float vy = y2 - y1;

	// 角度
	float angle = atan2f(vy, vx);
	// 角度を適用した差
	float dx = length*cosf(angle);
	float dy = length*sinf(angle);

	// 点線の数
	int count = (int)(sqrtf(vx * vx + vy * vy) / length);

	// 初期座標
	float x = x1;
	float y = y1;
	
	// 偶数だけ繰り返し
	int i;
	for (i = 0; i < count; i += 2)
	{
		// 線を描画
		DrawLine(x, y, (x + dx), (y + dy), color, Str);
		// 座標を更新
		x += dx * 2;
		y += dy * 2;
	}

	// 最後、偶数で終わったら
	if (count % 2 == 0)
	{
		DrawLine(x, y, x2, y2, color, Str);
	}
}
