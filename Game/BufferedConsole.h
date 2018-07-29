//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   BufferedConsole.h
//!
//! @brief  �\�t�g�E�F�A�E�R���\�[���o�b�t�@
//!
//! @date   2018/07/29
//!
//! @author GF1 26 �R������
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ���d�C���N���[�h�̖h�~ ==================================================
#pragma once

// �w�b�_�t�@�C���̓ǂݍ��� ================================================

#include "Console.h"

// �^�̒�` ================================================================

typedef short SHORT;

// �\���̂̒�` ============================================================

typedef struct {
	SHORT X;
	SHORT Y;
} COORD, *PCOORD;

typedef struct {
	ConsoleColor foreground;
	ConsoleColor background;
} ATTR;

// �O���[�o���ϐ��錾 ======================================================

extern const ATTR DEFAULT_ATTR;

// �֐��̐錾 ==============================================================

ATTR CreateATTR(ConsoleColor foreground, ConsoleColor background);

void BufferedConsole_Initialize(void);

void Clear(void);

void Print(COORD coord, ATTR attributes, const char* format);

void BufferedConsole_Flush(void);
