//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/
//! @file    Console.h
//! @brief   �R���\�[������֘A�̃w�b�_�t�@�C��
//!  
//! @author  S.Takaki
//! @date    2016/7/7
//!
//! @version 1.00
//!
//! Copyright (C) 2016 Susumu Takaki, All rights reserved.
//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/

// ���d�C���N���[�h�̖h�~  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//________________________/_____/_____/_____/_____/_____/_____/_____/_____/
#pragma once




// �萔��`    /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//____________/_____/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @enum  KeyCode
//! @brief ���z�L�[�R�[�h
//===================================================================
enum KeyCode
{
	KEY_BACKSPACE = 0x08,
	KEY_ENTER     = 0x0D, 
	KEY_ESC       = 0x1B,
	KEY_SPACE     = 0x20,
	KEY_UP        = 0x48 | 0x80,
	KEY_LEFT      = 0x4B | 0x80,
	KEY_RIGHT     = 0x4D | 0x80,
	KEY_DOWN      = 0x50 | 0x80,
};


//===================================================================
//! @enum  ConsoleColor
//! @brief �����F����єw�i�F
//===================================================================
enum ConsoleColor
{
	COLOR_BLACK,
	COLOR_DARK_BLUE,
	COLOR_DARK_GREEN,
	COLOR_DARK_CYAN,
	COLOR_DARK_RED,
	COLOR_DARK_MAGENTA,
	COLOR_DARK_YELLOW,
	COLOR_GRAY,
	COLOR_DARK_GRAY,
	COLOR_BLUE,
	COLOR_GREEN,
	COLOR_CYAN,
	COLOR_RED,
	COLOR_MAGENTA,
	COLOR_YELLOW,
	COLOR_WHITE,
};


//===================================================================
//! @enum  CursorVisibility
//! @brief �J�[�\���̕\�����
//===================================================================
enum CursorVisibility
{
	CURSOR_INVISIBLE,
	CURSOR_VISIBLE
};




// �֐��̃v���g�^�C�v�錾  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//________________________/_____/_____/_____/_____/_____/_____/_____/_____/

// �R���\�[������֘A�̊֐�
void SetScreenSize(int screenWidth, int screenHeight);    // ��ʃT�C�Y��ݒ�
int  GetScreenWidth(void);                                // ��ʕ��̎擾
int  GetScreenHeight(void);                               // ��ʍ����̎擾
void SetFontSize(int fontSize);                           // �t�H���g�T�C�Y�̐ݒ�
int  GetFontSize(void);                                   // �t�H���g�T�C�Y�̎擾


// �J�[�\������p�֐�
void SetCursorVisibility(CursorVisibility cursorState);                  // �J�[�\���̕\���E��\���؂�ւ�
void SetCursorPosition(int cursorPositionX, int cursorPositionY);        // �J�[�\���̈ʒu�ݒ�
void GetCursorPosition(int* pCursorPositionX, int* pCursorPositionY);    // �J�[�\���̈ʒu�擾
void MoveCursorPosition(int movementX, int movementY);                   // �J�[�\���̈ړ�


// ���͐���p�֐�
int GetKeyInput(void);                                         // �L�[���͂̎擾
int WaitKey(void);                                             // �L�[���������܂őҋ@
int WaitKeyWithMessage(const char* message);                   // �L�[���������܂őҋ@(���b�Z�[�W�t��)
int WaitKeyWithKeyCode(const int codeList[], int numItems);    // �w��L�[�̓��͑҂�
int InputInteger(void);                                        // �����l�̓���


// �o�͐���p�֐�
void ClearScreen(void);                       // ��ʂ̃N���A
void SetTextColor(ConsoleColor textColor);    // �����F�̐ݒ�
void SetBackColor(ConsoleColor backColor);    // �w�i�F�̐ݒ�
