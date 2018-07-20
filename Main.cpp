//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Main.cpp
//!
//! @brief  �Q�[���v���O�����̃G���g���[�|�C���g�̃\�[�X�t�@�C��
//!
//! @date   2018/07/18
//!
//! @author GF1 26 �R������
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================

#include <stdio.h>
#include <time.h>
#include "Game\Console.h"
#include "Game\GameMain.h"



// �O���[�o���ϐ���` ======================================================

int SCREEN_WIDTH;	// ��ʂ̕�[pixel]
int SCREEN_HEIGHT;	// ��ʂ̍���[pixel]
												

												
// �֐���` ================================================================

// 1�t���[��
static int ProcessMessage(void)
{
	int next_width = GetScreenWidth();
	int next_height = GetScreenHeight();

	printf("�ł����Ȃ����I\n");
	if (SCREEN_WIDTH != next_width || SCREEN_HEIGHT != next_height)
	{
	}

	SCREEN_WIDTH = next_width;
	SCREEN_HEIGHT = next_height;

	return FALSE;
}

// �L�[�`�F�b�N
static int CheckHitKey(int keycode)
{
	return (GetKeyInput() & keycode) != 0;
}

// ����ʐ؂�ւ�
static int ScreenFlip(void)
{
	return TRUE;
}

//----------------------------------------------------------------------
//! @brief �v���O�����̃G���g���[�|�C���g
//!
//! @retval  0 ����I��
//! @retval -1 �ُ�I��
//----------------------------------------------------------------------
int main(void)
{
	// ������Ԃ̉�ʃ��[�h�̐ݒ�
	{
		int screen_width = GetScreenWidth();
		int screen_height = GetScreenHeight();
		int font_old_size = GetFontSize();
		int font_new_size = 5;
		SetFontSize(font_new_size);
		SetScreenSize(screen_width * font_old_size / font_new_size, screen_height * font_old_size / font_new_size);
	}
	SetCursorVisibility(CURSOR_INVISIBLE);

	// �Q�[���̏���
	InitializeGame();			// �Q�[���̏���������

	while (!ProcessMessage() && !CheckHitKey(KEY_ESC))
	{
		UpdateGame();			// �Q�[���̍X�V����
		RenderGame();			// �Q�[���̕`�揈��

		ScreenFlip();			// ����ʂ̓��e��\��ʂɔ��f
		ClearScreen();			// ����ʂ̏���
	}

	FinalizeGame();				// �Q�[���̏I������

	return 0;					// ����I��
}
