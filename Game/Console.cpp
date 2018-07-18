//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/
//! @file    Console.cpp
//! @brief   �R���\�[������֘A�̃\�[�X�t�@�C��
//!  
//! @author  S.Takaki
//! @date    2018/7/7
//!
//! @version 1.00
//!
//! Copyright (C) 2018 Susumu Takaki, All rights reserved.
//____/____/____/____/____/____/____/____/____/____/____/____/____/____/____/

// �w�b�_�t�@�C���̃C���N���[�h  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//______________________________/_____/_____/_____/_____/_____/_____/_____/
#include "Console.h"    // �R���\�[������֐�

#include <stdio.h>      // printf()
#include <string.h>     // strlen()
#include <conio.h>      // _kbhit(), _getch(), _putch()
#include <windows.h>    // Windows����֘A




// �\���̂̒�`      /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//__________________/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @brief ��ʂɊ֘A������
//===================================================================
struct tag_ScreenInfo
{
	HANDLE hOutput;           // �o�̓f�o�C�X�̃n���h��
	COORD  bufferSize;        // �o�b�t�@�T�C�Y
	COORD  screenSize;        // ��ʃT�C�Y
	WORD   textAttributes;    // �����̑O�i�F�����Ɣw�i�F����
};
typedef struct tag_ScreenInfo ScreenInfo;




// ����J�֐��̃v���g�^�C�v�錾  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//______________________________/_____/_____/_____/_____/_____/_____/_____/
ScreenInfo GetCurrentConsoleScreenInfo(void);    // �R���\�[����ʂ̏�����
void ResizeScreen(SHORT dx, SHORT dy);           // ��ʂ̃T�C�Y�ύX
int  Clamp(int num, int min, int max);           // �w�肳�ꂽ�͈͓��̐��l��Ԃ�
void ExitWithMessage(const char* message);       // �����I��(���b�Z�[�W�t��)




// �t�@�C���X�R�[�v�ϐ��̐錾    /__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//______________________________/_____/_____/_____/_____/_____/_____/_____/
static ScreenInfo s_screenInfo = GetCurrentConsoleScreenInfo();    // ��ʂɊ֘A������




// ���J�֐��̒�`    /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//__________________/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @brief ��ʃT�C�Y��ݒ�
//!
//! @param[in] screenWidth  ��ʂ̕�
//! @param[in] screenHeight ��ʂ̍���
//!
//! @return �Ȃ�
//===================================================================
void SetScreenSize(int screenWidth, int screenHeight)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	SHORT dx = screenWidth  - s_screenInfo.screenSize.X;    // ���������̍���
	SHORT dy = screenHeight - s_screenInfo.screenSize.Y;    // ���������̍���


	// ���������̃X�P�[�����O ------------------------------
	ResizeScreen(dx, 0);


	// ���������̃X�P�[�����O ------------------------------
	ResizeScreen(0, dy);
}



//===================================================================
//! @brief ��ʕ��̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return ��ʕ�(���p�����P��)
//===================================================================
int GetScreenWidth(void)
{
	return s_screenInfo.screenSize.X;
}



//===================================================================
//! @brief ��ʍ����̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return ��ʍ���(���p�����P��)
//===================================================================
int GetScreenHeight(void)
{
	return s_screenInfo.screenSize.Y;
}



//===================================================================
//! @brief �t�H���g�T�C�Y�̐ݒ�
//!        (���p�̂��߉������͔����ɂȂ�)
//!
//! @param[in] fontSize �t�H���g�T�C�Y[pixel]
//!
//! @return �Ȃ�
//===================================================================
void SetFontSize(int fontSize)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(fontInfo) };    // �t�H���g���


	// ���ݎg�p���̃t�H���g�̎擾 ------------------------
	if (GetCurrentConsoleFontEx(s_screenInfo.hOutput, FALSE, &fontInfo) == 0)
	{
		ExitWithMessage("�t�H���g�Ɋւ�����̎擾�Ɏ��s���܂���");
	}


	// �t�H���g�T�C�Y�̐ݒ� ------------------------------
	fontInfo.dwFontSize.X = (SHORT)fontSize / 2;
	fontInfo.dwFontSize.Y = (SHORT)fontSize;


	// �t�H���g�̍X�V ------------------------------------
	if (SetCurrentConsoleFontEx(s_screenInfo.hOutput, FALSE, &fontInfo) == 0)
	{
		ExitWithMessage("�t�H���g�̍X�V�Ɏ��s���܂���");
	}
}



