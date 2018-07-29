//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   BufferedConsole.cpp
//!
//! @brief  ソフトウェア・コンソールバッファ
//!
//! @date   2018/07/29
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================

#include "BufferedConsole.h"
#include "Game.h"
#include <stdio.h>
#include <string.h>
#include "Console.h"
#include "GameUtils.h"

// 構造体の定義 ============================================================

// 文字列オブジェクト
typedef struct {
	// 文字列
	const char* str;
	// 長さ
	SHORT size;
} STR;

// ピクセル情報
typedef struct {
	// 文字列オブジェクト
	STR str;
	// 属性(色)
	ATTR attributes;
} PIXEL, *PPIXEL;

// 関数の宣言 ==============================================================

// 文字列オブジェクトを作成
static STR CreateSTR(const char* str);

// グローバル変数の定義 ====================================================

// デフォルトの属性(色)
const ATTR DEFAULT_ATTR = CreateATTR(COLOR_WHITE, COLOR_BLACK);
// デフォルトの文字列オブジェクト
const STR DEFAULT_STR = CreateSTR(" ");

// デフォルトのピクセル情報
static const PIXEL default_pixel = { CreateSTR(" "), CreateATTR(COLOR_WHITE, COLOR_BLACK) };

// 現在表示されている画面
static PIXEL buffer1[SCREEN_HEIGHT*SCREEN_WIDTH];
// 次表示する画面
static PIXEL buffer2[SCREEN_HEIGHT*SCREEN_WIDTH];

// 最後に描画した位置
static COORD last_coord;
// 最後に描画した属性(色)
static ATTR last_attributes;

// 関数の定義 ==============================================================

// 属性(色)を作成
ATTR CreateATTR(ConsoleColor foreground, ConsoleColor background)
{
	return{ foreground, background };
}

// 文字列オブジェクトを作成
static STR CreateSTR(const char* str)
{
	return{ str, (SHORT)strlen(str) };
}

// ソフトウェア・コンソールバッファを初期化する
void BufferedConsole_Initialize(void)
{
	// スクリーン、オフスクリーンをデフォルトピクセルで初期化
	int i;
	for (i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++)
	{
		buffer1[i] = default_pixel;
		buffer2[i] = default_pixel;
	}

	// 最後に描画した位置を更新
	last_coord = { 0, 0 };
	// 最後に描画した属性(色)を更新
	last_attributes = DEFAULT_ATTR;
}

// 現在表示されている画面
static PPIXEL GetScreen(void)
{
	return buffer1;
}

// 次表示する画面
static PPIXEL GetOffScreen(void)
{
	return buffer2;
}

// ピクセルを取得
static PPIXEL GetPixel(const PPIXEL screen, COORD coord)
{
	return &screen[coord.Y*SCREEN_WIDTH + coord.X];
}

// 画面を初期化する
void Clear(void)
{
	PPIXEL screen = GetOffScreen();

	// オフスクリーンをデフォルトピクセルで初期化
	int i;
	for (i = 0; i < SCREEN_HEIGHT*SCREEN_WIDTH; i++)
	{
		screen[i] = default_pixel;
	}
}

// 指定座標に指定文字を指定属性(色)で表示
void Print(COORD coord, ATTR attributes, const char* format)
{
	// 右下より左上なら
	if (coord.X < SCREEN_RIGHT && coord.Y < SCREEN_BOTTOM)
	{
		// オフスクリーンに
		PPIXEL screen = GetOffScreen();

		// 左にオーバーしている距離
		SHORT negative = GetMax(0, -coord.X);

		// Yループ
		SHORT iy;
		for (iy = coord.Y; iy < SCREEN_HEIGHT; iy++)
		{
			// 改行までの距離
			const char* enter = strchr(format, '\n');

			// 文字の長さ
			SHORT size;
			// 改行がなければ\0まで、そうでなければ改行までの長さ
			if (enter == NULL)
				size = (SHORT)strlen(format);
			else
				size = (SHORT)(enter - format);

			// 左と上にオーバーしすぎて見えなくなっていないかチェック
			if (size - negative > 0 && iy >= SCREEN_TOP)
			{
				// 左めり込み修正
				SHORT i;
				for (i = 0; i < size - negative; i++)
				{
					// 左めり込み座標
					SHORT neg = negative + i;
					// 文字の描画座標X
					SHORT ix = coord.X + neg;
					// 描画すべき文字幅
					SHORT width = (iy >= SCREEN_BOTTOM - 1) ? SCREEN_WIDTH - 1 : SCREEN_WIDTH;
					// 文字の描画座標Xが描画すべき文字幅より小さければ描画
					if (ix < width)
						*GetPixel(screen, { ix, iy }) = { {format + neg, (SHORT)GetMin(size - neg, width - ix) }, attributes };
				}
			}

			// 改行がなければここで終了
			if (enter == NULL)
				return;
			// 改行があれば改行のあとまでカーソルをずらして続行
			else
				format = enter + 1;
		}
	}
}

// ピクセルを反映
static void FlushPixel(COORD coord, PPIXEL pixel_before, PPIXEL pixel_after)
{
	// 更新があるか
	BOOL modified = FALSE;
	// 背景色、テキスト色、文字のいずれかが更新されていないか
	if (pixel_before->attributes.background != pixel_after->attributes.background ||
		pixel_before->attributes.foreground != pixel_after->attributes.foreground ||
		pixel_before->str.size != pixel_after->str.size || strcmp(pixel_before->str.str, pixel_after->str.str) != 0)
		modified = TRUE;

	// 更新されていた場合
	if (modified)
	{
		// 座標が連続していない場合は座標をセット
		if (coord.X != last_coord.X || coord.Y != last_coord.Y)
			SetCursorPosition(coord.X, coord.Y);
		// 最後に描画した背景色が更新されていたら背景色を変更する
		if (last_attributes.background != pixel_after->attributes.background)
		{
			SetBackColor(pixel_after->attributes.background);
			last_attributes.background = pixel_after->attributes.background;
		}
		// 最後に描画したテキスト色が更新されていたらテキスト色を変更する
		if (last_attributes.foreground != pixel_after->attributes.foreground)
		{
			SetTextColor(pixel_after->attributes.foreground);
			last_attributes.foreground = pixel_after->attributes.foreground;
		}
		// 文字を指定した長さで描画する
		printf("%.*s", pixel_after->str.size, pixel_after->str.str);
		// 次の座標をセットする(最後に描画した座標+1の座標)
		last_coord = { coord.X + pixel_after->str.size, coord.Y };

		// スクリーンの情報をオフスクリーンの情報で更新
		{
			int i;
			for (i = 0; i < pixel_after->str.size; i++)
				pixel_before[i] = pixel_after[i];
		}
	}
}

// スクリーンを反映
void BufferedConsole_Flush(void)
{
	// 現在表示されている画面
	PPIXEL screen_before = GetScreen();
	// 次表示する画面
	PPIXEL screen_after = GetOffScreen();

	// Yループ
	SHORT ix, iy;
	for (iy = 0; iy < SCREEN_HEIGHT; iy++)
	{
		// Xループ
		for (ix = 0; ix < SCREEN_WIDTH;)
		{
			// 現在の座標
			COORD coord = { ix, iy };

			// 現在表示されているピクセル
			PPIXEL pixel_before = GetPixel(screen_before, coord);
			// 次表示するピクセル
			PPIXEL pixel_after = GetPixel(screen_after, coord);

			// ピクセルを反映
			FlushPixel(coord, pixel_before, pixel_after);

			// 文字の長さ分カーソルを進める
			ix += GetMax(1, pixel_after->str.size);
		}
	}
}
