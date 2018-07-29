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

// �R���\�[�����W�̗v�f
typedef short SHORT;

// �\���̂̒�` ============================================================

// �R���\�[�����W
typedef struct {
	SHORT X;
	SHORT Y;
} COORD, *PCOORD;

// ����(�F)
typedef struct {
	ConsoleColor foreground;
	ConsoleColor background;
} ATTR;

// �O���[�o���ϐ��錾 ======================================================

// �f�t�H���g����(�F)
extern const ATTR DEFAULT_ATTR;

// �֐��̐錾 ==============================================================

// ����(�F)���쐬
ATTR CreateATTR(ConsoleColor foreground, ConsoleColor background);

// �\�t�g�E�F�A�E�R���\�[���o�b�t�@������������
void BufferedConsole_Initialize(void);

// ��ʂ�����������
void Clear(void);

// �w����W�Ɏw�蕶�����w�葮��(�F)�ŕ\��
void Print(COORD coord, ATTR attributes, const char* format);

// �X�N���[���𔽉f
void BufferedConsole_Flush(void);