//===================================================================
//! @brief �t�H���g�T�C�Y(����)�̎擾
//!
//! @param[in] �Ȃ�
//!
//! @return �t�H���g�T�C�Y[pixel]
//===================================================================
int GetFontSize(void)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	CONSOLE_FONT_INFOEX fontInfo = { sizeof(fontInfo) };    // �t�H���g���


	// ���ݎg�p���̃t�H���g�̎擾 ------------------------
	if (GetCurrentConsoleFontEx(s_screenInfo.hOutput, FALSE, &fontInfo) == 0)
	{
		ExitWithMessage("�t�H���g�Ɋւ�����̎擾�Ɏ��s���܂���");
	}


	// �t�H���g�T�C�Y�̕ԋp ------------------------------
	return (int)fontInfo.dwFontSize.Y;
}



//===================================================================
//! @brief �J�[�\���̕\���E��\���؂�ւ�
//!
//! @param[in] cursorState �J�[�\���̕\�����
//!
//! @return �Ȃ�
//===================================================================
void SetCursorVisibility(CursorVisibility cursorState)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	CONSOLE_CURSOR_INFO cursorInfo;    // �J�[�\�����


	// �J�[�\���̕\����Ԃ̕ύX --------------------------
	if (GetConsoleCursorInfo(s_screenInfo.hOutput, &cursorInfo) == 0)
	{
		ExitWithMessage("�J�[�\���Ɋւ�����̎擾�Ɏ��s���܂���");
	}
	cursorInfo.bVisible = cursorState;


	// �J�[�\���̕\����Ԃ̍X�V
	if (SetConsoleCursorInfo(s_screenInfo.hOutput, &cursorInfo) == 0)
	{
		ExitWithMessage("�J�[�\���Ɋւ�����̍X�V�Ɏ��s���܂���");
	}
}



//===================================================================
//! @brief �J�[�\���̈ʒu�ݒ�
//!
//! @param[in] cursorPositionX �J�[�\����X���W
//! @param[in] cursorPositionY �J�[�\����Y���W
//!
//! @return �Ȃ�
//===================================================================
void SetCursorPosition(int cursorPositionX, int cursorPositionY)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	COORD  cursorPosition =    // �J�[�\���̈ʒu���
	{
		(SHORT)cursorPositionX,    // X���W
		(SHORT)cursorPositionY     // Y���W
	};


	// �J�[�\���ʒu�̐ݒ� ---------------------------------
	SetConsoleCursorPosition(s_screenInfo.hOutput, cursorPosition);
}



//===================================================================
//! @brief �J�[�\���̍��W�擾
//!
//! @param[out] pCursorPositionX �J�[�\����X���W���i�[����ϐ��ւ̃|�C���^
//! @param[out] pCursorPositionY �J�[�\����Y���W���i�[����ϐ��ւ̃|�C���^
//!
//! @return �Ȃ�
//===================================================================
void GetCursorPosition(int* pCursorPositionX, int* pCursorPositionY)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;    // �X�N���[���o�b�t�@�Ɋւ�����


	// �X�N���[���o�b�t�@�Ɋւ�����̎擾 --------------
	if (GetConsoleScreenBufferInfo(s_screenInfo.hOutput, &screenBufferInfo) == 0)
	{
		ExitWithMessage("�X�N���[���o�b�t�@�Ɋւ�����̎擾�Ɏ��s���܂���");
	}


	// �J�[�\���ʒu�̏������� ----------------------------
	*pCursorPositionX = screenBufferInfo.dwCursorPosition.X;
	*pCursorPositionY = screenBufferInfo.dwCursorPosition.Y;
}



//===================================================================
//! @brief �J�[�\���̈ړ�
//!
//! @param[in] movementX X�����̈ړ���
//! @param[in] movementY Y�����̈ړ���
//!
//! @return �Ȃ�
//===================================================================
void MoveCursorPosition(int movementX, int movementY)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	int cursorPositionX;    // �J�[�\����X���W
	int cursorPositionY;    // �J�[�\����Y���W


	// �J�[�\���ʒu�̐ݒ� --------------------------------
	GetCursorPosition(&cursorPositionX, &cursorPositionY);
	SetCursorPosition((cursorPositionX + movementX), (cursorPositionY + movementY));
}



//===================================================================
//! @brief �L�[���͂̎擾
//!
//! @param[in] �Ȃ�
//!
//! @retval �L�[�R�[�h �L�[���͂���(����L�[�̏ꍇ�͉��z�L�[�R�[�h��ԋp)
//! @retval 0          �L�[���͂Ȃ�
//===================================================================
int GetKeyInput(void)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	int code = 0;    // ���͂��ꂽ�L�[�̃R�[�h


	// ���͂��ꂽ�L�[�̎擾 ------------------------------
	if (_kbhit())
	{
		code = _getch();    // ���̓L�[�̎擾

		if (code == 0xE0 || code == 0x00)    // ���ꕶ���̏ꍇ
		{
    		code = _getch() | 0x80;
		}
	}


	// �����R�[�h�̕ԋp ----------------------------------
	return code;
}



//===================================================================
//! @brief �L�[���������܂őҋ@
//!
//! @param[in] �Ȃ�
//!
//! @return ���͂��ꂽ�L�[�̃R�[�h��ԋp
//===================================================================
int WaitKey(void)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	int code;    // ���͂��ꂽ�L�[�̃R�[�h


	// ���͏��� ------------------------------------------
	while (1)
	{
		code = GetKeyInput();    // ���̓L�[�̎擾

		// ���͏����̍��v���� --------------------------------
		if (code != 0)
		{
			return code;
		}
	}
}



//===================================================================
//! @brief �L�[���������܂őҋ@(���b�Z�[�W�t��)
//!
//! @param[in] //! @param[in] message �o�̓��b�Z�[�W
//!
//! @return ���͂��ꂽ�L�[�̃R�[�h��ԋp
//===================================================================
int WaitKeyWithMessage(const char* message)
{
	// ���b�Z�[�W�̕\��
	printf("%s", message);


	// ���͏��� ------------------------------------------
	return WaitKey();
}



//===================================================================
//! @brief �w�肳�ꂽ�L�[���������܂őҋ@
//!
//! @param[in] codeList �L�[�R�[�h�̃��X�g
//! @param[in] numItems ���X�g�̃A�C�e����
//!
//! @return �I�����̒�������͂��ꂽ�L�[�̃R�[�h��ԋp
//===================================================================
int WaitKeyWithKeyCode(const int codeList[], int numItems)
{
	// ���[�J���ϐ��̐錾 --------------------------------
    int code;    // ���͂��ꂽ�L�[�̃R�[�h
    int i;       // ���[�v�p

	
	// ���͏��� ------------------------------------------
    while (1)
	{
	    code = GetKeyInput();    // ���̓L�[�̎擾

	    // ���͏����̍��v���� --------------------------------
		if (code != 0)
		{
			for (i = 0; i < numItems; i++)
			{
				if (code == codeList[i])
				{
					return codeList[i];
				}
			}
		}
    }
}



