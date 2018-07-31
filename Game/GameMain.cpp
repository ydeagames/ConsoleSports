//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   GameMain.cpp
//!
//! @brief  Pong Online! �I���W�i���ۑ�
//!
//! @date   2018/06/13
//!
//! @author GF1 26 �R������
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// �w�b�_�t�@�C���̓ǂݍ��� ================================================
#include "GameMain.h"
#include "Game.h"
#include <stdio.h>
#include "GameObject.h"
#include "GameObjects.h"
#include "GameControllers.h"
#include "GameScore.h"
#include "GameUtils.h"
#include "InputManager.h"


// �萔�̒�` ==============================================================

// <�_���K�C�h> --------------------------------------------------------
#define SCORE_TO_GUID 2

// <�T�[�u�ҋ@> --------------------------------------------------------
#define SERVE_WAIT_TIME 2*60


// �O���[�o���ϐ��̒�` ====================================================

// <�V�[��> ------------------------------------------------------------
GameScene g_scene;

// <�R���g���[���[> ----------------------------------------------------
GameControllers g_controllers;

// <�t�H���g> ----------------------------------------------------------
CXFont g_font_pong;

// <�|�[�Y> ------------------------------------------------------------
BOOL g_paused;
int g_pause_select;


// �֐��̐錾 ==============================================================

// <�Q�[���̍X�V����:�V�[��> -------------------------------------------
void UpdateGameSceneDemo(void);
void UpdateGameSceneServe(void);
void UpdateGameScenePlay(void);

// <�Q�[���̕`�揈��> --------------------------------------------------
void RenderGameSceneDemo(void);
void RenderGameSceneServe(void);
void RenderGameScenePlay(void);

// <�Q�[���̍X�V����:�X�R�A���Z>
void UpdateGameScore(ObjectSide side);
// <�Q�[���̏���������:�|�[�Y>
void InitializeGamePause(void);
// <�Q�[���̍X�V����:�|�[�Y>
void UpdateGamePause(void);
// <�Q�[���̕`�揈��:�|�[�Y>
void RenderGamePause(void);

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
	// �V�[�����
	g_scene.game_state = STATE_DEMO;

	// �t�B�[���h
	g_scene.field = GameObject_Field_Create();

	// �{�[��
	g_scene.ball = GameObject_Ball_Create();
	g_scene.ball.color = CreateATTR(COLOR_BLACK, COLOR_YELLOW);
	GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetPosYDefault(&g_scene.ball, &g_scene.field);
	GameObject_Ball_SetVelXDefault(&g_scene.ball);
	GameObject_Ball_SetVelYDefault(&g_scene.ball);

	// �p�h��1
	g_scene.paddle1 = GameObject_Paddle_Create();
	g_scene.paddle1.color = CreateATTR(COLOR_BLACK, COLOR_RED);
	g_scene.paddle1.pos.x = GameObject_OffsetX(&g_scene.paddle1, LEFT, GameObject_GetX(&g_scene.field, LEFT), -12);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle1, &g_scene.field);
	g_controllers.paddle1 = GameController_Bot_Create(&g_scene.paddle1, &g_scene, &g_scene.paddle2);

	// �p�h��2
	g_scene.paddle2 = GameObject_Paddle_Create();
	g_scene.paddle2.color = CreateATTR(COLOR_BLACK, COLOR_BLUE);
	g_scene.paddle2.pos.x = GameObject_OffsetX(&g_scene.paddle2, RIGHT, GameObject_GetX(&g_scene.field, RIGHT), -12);
	GameObject_Paddle_SetPosYDefault(&g_scene.paddle2, &g_scene.field);
	g_controllers.paddle2 = GameController_Player_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1, KEY_UP, KEY_DOWN);
	//g_controllers.paddle2 = GameController_Bot_Create(&g_scene.paddle2, &g_scene, &g_scene.paddle1);

	// �t�H���g
	g_font_pong = CreateFontToHandle(CXFONT_PONG, 10);

	// ���_
	g_scene.score = GameScore_Create();

	// �T�[�u�ҋ@
	g_scene.counter = 0;
}

// <�Q�[���̏���������:�|�[�Y>
void InitializeGamePause(void)
{
	g_paused = TRUE;
	g_pause_select = 0;
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
	switch (g_scene.game_state)
	{
	case STATE_DEMO:
		UpdateGameSceneDemo();
		break;
	case STATE_SERVE:
		UpdateGameSceneServe();
		break;
	case STATE_PLAY:
		UpdateGameScenePlay();
		break;
	}
}

// <�Q�[���̍X�V����:�V�[��:�f��> --------------------------------------
void UpdateGameSceneDemo(void)
{
	// Esc�������ꂽ��I��
	if (IsKeyDown(KEY_ESC))
		ExitGame();

	// �ҋ@&������
	{
		// ���͂��ꂽ��
		if (IsKeyDown(KEY_SPACE))
		{
			// �_�����Z�b�g
			GameScore_Clear(&g_scene.score);

			// X���W����ʒ����֖߂�
			GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

			// �p�h���������ʒu��
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle1, &g_scene.field);
			GameObject_Paddle_SetPosYDefault(&g_scene.paddle2, &g_scene.field);

			// �V�[�����v���C�ɕύX
			g_scene.game_state = STATE_PLAY;
		}
	}

	// �R���g���[���[�X�V
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.y *= -1;
	if (GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.x *= -1;
}

// <�Q�[���̍X�V����:�V�[��:�T�[�u> ------------------------------------
void UpdateGameSceneServe(void)
{
	// Esc�������ꂽ��|�[�Y
	if (IsKeyDown(KEY_ESC))
		InitializeGamePause();
	// �|�[�Y���Ȃ�|�[�Y����
	if (g_paused)
	{
		UpdateGamePause();
		return;
	}

	// �ҋ@&������
	g_scene.counter++;

	// ���Ԍo�߂�
	if (g_scene.counter >= SERVE_WAIT_TIME)
	{
		// X���W����ʒ����֖߂�
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);

		// �V�[�����v���C�ɕύX
		g_scene.game_state = STATE_PLAY;

		g_scene.counter = 0;
	}

	// �R���g���[���[�X�V
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// ����
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
		g_scene.ball.vel.y *= -1;
	GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball);
	GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, CONNECTION_BARRIER, EDGESIDE_INNER);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, CONNECTION_BARRIER, EDGESIDE_INNER);
}

// <�Q�[���̍X�V����:�V�[��:�v���C> ------------------------------------
void UpdateGameScenePlay(void)
{
	// Esc�������ꂽ��|�[�Y
	if (IsKeyDown(KEY_ESC))
		InitializeGamePause();
	// �|�[�Y���Ȃ�|�[�Y����
	if (g_paused)
	{
		UpdateGamePause();
		return;
	}

	// �R���g���[���[�X�V
	GameController_Update(&g_controllers.paddle1);
	GameController_Update(&g_controllers.paddle2);

	// ����
	GameController_UpdateControl(&g_controllers.paddle1);
	GameController_UpdateControl(&g_controllers.paddle2);

	// ���W�X�V
	GameObject_UpdatePosition(&g_scene.ball);
	GameObject_UpdatePosition(&g_scene.paddle1);
	GameObject_UpdatePosition(&g_scene.paddle2);

	// �����蔻��
	if (GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.ball, CONNECTION_BARRIER, EDGESIDE_INNER))
	{
		g_scene.ball.vel.y *= -1;
	}
	{
		ObjectSide side = GameObject_Field_CollisionHorizontal(&g_scene.field, &g_scene.ball, CONNECTION_NONE, EDGESIDE_OUTER);
		if (side)
		{
			UpdateGameScore(side);
		}
	}
	GameObject_Paddle_CollisionBall(&g_scene.paddle1, &g_scene.ball);
	GameObject_Paddle_CollisionBall(&g_scene.paddle2, &g_scene.ball);

	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle1, CONNECTION_BARRIER, EDGESIDE_INNER);
	GameObject_Field_CollisionVertical(&g_scene.field, &g_scene.paddle2, CONNECTION_BARRIER, EDGESIDE_INNER);
}

