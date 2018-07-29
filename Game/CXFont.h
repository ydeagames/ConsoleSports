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

// �t�H���g�n���h�����g�p���ĕ������`�悷��
void DrawStringToHandle(float x, float y, const char* String, ATTR Color, const CXFont* FontHandle, const char* Str = " ");
