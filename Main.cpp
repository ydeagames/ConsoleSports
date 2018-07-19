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
#include "Game\Console.h"
#include <windows.h>
#include "Game\GameMain.h"




// 関数定義 ================================================================

// 1フレーム
static int ProcessMessage(void)
{
	Sleep(17);

	return TRUE;
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
	//SetConsoleTitle(TEXT(GAME_TITLE));
	SetFontSize(5);
	SetScreenSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	SetCursorVisibility(CURSOR_INVISIBLE);

	// 描画先を裏画面に設定
	;

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