// <�Q�[���̍X�V����:�|�[�Y>
void UpdateGamePause(void)
{
	if (IsKeyPressed(KEY_UP))
		g_pause_select = GetLoop(g_pause_select - 1, 3);
	if (IsKeyPressed(KEY_DOWN))
		g_pause_select = GetLoop(g_pause_select + 1, 3);
	if (IsKeyPressed(KEY_SPACE))
	{
		switch (g_pause_select)
		{
		default:
		case 0:
			g_paused = FALSE;
			break;
		case 1:
			g_paused = FALSE;
			// �I������
			{
				GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
				GameObject_Ball_SetVelXDefault(&g_scene.ball);
				GameObject_Ball_SetVelYDefault(&g_scene.ball);

				// �V�[�����f���ɕύX
				g_scene.game_state = STATE_DEMO;
			}
			break;
		case 2:
			ExitGame();
			break;
		}
	}
}

// <�Q�[���̍X�V����:�X�R�A���Z>
void UpdateGameScore(ObjectSide side)
{
	// ���_����
	GameScore_Add(&g_scene.score, side);

	if (GameScore_IsFinished(&g_scene.score))
	{
		GameObject_Ball_SetPosXDefault(&g_scene.ball, &g_scene.field);
		GameObject_Ball_SetVelXDefault(&g_scene.ball);
		GameObject_Ball_SetVelYDefault(&g_scene.ball);

		// �V�[�����f���ɕύX
		g_scene.game_state = STATE_DEMO;
	}
	else
	{
		// �V�[�����T�[�u�ɕύX
		g_scene.game_state = STATE_SERVE;
	}
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
	switch (g_scene.game_state)
	{
	case STATE_DEMO:
		RenderGameSceneDemo();
		break;
	case STATE_SERVE:
		RenderGameSceneServe();
		break;
	case STATE_PLAY:
		RenderGameScenePlay();
		break;
	}
}

// <�Q�[���̕`�揈��:�V�[��:�f��> ---------------------------------------------
void RenderGameSceneDemo(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_scene.field);
	// �{�[���`��
	GameObject_Render(&g_scene.ball);
	// ���j���[�`��
	{
		float width = GetDrawStringWidthToHandle("Pong Game", &g_font_pong);
		//DrawBox(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) - 9, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 7, CreateATTR(COLOR_BLACK, COLOR_BLACK), TRUE);
		DrawLine(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) + 5, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 5, CreateATTR(COLOR_BLACK, COLOR_DARK_GREEN));
		DrawStringToHandle(GameObject_GetX(&g_scene.field, CENTER_X) - width / 2 - 10, GameObject_GetY(&g_scene.field, CENTER_Y) - 7, "Pong Game", CreateATTR(COLOR_BLACK, COLOR_CYAN), &g_font_pong);
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) + 20, GameObject_GetY(&g_scene.field, CENTER_Y) + 7, "�`���g���ȑ싅�X�|�[�c�Q�[���`", CreateATTR(COLOR_YELLOW, COLOR_BLACK));

		DrawBox(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) + 14, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 28, CreateATTR(COLOR_BLACK, COLOR_BLACK), TRUE);
		DrawBox(GameObject_GetX(&g_scene.field, LEFT, -39), GameObject_GetY(&g_scene.field, CENTER_Y) + 14, GameObject_GetX(&g_scene.field, RIGHT, -38), GameObject_GetY(&g_scene.field, CENTER_Y) + 28, CreateATTR(COLOR_BLACK, COLOR_DARK_MAGENTA), FALSE);

		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 10, GameObject_GetY(&g_scene.field, CENTER_Y) + 17, "�X�y�[�X�L�[�������ăQ�[�����n�߂悤�I", CreateATTR(COLOR_YELLOW, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 11, GameObject_GetY(&g_scene.field, CENTER_Y) + 19, "�����L�[�ŉE�p�h���𑀍삵��CPU��|�����I", CreateATTR(COLOR_MAGENTA, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 11, GameObject_GetY(&g_scene.field, CENTER_Y) + 21, "�Z�������Ȃ��̂��߂�5�_�}�b�`�ɂ��܂����I", CreateATTR(COLOR_GREEN, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 10, GameObject_GetY(&g_scene.field, CENTER_Y) + 23, "2�_�������ƁA�K�C�h���\�������YO�I", CreateATTR(COLOR_DARK_BLUE, COLOR_BLACK));
		DrawString(GameObject_GetX(&g_scene.field, CENTER_X) - 14, GameObject_GetY(&g_scene.field, CENTER_Y) + 25, "Pong Game�����đ싅�Ƃ������h�ȃX�|�[�c�Q�[���_�]�I", CreateATTR(COLOR_DARK_GRAY, COLOR_BLACK));

		DrawString(GameObject_GetX(&g_scene.field, RIGHT, -17), GameObject_GetY(&g_scene.field, BOTTOM, -3), "Copyrights 2018 YdeaGames", CreateATTR(COLOR_GRAY, COLOR_BLACK));
	}
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, &g_scene.field, g_font_pong);
}

