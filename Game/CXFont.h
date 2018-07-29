#pragma once
#include "CXLib.h"

typedef enum {
	CXFONT_DEFAULT,
	CXFONT_PONG
} CXFontType;

typedef struct {
	CXFontType type;
	float size;
} CXFont;

// フォントハンドルを使用して文字列を描画する
void DrawStringToHandle(float x, float y, const char* String, ATTR Color, const CXFont* FontHandle, const char* Str = " ");
