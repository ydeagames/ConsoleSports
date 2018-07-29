#include "GameController.h"
#include "GameObjects.h"
#include "GameUtils.h"
#include "InputManager.h"

// �֐��̐錾 ==============================================================

void GameController_Player_Update(GameController* ctrl);
void GameController_Player_UpdateControl(GameController* ctrl);
void GameController_Bot_Update(GameController* ctrl);
void GameController_Bot_UpdateControl(GameController* ctrl);

inline float abs(float a)
{
	return a > 0 ? a : -a;
}

// �֐��̒�` ==============================================================

// <<�R���g���[���[>> --------------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Create(GameObject* object, void(*updateFunc)(GameController*), void(*updateCtrlFunc)(GameController*), GameScene* scene, GameObject* enemy)
{
	return { object, updateFunc, updateCtrlFunc, object->pos, scene, enemy };
}

// <�R���g���[���[�X�V>
void GameController_Update(GameController* ctrl)
{
	ctrl->Update(ctrl);
}

// <�R���g���[���[����X�V>
void GameController_UpdateControl(GameController* ctrl)
{
	ctrl->target_pos.y = GameController_GetTargetY(&ctrl->scene->field, &ctrl->scene->ball, ctrl->object, ctrl->enemy);

	ctrl->UpdateControl(ctrl);
}

// <�{�[�����e�_�\�z�A���S���Y��>
float GameController_GetTargetY(GameObject* field, GameObject* ball, GameObject* paddle_myself, GameObject* paddle_enemy)
{
	// �{�[���A�p�h���T�C�Y���l�������G�p�h���A���p�h����X���W
	float enemy_pos_x, myself_pos_x;
	// �{�[�����牽px�Ŏ��p�h���ɓ�������̂����Z�o
	float length_x, length_y;
	// �{�[���̊�ʒu
	float ball_base_y;
	// �ڕW�̍��W
	float ball_absolute_y;
	// �{�[���̈ړ��\�͈�
	float screen_top_y, screen_bottom_y, screen_height;
	// �ڕW�̉�ʓ����W
	float target_pos_y;
	// ���p�h��������{�[���̌��� (1: �E����, -1: ������)
	int k = (paddle_myself->pos.x < paddle_enemy->pos.x) ? 1 : -1;

	// �{�[���A�p�h���T�C�Y���l�������G�p�h���A���p�h����X���W
	{
		myself_pos_x = paddle_myself->pos.x - k * (ball->size.x / 2 + paddle_myself->size.x / 2);
		enemy_pos_x = paddle_enemy->pos.x + k * (ball->size.x / 2 + paddle_enemy->size.x / 2);
	}

	{
		// �{�[�����牽px�Ŏ��p�h���ɓ�������̂����Z�o
		{
			length_x = 0;
			if (k*ball->vel.x > 0)
			{
				// �{�[�����E�ɐi��ł���Ƃ� �������G������
				// �{�[���`�G�܂ł̋��� (�s��)
				length_x += k * (enemy_pos_x - ball->pos.x);
				// �G�`�����̋��� (�A��)
				length_x += k * (enemy_pos_x - myself_pos_x);
			}
			else
			{
				// �{�[�������ɐi��ł���Ƃ� �G������
				// �{�[���`�����܂ł̋���
				length_x += k * (ball->pos.x - myself_pos_x);
			}
		}

		// ���˕Ԃ�𖳎������Ƃ��A���p�h���ɓ������{�[����Y���W
		{
			length_y = length_x / ball->vel.x * ball->vel.y;
			if (length_y < 0)
				length_y *= -1; // ��Βl
		}
	}

	{
		// �{�[���T�C�Y���l�������㉺�̕�
		screen_top_y = GameObject_OffsetY(ball, BOTTOM, GameObject_GetY(field, TOP));
		screen_bottom_y = GameObject_OffsetY(ball, TOP, GameObject_GetY(field, BOTTOM));
		// �{�[���T�C�Y���l�������{�[���̈ړ��͈�
		screen_height = screen_bottom_y - screen_top_y;
	}

	// �{�[����Y���W
	{
		ball_base_y = ball->pos.y - screen_top_y;
		if (ball->vel.y < 0)
			ball_base_y *= -1; // ���x��������̂Ƃ��A��Ƀ^�[�Q�b�g�����݂���
	}

	// �^�[�Q�b�g�̎Z�o
	ball_absolute_y = ball_base_y + length_y;

	// ��ʂ͈̔͊O��������height�������Ĕ͈͓��ɂ���
	// ���̂Ƃ��A����̉񐔂𐔂���
	{
		int count = 0;
		float pos_y_loop = ball_absolute_y;
		while (pos_y_loop < 0)
		{
			pos_y_loop += screen_height;
			count++;
		}
		while (pos_y_loop > screen_height)
		{
			pos_y_loop -= screen_height;
			count++;
		}

		// �͈͂𒲐�����񐔂���ł����Y���𒆐S�ɔ��]������
		if (count % 2 == 0)
			target_pos_y = pos_y_loop;
		else
			target_pos_y = screen_height - pos_y_loop;
	}

	return target_pos_y + screen_top_y;
}

// <<�v���C���[�R���g���[���[>> ----------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Player_Create(GameObject* object, GameScene* scene, GameObject* enemy, int key_up, int key_down)
{
	GameController ctrl = GameController_Create(object, GameController_Player_Update, GameController_Player_UpdateControl, scene, enemy);
	ctrl.player_key_up = key_up;
	ctrl.player_key_down = key_down;
	return ctrl;
}

void GameController_Player_Update(GameController* ctrl)
{
}

// �L�[���͂Ńp�h���𑀍�
void GameController_Player_UpdateControl(GameController* ctrl)
{
	ctrl->object->vel.y = 0.f;
	if (IsKeyDown(ctrl->player_key_up))
		ctrl->object->vel.y += -PADDLE_VEL;
	if (IsKeyDown(ctrl->player_key_down))
		ctrl->object->vel.y += PADDLE_VEL;
}

// <<Bot�R���g���[���[>> -----------------------------------------------

// <�R���g���[���[�쐬>
GameController GameController_Bot_Create(GameObject* object, GameScene* scene, GameObject* enemy)
{
	return  GameController_Create(object, GameController_Bot_Update, GameController_Bot_UpdateControl, scene, enemy);
}

void GameController_Bot_Update(GameController* ctrl)
{
}

// Bot���p�h���𑀍�
void GameController_Bot_UpdateControl(GameController* ctrl)
{
	// Bot�������n�߂邵�����l
	float padding = 40 * BALL_VEL_X_MIN / PADDLE_VEL;

	int k = (ctrl->object->pos.x < ctrl->enemy->pos.x) ? 1 : -1;

	ctrl->object->vel.y = 0.f;

	// �����������������l���߂������瓮��
	if (k * (ctrl->scene->ball.vel.x) < 0 && abs(ctrl->scene->ball.pos.x - ctrl->object->pos.x) < padding)
	{
		// Bot���p�h���𑀍�
		float pos_y = ctrl->target_pos.y;

		// ���񂾂璆���ɖ߂�
		if (k * ctrl->scene->ball.pos.x < k * GameObject_GetX(&ctrl->scene->field, k > 0 ? LEFT : RIGHT))
			pos_y = ClampF(ctrl->scene->ball.pos.y, ctrl->scene->field.pos.y - 2.f, ctrl->scene->field.pos.y + 2.f);

		// Bot���ړ��ł��镝�𐧌�
		//target1_pos_y = ClampF(target1_pos_y, SCREEN_TOP + 50, SCREEN_BOTTOM - 50);

		if (ctrl->object->pos.y - pos_y > .02f * PADDLE_VEL)
			ctrl->object->vel.y += -PADDLE_VEL;
		else if (ctrl->object->pos.y - pos_y < -.02f * PADDLE_VEL)
			ctrl->object->vel.y += PADDLE_VEL;
	}
}

// <Bot�K�C�h�`��>
void GameController_RenderGuide(GameController* ctrl)
{
	GameObject target = *ctrl->object;
	target.pos = ctrl->target_pos;
	target.color = CreateATTR(COLOR_BLACK, COLOR_GRAY);
	GameObject_Render(&target);
}
