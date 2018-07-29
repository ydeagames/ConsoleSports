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
#include "InputManager.h"
#include "CXLib.h"




// �萔�̒�` ==============================================================




// �O���[�o���ϐ��̒�` ====================================================
float g_acc_x;
float g_acc_y;
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
		g_acc_x = -1;
	if (IsKeyDown(KEY_RIGHT))
		g_acc_x = 1;
	if (IsKeyDown(KEY_UP))
		g_acc_y = -1;
	if (IsKeyDown(KEY_DOWN))
		g_acc_y = 1;

	g_acc_x *= .001f * delta_seconds;
	g_acc_y *= .001f * delta_seconds;

	g_vel_x += g_acc_x;
	g_vel_y += g_acc_y;

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
	//DrawString(g_pos_x, g_pos_y, /*"**\n**"*/"abcdefg\nhijklmn\ndkuyrgca\nauycgfbag");
	//DrawBox(g_pos_x - 4, g_pos_y - 4, g_pos_x + 4, g_pos_y + 4, CreateATTR(COLOR_BLACK, COLOR_WHITE), TRUE, " ");
	DrawLine(20, 20, g_pos_x, g_pos_y, CreateATTR(COLOR_BLACK, COLOR_WHITE), " ");
	//DrawOval(g_pos_x, g_pos_y, 16, 6, CreateATTR(COLOR_BLACK, COLOR_WHITE), FALSE);
	Print({ 12, 22 }, DEFAULT_ATTR, "���������L�[�ő���");
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