//===================================================================
//! @brief �����l�̓���
//!
//! @param[in] �Ȃ�
//!
//! @return ���͂��ꂽ�����l��ԋp
//===================================================================
int InputInteger(void)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	int digit  = 0;    // ��
    int number = 0;    // ���͒l
	int code;          // ���͂��ꂽ�L�[�̃R�[�h
	

    // ���͏��� ------------------------------------------
    while (1)
	{
	    code = GetKeyInput();    // ���̓L�[�̎擾

		if (code >= '0' && code <= '9')
		{
			digit++;
			_putch(code);
			number = number * 10 + ( code - '0' );
		}
		else if (code == KEY_ENTER)
		{
			_putch('\n');
			break;
		}
		else if (code == KEY_BACKSPACE)
		{
			if (digit > 0)
			{
				digit--;
				number /= 10;
				_putch(KEY_BACKSPACE);
				_putch(KEY_SPACE);
				_putch(KEY_BACKSPACE);
			}
		}
    }


	// ���͂��ꂽ�����l��ԋp ----------------------------
	return number;
}



//===================================================================
//! @brief ��ʂ̃N���A
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//===================================================================
void ClearScreen(void)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	DWORD length     = s_screenInfo.bufferSize.X * s_screenInfo.bufferSize.Y;    // �������ޕ�����
	COORD writeCoord = {0, 0};                                                   // �������������ރX�N���[���o�b�t�@���W
	DWORD numCharactersWritten;                                                  // �X�N���[���o�b�t�@�Ɏ��ۂɏ������܂ꂽ������


	// ��ʂ��N���A --------------------------------------
	FillConsoleOutputCharacter(s_screenInfo.hOutput, ' ', length, writeCoord, &numCharactersWritten);
	FillConsoleOutputAttribute(s_screenInfo.hOutput, s_screenInfo.textAttributes, length, writeCoord, &numCharactersWritten);

	SetCursorPosition(0, 0);
}



//===================================================================
//! @brief �����F�̐ݒ�
//!
//! @param[in] textColor �����F
//!
//! @return �Ȃ�
//===================================================================
void SetTextColor(ConsoleColor textColor)
{
	// �����F�̕ύX--------------------------------------
	s_screenInfo.textAttributes = (s_screenInfo.textAttributes & 0xF0) | textColor;
    

	// �����̑����̍X�V ----------------------------------
	if (SetConsoleTextAttribute(s_screenInfo.hOutput, s_screenInfo.textAttributes) == 0)
	{
		ExitWithMessage("�����F�̕ύX�Ɏ��s���܂����B");
	}
}



//===================================================================
//! @brief �w�i�F�̐ݒ�
//!
//! @param[in] backColor �w�i�F
//!
//! @return �Ȃ�
//===================================================================
void SetBackColor(ConsoleColor backColor)
{
	// �w�i�F�̕ύX --------------------------------------
	s_screenInfo.textAttributes = (s_screenInfo.textAttributes & 0x0F) | (backColor << 4);


	// �����̑����̍X�V ----------------------------------
	if (SetConsoleTextAttribute(s_screenInfo.hOutput, s_screenInfo.textAttributes) == 0)
	{
		ExitWithMessage("�w�i�F�̕ύX�Ɏ��s���܂����B");
	}
}




// ����J�֐��̒�`  /__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//__________________/_____/_____/_____/_____/_____/_____/_____/_____/_____/

