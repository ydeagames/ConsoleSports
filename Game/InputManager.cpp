#include "InputManager.h"
#include "Console.h"
#include <time.h>

// 定数の定義 ==============================================================

#define KEY_DURATION 475

// グローバル変数の定義 ====================================================

// 入力状態
static int g_input_state;
// 前回の入力状態
static int s_input_state_last;

// 待機中のキー
static int g_pending_key;
// 最後に入力された時間
static time_t g_pending_last;

// 関数の定義 ==============================================================

// キー更新
void UpdateInputManager(void)
{
	// 最後に入力されたキーを更新
	s_input_state_last = g_input_state;
	// 現在入力されたキーを更新
	g_input_state = GetKeyInput();

	// 入力があったら
	if (g_input_state != 0)
	{
		// 待機中のキーを更新
		g_pending_key = g_input_state;
		g_pending_last = clock();
	}
	else
	{
		// 待機中のキーを一定時間押したことにする
		if (clock() - g_pending_last < KEY_DURATION)
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
