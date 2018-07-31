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

#include "Game\Game.h"
#include <stdio.h>
#include <time.h>
#include "Game\Console.h"
#include "Game\GameMain.h"
#include "Game\BufferedConsole.h"
#include "Game\InputManager.h"
#include "Game\SpeedTest.h"
#include "Game\GameUtils.h"



// �O���[�o���ϐ���` ======================================================

// �Ō�̎���
static clock_t last_clock;
// 1�t���[���̕b
float delta_seconds;
// �I�����N�G�X�g
static BOOL exit_request = FALSE;



// �֐���` ================================================================

// 1�t���[��
static int ProcessMessage(void)
{
	clock_t now = clock();
	delta_seconds = GetMin(60, now - last_clock) / 1000.f;

	UpdateInputManager();

	return exit_request;
}

// ����ʐ؂�ւ�
static int ScreenFlip(void)
{
	BufferedConsole_Flush();

	return TRUE;
}

// �I�����N�G�X�g
void ExitGame(void)
{
	exit_request = TRUE;
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

	BufferedConsole_Initialize();

	/*
	// �X�s�[�h�e�X�g (�f�o�b�O)
	{
		TestSpeed();
		return 0;
	}
	/**/

	// �Q�[���̏���
	InitializeGame();			// �Q�[���̏���������

	while (!ProcessMessage())
	{
		UpdateGame();			// �Q�[���̍X�V����
		RenderGame();			// �Q�[���̕`�揈��

		ScreenFlip();			// ����ʂ̓��e��\��ʂɔ��f
		Clear();				// ����ʂ̏���
	}

	FinalizeGame();				// �Q�[���̏I������

	return 0;					// ����I��
}
