#include "InputManager.h"
#include "Console.h"
#include <time.h>

// グローバル変数の定義 ====================================================

static int g_input_state;
static int s_input_state_last;

static int g_pending_key;
static time_t g_pending_last;

// 関数の定義 ==============================================================

// キー更新
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

// キーが押されているか
BOOL IsKeyDown(int key)
{
	return g_input_state == key;
}

// キーが離されているか
BOOL IsKeyUp(int key)
{
	return !IsKeyDown(key);
}

// キーを押した直後か
BOOL IsKeyPressed(int key)
{
	return (s_input_state_last != key) && (g_input_state == key);
}

// キーを離した直後か
BOOL IsKeyReleased(int key)
{
	return (s_input_state_last == key) && (g_input_state != key);
}