// <�Q�[���̕`�揈��:�V�[��:�T�[�u> -------------------------------------------
void RenderGameSceneServe(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_scene.field);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, &g_scene.field, g_font_pong);
	// �p�h���`��
	GameObject_Render(&g_scene.paddle1);
	GameObject_Render(&g_scene.paddle2);
	// �{�[���`��
	GameObject_Render(&g_scene.ball);

	// �|�[�Y���Ȃ�|�[�Y����
	if (g_paused)
		RenderGamePause();
}

// <�Q�[���̕`�揈��:�V�[��:�v���C> -------------------------------------------
void RenderGameScenePlay(void)
{
	// <�I�u�W�F�N�g�`��>
	// �t�B�[���h�`��
	GameObject_Field_Render(&g_scene.field);
	// �X�R�A�`��
	GameScore_Render(&g_scene.score, &g_scene.field, g_font_pong);
	// �K�C�h�`��
	if (g_scene.score.score2 - g_scene.score.score1 >= SCORE_TO_GUID)
		GameController_RenderGuide(&g_controllers.paddle1);
	if (g_scene.score.score1 - g_scene.score.score2 >= SCORE_TO_GUID)
		GameController_RenderGuide(&g_controllers.paddle2);
	// �p�h���`��
	GameObject_Render(&g_scene.paddle1);
	GameObject_Render(&g_scene.paddle2);
	// �{�[���`��
	GameObject_Render(&g_scene.ball);

	// �|�[�Y���Ȃ�|�[�Y����
	if (g_paused)
		RenderGamePause();
}

// <�Q�[���̕`�揈��:�|�[�Y>
void RenderGamePause(void)
{
	char str[20];
	snprintf(str, 20, "[ %d ]", g_pause_select);
	DrawString(2, 2, str, DEFAULT_ATTR);
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
	// ���擾
	CXFont font = { CXFONT_DEFAULT, SCREEN_FONT_SIZE };
	float widththanks = GetDrawStringWidthToHandle("Thank You!", &g_font_pong);
	float widthmsg = GetDrawStringWidthToHandle("�v���C�����������肪�Ƃ��������܂����B", &font);
	float widthwait = GetDrawStringWidthToHandle("���s����ɂ͉����L�[�������Ă������� . . .", &font);
	// ��ʂ��N���A
	Clear();
	// �I�����
	DrawStringToHandle(WORLD_CENTER_X - widththanks / 2 - 10, WORLD_CENTER_Y - 10, "Thank You!", CreateATTR(COLOR_BLACK, COLOR_CYAN), &g_font_pong);
	DrawStringToHandle(WORLD_CENTER_X - widthmsg / 2, WORLD_CENTER_Y + 5, "�v���C�����������肪�Ƃ��������܂����B", CreateATTR(COLOR_YELLOW, COLOR_BLACK), &font);
	BufferedConsole_Flush();
	// ��n��
	SetTextColor(COLOR_DARK_GRAY);
	SetBackColor(COLOR_BLACK);
	SetCursorPosition(SCREEN_RIGHT - WorldToConsoleX(widthwait) - 2, SCREEN_BOTTOM - 2);
}
