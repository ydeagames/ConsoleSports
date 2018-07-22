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




// �֐���` ================================================================

// 1�t���[��
static int ProcessMessage(void)
{
	printf("�ł����Ȃ����I\n");

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
	SetFontSize(SCREEN_FONT_SIZE);
	SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
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
