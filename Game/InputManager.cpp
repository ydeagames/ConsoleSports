#include "InputManager.h"
#include "Console.h"
#include <time.h>

// �O���[�o���ϐ��̒�` ====================================================

static int g_input_state;
static int s_input_state_last;

static int g_pending_key;
static time_t g_pending_last;

// �֐��̒�` ==============================================================

// �L�[�X�V
void UpdateInputManager(void)
{
	s_input_state_last = g_input_state;
	g_input_state = GetKeyInput();

	if (g_input_state != 0)
	{
		g_pending_key = g_input_state;
		g_pending_last = clock();
	}
	else
	{
		if (clock() - g_pending_last < 300)
			g_input_state = g_pending_key;
	}
}

// �L�[��������Ă��邩
BOOL IsKeyDown(int key)
{
	return g_input_state == key;
}

// �L�[��������Ă��邩
BOOL IsKeyUp(int key)
{
	return !IsKeyDown(key);
}

// �L�[�����������ォ
BOOL IsKeyPressed(int key)
{
	return (s_input_state_last != key) && (g_input_state == key);
}

// �L�[�𗣂������ォ
BOOL IsKeyReleased(int key)
{
	return (s_input_state_last == key) && (g_input_state != key);
}
