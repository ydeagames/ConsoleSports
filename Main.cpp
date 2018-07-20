//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/
//! @file   Main.cpp
//!
//! @brief  ゲームプログラムのエントリーポイントのソースファイル
//!
//! @date   2018/07/18
//!
//! @author GF1 26 山口寛雅
//__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/__/

// ヘッダファイルの読み込み ================================================

#include <stdio.h>
#include <time.h>
#include "Game\Console.h"
#include "Game\GameMain.h"



// グローバル変数定義 ======================================================

int SCREEN_WIDTH;	// 画面の幅[pixel]
int SCREEN_HEIGHT;	// 画面の高さ[pixel]
												

												
// 関数定義 ================================================================

// 1フレーム
static int ProcessMessage(void)
{
	int next_width = GetScreenWidth();
	int next_height = GetScreenHeight();

	printf("でかくなった！\n");
	if (SCREEN_WIDTH != next_width || SCREEN_HEIGHT != next_height)
	{
	}

	SCREEN_WIDTH = next_width;
	SCREEN_HEIGHT = next_height;

	return FALSE;
}

// キーチェック
static int CheckHitKey(int keycode)
{
	return (GetKeyInput() & keycode) != 0;
}

// 裏画面切り替え
static int ScreenFlip(void)
{
	return TRUE;
}

//----------------------------------------------------------------------
//! @brief プログラムのエントリーポイント
//!
//! @retval  0 正常終了
//! @retval -1 異常終了
//----------------------------------------------------------------------
int main(void)
{
	// 初期状態の画面モードの設定
	{
		int screen_width = GetScreenWidth();
		int screen_height = GetScreenHeight();
		int font_old_size = GetFontSize();
		int font_new_size = 5;
		SetFontSize(font_new_size);
		SetScreenSize(screen_width * font_old_size / font_new_size, screen_height * font_old_size / font_new_size);
	}
	SetCursorVisibility(CURSOR_INVISIBLE);

	// ゲームの処理
	InitializeGame();			// ゲームの初期化処理

	while (!ProcessMessage() && !CheckHitKey(KEY_ESC))
	{
		UpdateGame();			// ゲームの更新処理
		RenderGame();			// ゲームの描画処理

		ScreenFlip();			// 裏画面の内容を表画面に反映
		ClearScreen();			// 裏画面の消去
	}

	FinalizeGame();				// ゲームの終了処理

	return 0;					// 正常終了
}