//===================================================================
//! @brief ���݂̃R���\�[����ʂɊւ�������擾
//!
//! @param[in] �Ȃ�
//!
//! @return ���݂̃R���\�[����ʂɊւ�����
//===================================================================
ScreenInfo GetCurrentConsoleScreenInfo(void)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	HANDLE                     hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);    // �W���o�̓f�o�C�X�̃n���h��
	CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;                                // �X�N���[���o�b�t�@�Ɋւ�����
	ScreenInfo                 screenInfo;                                      // �R���\�[����ʂɊւ�����
	

	// �X�N���[���o�b�t�@�Ɋւ�����̎擾 --------------
	if (GetConsoleScreenBufferInfo(hStdOutput, &screenBufferInfo) == 0)
	{
		ExitWithMessage("�X�N���[���o�b�t�@�Ɋւ�����̎擾�Ɏ��s���܂���");
	}


	// �R���\�[����ʂɊւ�����̐ݒ� ------------------
	screenInfo.hOutput        = hStdOutput;
	screenInfo.bufferSize     = screenBufferInfo.dwSize;
	screenInfo.screenSize.X   = screenBufferInfo.srWindow.Right  + 1;
	screenInfo.screenSize.Y   = screenBufferInfo.srWindow.Bottom + 1;
	screenInfo.textAttributes = screenBufferInfo.wAttributes;


	// �R���\�[����ʂɊւ�����̕ԋp ------------------
	return screenInfo;
}



//===================================================================
//! @brief ��ʂ̊g��E�k��
//!
//! @param[in] dx ���������̍���
//! @param[in] dy ���������̍���
//!
//! @return �Ȃ�
//===================================================================
void ResizeScreen(SHORT dx, SHORT dy)
{
	// ���[�J���ϐ��̐錾 --------------------------------
	COORD  screenBufferSize =    // �X�N���[���o�b�t�@�̃T�C�Y
	{
		s_screenInfo.screenSize.X + dx,    // ��
		s_screenInfo.screenSize.Y + dy     // ����
	};

	SMALL_RECT windowRect =    // �E�B���h�E��`
	{
		0,                         // ��
		0,                         // ��
		screenBufferSize.X - 1,    // �E
		screenBufferSize.Y - 1     // ��
	};


	// ��ʂ̃T�C�Y�ύX ----------------------------------
	if (dx > 0 || dy > 0)
	{
		// �g��̏ꍇ --------------------------------
		if (!SetConsoleScreenBufferSize(s_screenInfo.hOutput, screenBufferSize))
		{
			ExitWithMessage("�X�N���[���o�b�t�@�̃T�C�Y�̕ύX�Ɏ��s���܂����B");
		}

		if (!SetConsoleWindowInfo(s_screenInfo.hOutput, TRUE, &windowRect))
		{
			ExitWithMessage("�E�B���h�E�̃T�C�Y�̕ύX�Ɏ��s���܂����B");
		}
	}
	else
	{
		// �k���̏ꍇ --------------------------------
		if (!SetConsoleWindowInfo(s_screenInfo.hOutput, TRUE, &windowRect))
		{
			ExitWithMessage("�E�B���h�E�̃T�C�Y�̕ύX�Ɏ��s���܂����B");
		}

		if (!SetConsoleScreenBufferSize(s_screenInfo.hOutput, screenBufferSize))
		{
			ExitWithMessage("�X�N���[���o�b�t�@�̃T�C�Y�̕ύX�Ɏ��s���܂����B");
		}
	}


	// ��ʂɊւ�������̃T�C�Y�����X�V
	s_screenInfo.screenSize = screenBufferSize;
	s_screenInfo.bufferSize = screenBufferSize;
}



//===================================================================
//! @brief �w�肳�ꂽ�͈͓��̐��l��Ԃ�
//!
//! @param[in] num ���肷�鐔�l
//! @param[in] min �͈͂̍ŏ��l
//! @param[in] max �͈͂̍ő�l
//!
//! @return �w�肳�ꂽ�͈͓��̐��l
//===================================================================
int Clamp(int num, int min, int max)
{
	return (num < min) ? min : ((num < max) ? num : max);
}



//===================================================================
//! @brief �����I��(���b�Z�[�W�t��)
//!
//! @param[in] message �o�̓��b�Z�[�W
//!
//! @return �Ȃ�
//===================================================================
void ExitWithMessage(const char* message)
{
	WaitKeyWithMessage(message);

	exit(-1);
}
