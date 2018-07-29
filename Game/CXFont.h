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

// �t�H���g�n���h�����쐬����
CXFont CreateFontToHandle(CXFontType Font, int Size);

// ������`��֐�
void DrawString(float x, float y, const char* String, ATTR Color = DEFAULT_ATTR);

// �t�H���g�n���h�����g�p����������̕`�敝(���[���h���W)���擾����
float GetDrawStringWidthToHandle(const char* String, const CXFont* FontHandle);

// �t�H���g�n���h�����g�p���ĕ������`�悷��
void DrawStringToHandle(float x, float y, const char* String, ATTR Color, const CXFont* FontHandle, const char* Str = " ");
