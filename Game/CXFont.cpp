#include "CXFont.h"
#include "CXLib.h"
#include "GameMain.h"
#include "GameUtils.h"
#include <string.h>

typedef struct
{
	char name;
	int x;
	int y;
	int w;
	int h;
} Sprite;

#define TEXTURE_ATLAS_WIDTH 60
#define TEXTURE_ATLAS_HEIGHT 18
#define NUM_FONTS 19
#define FONT_SPAN_WIDTH 2.f
#define FONT_SPAN_HEIGHT 2.f

static char* texture_atlas[TEXTURE_ATLAS_HEIGHT] = {
	"11   1111  1111  1  1  1111  11111 11111  11111 11111  11111",
	"11   1111  1111  1  1  1111  11111 11111  11111 11111  11111",
	" 1   1  1     1  1  1  1     1        11  1  11 1  11  11  1",
	" 1     11   111  1111  1111  11111    11  11111 11111  111 1",
	" 1    11      1  1111     1  11111    11  11111 11111  1 1 1",
	" 1   11       1     1     1  1  11    11  1  11    11  1  11",
	"111  1111  1111     1  1111  11111    11  11111    11  11111",
	"                                                            ",
	"11111                        11111                          ",
	"11111                        11111                          ",
	"1   1  1111  1111  1111      1     111  1111111  111        ",
	"11111  1  1  1  1  1  1      1 111   1  1  1  1  1 1        ",
	"11111  1  1  1  1  1  1      1 111 111  1  1  1  111        ",
	"1      1  1  1  1  1  1      1  11 1 1  1  1  1  1          ",
	"1      1111  1  1  1111      11111 111  1  1  1  111        ",
	"                      1                                     ",
	"                   1111                                     ",
	"                   1111                                     "
};

static Sprite fonts[NUM_FONTS] = {
	{ '1', 0, 0, 3, 7 },
	{ '2', 5, 0, 4, 7 },
	{ '3', 11, 0, 4, 7 },
	{ '4', 17, 0, 4, 7 },
	{ '5', 23, 0, 4, 7 },
	{ '6', 29, 0, 5, 7 },
	{ '7', 35, 0, 5, 7 },
	{ '8', 42, 0, 5, 7 },
	{ '9', 48, 0, 5, 7 },
	{ '0', 55, 0, 5, 7 },
	{ 'P', 0, 8, 5, 7 },
	{ 'o', 7, 8, 4, 7 },
	{ 'n', 13, 8, 4, 7 },
	{ 'g', 19, 8, 4, 10 },
	{ ' ', 24, 8, 4, 7 },
	{ 'G', 29, 8, 5, 7 },
	{ 'a', 35, 8, 3, 7 },
	{ 'm', 40, 8, 7, 7 },
	{ 'e', 49, 8, 3, 7 }
};

static char GetPixel(int x, int y)
{
	return texture_atlas[y][x];
}

// フォントハンドルを使用して文字列を描画する
void DrawStringToHandle(float x, float y, const char* String, ATTR Color, const CXFont* FontHandle, const char* Str)
{
	switch (FontHandle->type)
	{
	default:
	case CXFONT_DEFAULT:
		DrawString(x, y, String, Color);
		break;
	case CXFONT_PONG:
		float size = 7 / FontHandle->size;
		float font_x = 0;
		float font_y = 0;
		float font_h = 0;
		const char* c;
		for (c = String; *c != '\0'; c++)
		{
			Sprite* sprite = NULL;

			if (*c == '\n')
			{
				font_x = 0;
				font_y += font_h + FONT_SPAN_HEIGHT;
				font_h = 0;
				continue;
			}

			{
				int i;
				for (i = 0; i < NUM_FONTS; i++)
				{
					if (fonts[i].name == *c)
					{
						sprite = &fonts[i];
						break;
					}
				}
			}
			if (sprite != NULL)
			{
				int str_len = GetMax(1, strlen(Str));

				SHORT ix, iy;
				for (iy = 0; WorldYF(iy) < sprite->h / size; iy += 1)
				{
					for (ix = 0; WorldXF(ix) < sprite->w / size; ix += str_len)
					{
						if (GetPixel(sprite->x + (int)(WorldX(ix) * size), sprite->y + (int)(WorldY(iy) * size)) == '1')
							Print({ ConsoleX(font_x / size + x) + ix, ConsoleY(font_y / size + y) + iy }, Color, Str);
					}
				}
			}
			font_x += sprite->w + FONT_SPAN_WIDTH;
			font_h = GetMaxF(font_y, WorldY(sprite->h));
		}
		break;
	}
}
