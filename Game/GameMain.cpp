//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  �Q�[���֘A�̃\�[�X�t�@�C��
//!
//! @date   2018/07/18
//!
//! @author GF1 26 �R�� ����
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "GameMain.h"
#include "BufferedConsole.h"
#include "InputManager.h"




// �萔�̒�` ==============================================================




// �O���[�o���ϐ��̒�` ====================================================
float g_vel_x;
float g_vel_y;
float g_pos_x;
float g_pos_y;



// �֐��̐錾 ==============================================================

void InitializeGame(void);  // �Q�[���̏���������
void UpdateGame(void);      // �Q�[���̍X�V����
void RenderGame(void);      // �Q�[���̕`�揈��
void FinalizeGame(void);    // �Q�[���̏I������




// �֐��̒�` ==============================================================

//----------------------------------------------------------------------
//! @brief �Q�[���̏���������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void InitializeGame(void)
{
	g_vel_x = 0;
	g_vel_y = 0;
	g_pos_x = WORLD_CENTER_X;
	g_pos_y = WORLD_CENTER_Y;
}



//----------------------------------------------------------------------
//! @brief �Q�[���̍X�V����
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void UpdateGame(void)
{
	if (IsKeyDown(KEY_LEFT))
		g_vel_x += -.001f;
	if (IsKeyDown(KEY_RIGHT))
		g_vel_x += .001f;
	if (IsKeyDown(KEY_UP))
		g_vel_y += -.001f;
	if (IsKeyDown(KEY_DOWN))
		g_vel_y += .001f;

	g_vel_x *= 0.9998f;
	g_vel_y *= 0.9998f;

	g_pos_x += g_vel_x;
	g_pos_y += g_vel_y;

	if (g_pos_x < WORLD_LEFT || WORLD_RIGHT <= g_pos_x)
		g_vel_x *= -1;
	if (g_pos_y < WORLD_TOP || WORLD_BOTTOM <= g_pos_y)
		g_vel_y *= -1;
	g_pos_x = CLAMP(g_pos_x, WORLD_LEFT, WORLD_RIGHT);
	g_pos_y = CLAMP(g_pos_y, WORLD_TOP, WORLD_BOTTOM);
}




//----------------------------------------------------------------------
//! @brief �Q�[���̕`�揈��
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void RenderGame(void)
{
	Print({ (SHORT)(g_pos_x * SCREEN_RESOLUTION_X), (SHORT)(g_pos_y * SCREEN_RESOLUTION_Y) }, DEFAULT_ATTRIBUTES, /*"**\n**"*/"abcdefg\nhijklmn\ndkuyrgca\nauycgfbag");
	Print({ 12, 22 }, DEFAULT_ATTRIBUTES, "���������L�[�ő���");
}



//----------------------------------------------------------------------
//! @brief �Q�[���̏I������
//!
//! @param[in] �Ȃ�
//!
//! @return �Ȃ�
//----------------------------------------------------------------------
void FinalizeGame(void)
{

}
